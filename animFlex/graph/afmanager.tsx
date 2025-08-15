import { createEditor } from "./rete";
import { createEditorSM, create } from "./retesm";
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

type SerializedNode = {
  id: string;
  type: string;
  label: string;
  isEntry: boolean;
  connectionOwner: string,
  position: { x: number; y: number } | null;
  valuesMap: Record<string, string>;
  pins: {
    inputs: Array<{ id: string; varName: string; socketType?: string }>;
    outputs: Array<{ id: string; varName: string; socketType?: string }>;
  };
};

type SerializedConnection = {
  id: string;
  source: string;
  target: string;
  sourceOutput: string;
  targetInput: string;
};

type SavedEditor = {
  id: string
  name: string
  type: ReteViewType
  nodes: Array<SerializedNode>
  connections: Array<SerializedConnection>
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

      return {
        id: node.id,
        type,
        label: node.label,
        isEntry: (node as any).meta?.isEntry,
        connectionOwner: (node as any).meta?.connectionOwner,
        position: pos ? { x: pos.x, y: pos.y } : null,
        ...(valuesMap ? { valuesMap } : {}),
        pins: { inputs, outputs },
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

export function printAllConnectionIds() {
  for (const e of editors) {
    const conns = e.editor.getConnections() as Array<{
      id: string;
      source: string;
      sourceOutput: string;
      target: string;
      targetInput: string;
    }>;

    console.group(`Editor "${e.name}" (${e.id}) ${conns.length} connection(s)`);
    for (const c of conns) {
      console.log(
        `id=${c.id} | ${c.source}:${c.sourceOutput} -> ${c.target}:${c.targetInput}`
      );
    }
    console.groupEnd();
  }
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

    // Tell the editor we are loading so it doesn't create conditional nodes automatically.
    (editorObject.editor as any).meta ??= {};
    (editorObject.editor as any).meta.isLoading = true;

    // Remove the default created nodes. We want to create them by hand in order to apply saved ID.
    for (const existingNode of existingNodes) {
      await editorObject.editor.removeNode(existingNode.id);
    }

    // Recreate nodes.
    for (const savedNode of savedNodes) {

      let newNode: any = undefined;

      // State Machine graphs don't use NodeFactory to create nodes.
      if(ed.type == 1) {

        const node = await create(savedNode.label, editorObject.editor, editorObject.selector, savedNode.isEntry, savedNode.connectionOwner, false);
        node.label = savedNode.label;
        node.id = savedNode.id;
        newNode = node;

      } else {

        const { node } = await AFNodeFactory.create(savedNode.type, editorObject.editor, true, ed.type, savedNode.id) as {
        node: ClassicPreset.Node };
        newNode = node;

      }

      // Reassign values map.
      (newNode as any).meta.valuesMap = savedNode.valuesMap;

      // Remap input pin IDs.
      for (const saved of savedNode.pins.inputs) {
        const currentKey = Object.keys(newNode.inputs).find(k =>
            ((newNode.inputs[k]?.socket as any).meta?.var_name) === saved.varName
        );
        if (currentKey && currentKey !== saved.id) {
          const inp = newNode.inputs[currentKey];
          (inp?.socket as any).name = saved.id;
          newNode.inputs[saved.id] = inp;
          delete newNode.inputs[currentKey];

          // Load on controls.
          if(savedNode.valuesMap && saved.varName in savedNode.valuesMap) {
            const controlData = savedNode.valuesMap[saved.varName];
            const loadFun = (inp as any).control?.serializeLoad;
            if(loadFun && typeof loadFun == "function") {
              (inp as any).control.serializeLoad(controlData);
            }
          }
        }
      }

      // Remap output pin IDs.
      for (const saved of savedNode.pins.outputs) {
        const currentKey = Object.keys(newNode.outputs).find(k =>
            ((newNode.outputs[k]?.socket as any).meta?.var_name) === saved.varName
        );
        if (currentKey && currentKey !== saved.id) {
          const out = newNode.outputs[currentKey];
          (out?.socket as any).name = saved.id;
          newNode.outputs[saved.id] = out;
          delete newNode.outputs[currentKey];

          // Load on controls.
          if(savedNode.valuesMap && saved.varName in savedNode.valuesMap) {
            const controlData = savedNode.valuesMap[saved.varName];
            const loadFun = (out as any).control?.serializeLoad;
            if(loadFun && typeof loadFun == "function") {
              (out as any).control.serializeLoad(controlData);
            }
          }
        }
      }

      // If this node is a subgraph, it has a name to apply.
      if (ids.includes(savedNode.id)) {
        const nodeName = data.editors[savedNode.id].name;
        (newNode as any).meta.title = nodeName;
      }

      await editorObject.editor.addNode(newNode);
      await editorObject.area.translate(newNode.id, savedNode.position);
    }

    await new Promise<void>(r => requestAnimationFrame(() => requestAnimationFrame(() => r())));

    // Recreate connections.
    for (const c of savedConnections) {
      const srcNode = editorObject.editor.getNode(c.source)!;
      const tgtNode = editorObject.editor.getNode(c.target)!;

      // State machine handles connections differently.
      if(ed.type == 1) {

        // Build the connection.
        const conn = (editorObject.editor as any).meta.createSMConnection(srcNode, tgtNode);

        // Override the connection ID.
        conn.id = c.id;

        await editorObject.editor.addConnection(conn);

        // Refresh connections.
        editorObject.area.update('connection', conn.id);
        
      } else {

        // Build the connection.
        const conn = new ClassicPreset.Connection(
            srcNode,
            c.sourceOutput,
            tgtNode,
            c.targetInput
        ) as any;

        // Override the connection ID.
        conn.id = c.id;

        await editorObject.editor.addConnection(conn);

        // Mark sockets as connected.
        const out = srcNode.outputs[c.sourceOutput];
        const inp = tgtNode.inputs[c.targetInput];
        if ((out as any)?.socket?.meta) (out as any).socket.meta.isConnected = true;
        if ((inp as any)?.socket?.meta) (inp as any).socket.meta.isConnected = true;
      }
    }

    // Nudge all the nodes.
    for (const n of editorObject.editor.getNodes()) {
      editorObject.area.update('node', n.id);
    }

    await new Promise<void>(r => requestAnimationFrame(() => r()));

    // Re-enable normal behavior.
    (editorObject.editor as any).meta.isLoading = false;
  }
}