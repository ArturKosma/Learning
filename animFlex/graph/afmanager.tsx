import { createEditor } from "./rete";

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

export async function createView(name: string, id: string) {

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

        const editorInstance = await createEditor(container, id);
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