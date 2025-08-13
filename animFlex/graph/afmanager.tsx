import { createEditor } from "./rete";
import { createEditorSM } from "./retesm";
import { createEditorCond } from "./reteCond";
import { ReteViewType } from "./aftypes";
import { ClassicPreset, GetSchemes, NodeEditor } from "rete";
import { AreaExtensions } from "rete-area-plugin";
import { SelectorEntity } from "rete-area-plugin/_types/extensions/selectable";
import { GraphNode, AFEnum } from "./afnodeFactory";

let currentView : any;

export function getCurrentView() {
    return currentView;
} 

const editors: { 
        name: string,
        id: string,
        container: HTMLElement, 
        editor: any, 
        selector: any,
        area: any,
        destroy: () => void 
    }[] = [];

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

interface EditorInstance {
    editor: any, 
    selector: any,
    destroy: () => void 
}

export async function createView(name: string, id: string, type: ReteViewType) {

    const entry = editors.find(e => e.id === id);
    if(entry) {

    }
    else {
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

        editors.push({name: name, id: id, container, ...editorInstance });
    }

    breadcrumbs.push({name, id});
    switchToView(id);
}

export function switchToView(id: string) {
    const entry = editors.find(e => e.id === id);
    if(entry) {

        // Deselect all previously selected nodes.
        entry.selector.unselectAll();

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

async function delKey(editor: NodeEditor<Schemes>, selector: AreaExtensions.Selector<SelectorEntity>) {

    for (const selectedEntity of selector.entities.values()) {
        const node = editor.getNode(selectedEntity.id) as ClassicPreset.Node & {
            meta?: {isRemovable?: boolean}
        };

        if(node?.meta?.isRemovable) {
            const connections = editor.getConnections();
            
            // Remove all connections where this node is source or target
            for (const conn of connections) {
                if (
                conn.source === node.id ||
                conn.target === node.id  
                ) {
                await editor.removeConnection(conn.id);
                }
            }

            await editor.removeNode(selectedEntity.id);
        }
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

export async function save() {
    console.log("im trying to kurwa save");
}