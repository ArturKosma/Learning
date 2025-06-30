import { createEditor } from "./rete";

const editors: { id: string, container: HTMLElement, editor: any, destroy: () => void }[] = [];
const parentContainer = document.getElementById('rete');
const viewLabel = document.getElementById('view-label');

export async function createView(viewId: string) {
    const container = document.createElement('div');
    container.id = viewId;
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

    const editorInstance = await createEditor(container);
    editors.push({ id: viewId, container, ...editorInstance });
}

export function switchToView(viewId: string) {
    editors.forEach(editor => {
        if (editor.id === viewId) {
            editor.container.style.display = 'block';
        } else {
            editor.container.style.display = 'none';
        }
    });

    viewLabel.textContent = `${viewId}`;
}