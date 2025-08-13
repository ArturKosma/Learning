import { createEditor } from "./rete";
import { createEditorSM } from "./retesm";
import { createEditorCond } from "./reteCond";
import { ReteViewType } from "./aftypes";
import { ClassicPreset, GetSchemes, NodeEditor } from "rete";
import { AreaExtensions } from "rete-area-plugin";
import { SelectorEntity } from "rete-area-plugin/_types/extensions/selectable";
import { GraphNode, AFEnum, AFNodeFactory } from "./afnodeFactory";
import { makeConnection } from "rete-connection-plugin";

let currentView : any;

export function getCurrentView() {
    return currentView;
} 

type SavedEditor = {
  id: string
  name: string
  type: ReteViewType
  nodes: Array<{
    id: string
    type: string
    valuesMap: Record<string, string>
    position: { x: number; y: number } | null
    meta?: any
    pins: {
      inputs: Array<{ id: string; varName: string; socketType?: string }>;
      outputs: Array<{ id: string; varName: string; socketType?: string }>;
    };
  }>
  connections: Array<{
    id: string
    source: string
    sourceOutput: string
    target: string
    targetInput: string
  }>
}

type SavedProject = {
  editors: Record<string, SavedEditor>
}

type EditorInstance = {
    editor: any, 
    selector: any,
    area: any,
    destroy: () => void 
}

type Editor = {
    name: string,
    id: string,
    container: HTMLElement, 
    editor: any, 
    selector: any,
    area: any,
    destroy: () => void,
    type: ReteViewType
}
const editors: Editor[] = [];

const parentContainer = document.getElementById('rete');
const viewLabel = document.getElementById('view-label');
const breadcrumbs: { name: string; id: string }[] = [];

function updateLabel() {
    viewLabel.innerHTML = '';

    breadcrumbs.forEach((crumbData, i) => {
        const { name, id } = crumbData;

        const crumb = document.createElement('span');
        crumb.textContent = name;
        crumb.classList.add('crumb');
        crumb.style.cursor = 'pointer';
        crumb.addEventListener('click', () => {
            switchToView(id);
        });
        viewLabel.appendChild(crumb);
        if (i < breadcrumbs.length - 1) {
            const sep = document.createElement('span');
            sep.textContent = ' › ';
            sep.classList.add('separator');
            viewLabel.appendChild(sep);
        }
    });
}

export async function createView(name: string, id: string, type: ReteViewType, switchView: boolean = true): Promise<Editor> {

    const existing = editors.find(e => e.id === id);
    if(existing) {
        switchToView(existing.id);
        return existing;
    }
    const container = document.createElement('div');
    container.id = id;
    container.style.cssText = `
        width: 100%;
        height: 100%;
        position: absolute;
        top: 0;
        left: 0;
        background: #202020;
        overflow: hidden;
        display: none;
    `;
    parentContainer?.appendChild(container);

    let editorInstance: EditorInstance;
    switch(type) {
        case ReteViewType.Graph: {
            editorInstance = await createEditor(container, id);
            break;
        }
        case ReteViewType.StateMachine: {
            editorInstance = await createEditorSM(container, id);
            break;
        }
        case ReteViewType.ConditionalGraph: {
            editorInstance = await createEditorCond(container, id);
            break;
        }

    }

    const entry: Editor = {name, id, container, ...editorInstance, type};
    editors.push(entry);

    breadcrumbs.push({name, id});

    if(switchView) switchToView(id);

    return entry;
}

export function switchToView(id: string) {
    const entry = editors.find(e => e.id === id);
    if(entry) {

        // Deselect all previously selected nodes.
        // entry.selector.unselectAll();

        // Show target container, hide all others.
        editors.forEach(e => {
            e.container.style.display = (e.id === id ? 'block' : 'none');
        });

        // Manage breadcrumbs label.
        const idx = breadcrumbs.findIndex(c => c.id === id);
        if (idx >= 0) {
            breadcrumbs.splice(idx + 1);
        } else {
            breadcrumbs.push({ name: entry.name, id });
        }
        updateLabel();

        currentView = entry;
    }
}

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

async function destroyEditorRecursively(editorId: string, visited = new Set<string>()) {
  const idx = editors.findIndex(e => e.id === editorId);
  if (idx === -1) return;
  if (visited.has(editorId)) return;
  visited.add(editorId);

  const entry = editors[idx];

  const nodes = entry.editor.getNodes() as Array<{ id: string }>;
  for (const n of nodes) {
    const child = editors.find(e => e.id === n.id);
    if (child) {
      await destroyEditorRecursively(child.id, visited);
    }
  }

  for (const conn of entry.editor.getConnections()) {
    await entry.editor.removeConnection(conn.id);
  }

  for (const n of entry.editor.getNodes()) {
    await entry.editor.removeNode(n.id);
  }

  entry.destroy?.();

  if (entry.container.parentElement) {
    entry.container.parentElement.removeChild(entry.container);
  }

  editors.splice(idx, 1);
}

async function delKey(
  editor: NodeEditor<Schemes>,
  selector: AreaExtensions.Selector<SelectorEntity>,
  force: boolean = false
) {
  const selectedEntities = Array.from(selector.entities.values());

  for (const selectedEntity of selectedEntities) {
    const node = editor.getNode(selectedEntity.id) as ClassicPreset.Node & {
      meta?: { isRemovable?: boolean }
    };

    if (!node?.meta?.isRemovable && !force) continue;

    // Remove all connections for this node in the current editor.
    for (const conn of editor.getConnections()) {
      if (conn.source === node.id || conn.target === node.id) {
        await editor.removeConnection(conn.id);
      }
    }

    // Remove the node from the current editor.
    await editor.removeNode(selectedEntity.id);

    // If this node represents a subgraph, nuke it (and its descendants).
    await destroyEditorRecursively(node.id);
  }
}

export let nodes: GraphNode[] = [];
export let enums: AFEnum[] = [];
export async function setManifest() {
    
    // Read and parse JSON graph manifest.
    const res = await fetch('./graphManifest.json');
    const data = await res.json();
    nodes = data.Nodes;
    enums = data.Enums;
}

export function getManifestNodes() {
    return nodes;
}

export function getManifestEnums() {
    return enums;
}

window.addEventListener('keydown', async (e) => {
    // Delete for nodes deletion.
    const current = getCurrentView();
    if(e.key === 'Delete') {
        await delKey(current?.editor, current?.selector)
    }
  }, {capture: true})

export async function save(): Promise<string> {
  type SerializedNode = {
    id: string;
    type: string;
    position: { x: number; y: number } | null;
    valuesMap: Record<string, string>;
    pins: {
      inputs: Array<{ id: string; varName: string; socketType?: string }>;
      outputs: Array<{ id: string; varName: string; socketType?: string }>;
    };
    pinsByVarName: Record<string, string>;
  };

  type SerializedConnection = {
    id: string;
    source: string;
    target: string;
    sourceOutput: string; // pin id on source node
    targetInput: string;  // pin id on target node
  };

  const project = {
    editors: {} as Record<
      string,
      {
        id: string;
        name: string;
        type: ReteViewType;
        nodes: SerializedNode[];
        connections: SerializedConnection[];
      }
    >
  };

  for (const e of editors) {
    const nodes = e.editor.getNodes();
    const conns = e.editor.getConnections();

    const serializedNodes: SerializedNode[] = nodes.map((node: any) => {
      const view = e.area?.nodeViews?.get(node.id);
      const pos = view ? view.position : null;

      const type = (node as any).meta?.type ?? "Node";
      const valuesMap: Record<string, string> | undefined =
        (node as any).meta?.valuesMap &&
        Object.keys(node.meta.valuesMap).length > 0
          ? { ...(node as any).meta.valuesMap }
          : undefined;

      const inputs = Object.entries(node.inputs ?? {}).map(
        ([pinId, input]: [string, any]) => ({
          id: pinId,
          varName: (input?.socket as any)?.meta?.var_name ?? "",
          socketType: (input?.socket as any)?.meta?.socketType ?? undefined
        })
      );

      const outputs = Object.entries(node.outputs ?? {}).map(
        ([pinId, output]: [string, any]) => ({
          id: pinId,
          varName: (output?.socket as any)?.meta?.var_name ?? "",
          socketType: (output?.socket as any)?.meta?.socketType ?? undefined
        })
      );

      const pinsByVarName: Record<string, string> = {};
      for (const p of inputs) if (p.varName) pinsByVarName[p.varName] = p.id;
      for (const p of outputs) if (p.varName) pinsByVarName[p.varName] = p.id;

      return {
        id: node.id,
        type,
        position: pos ? { x: pos.x, y: pos.y } : null,
        ...(valuesMap ? { valuesMap } : {}),
        pins: { inputs, outputs },
        pinsByVarName
      };
    });

    const serializedConns: SerializedConnection[] = conns.map((c: any) => ({
      id: c.id,
      source: c.source,
      target: c.target,
      sourceOutput: c.sourceOutput,
      targetInput: c.targetInput 
    }));

    project.editors[e.id] = {
      id: e.id,
      name: e.name,
      type: e.type,
      nodes: serializedNodes,
      connections: serializedConns
    };
  }

  return JSON.stringify(project, null, 2);
}

export async function clear() {
    const mainEditorEntry = editors.find(e => e.id === "0");
    if (!mainEditorEntry) return;

    await destroyEditorRecursively(mainEditorEntry.id);
}

export async function load(data: SavedProject) {
    
    // If there's no editors in the json - something went very wrong.
    if (!data?.editors) return;

    // Fetch all the saved editor IDs from the json.
    const ids = Object.keys(data.editors);

    // Create all the editors that were saved.
    for (const id of ids) {

        const ed = data.editors[id];
        await createView(ed.name, id, ed.type, false);

        const savedNodes = ed.nodes;
        const savedConnections = ed.connections;
        const editorObject = editors.find((e) => {return e.id == id});
        if(!editorObject) continue;
        const existingNodes = editorObject.editor.getNodes();

        // Remove the default created nodes. We want to create them by hand in order to apply saved ID.
        for (const existingNode of existingNodes) {
            await editorObject.editor.removeNode(existingNode.id);
        }

        // Recreate nodes.
        for (const savedNode of savedNodes) {

           const { node } = await AFNodeFactory.create(savedNode.type, editorObject.editor, true, ed.type, savedNode.id) as {
                node: ClassicPreset.Node };

            // Remap input pin IDs.
            for (const saved of savedNode.pins.inputs) {
                const currentKey = Object.keys(node.inputs).find(k =>
                    ((node.inputs[k]?.socket as any).meta?.var_name) === saved.varName
                );
                if (currentKey && currentKey !== saved.id) {
                    const inp = node.inputs[currentKey];
                    (inp?.socket as any).name = saved.id;
                    node.inputs[saved.id] = inp;
                    delete node.inputs[currentKey];
                }
            }

            // Remap output pin IDs.
            for (const saved of savedNode.pins.outputs) {
                const currentKey = Object.keys(node.outputs).find(k =>
                    ((node.outputs[k]?.socket as any).meta?.var_name) === saved.varName
                );
                if (currentKey && currentKey !== saved.id) {
                    const out = node.outputs[currentKey];
                    (out?.socket as any).name = saved.id;
                    node.outputs[saved.id] = out;
                    delete node.outputs[currentKey];
                }
            }

            await editorObject.editor.addNode(node);
            await editorObject.area.translate(node.id, savedNode.position);
        }

        // Recreate connections.
        for (const c of savedConnections) {
            await editorObject.editor.addConnection(
                new ClassicPreset.Connection(
                    await editorObject.editor.getNode(c.source), 
                    c.sourceOutput, 
                    await editorObject.editor.getNode(c.target), 
                    c.targetInput)
                );
        }
    }
}