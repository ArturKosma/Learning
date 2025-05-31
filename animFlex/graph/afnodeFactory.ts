import { NodeEditor, GetSchemes, ClassicPreset } from "rete";
import { GetNodeMeta, CreateSockets } from "./affunclib";

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;
export class AFNodeFactory {
    node: ClassicPreset.Node;

    private constructor(node: ClassicPreset.Node) {
        this.node = node;
    }

    static async create(type: string, editor: NodeEditor<Schemes>): Promise<AFNodeFactory> {
        const node = new ClassicPreset.Node("");

        // Fill meta like color, title.
        node.meta = GetNodeMeta(type);

        // Fill sockets like input, output.
        CreateSockets(type, node, editor);

        await editor.addNode(node);
        return new AFNodeFactory(node);
  }
}