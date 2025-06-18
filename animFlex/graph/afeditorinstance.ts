import type { ClassicPreset, GetSchemes, NodeEditor } from "rete";

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

export let editorInstance: NodeEditor<Schemes>;
export function SetEditorInstance(editor: NodeEditor<Schemes>) {
    editorInstance = editor;
}