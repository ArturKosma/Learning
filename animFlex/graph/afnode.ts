import { NodeEditor, GetSchemes, ClassicPreset } from "rete";
import { GetNodeMeta, CreateSockets } from "./afnodevars";

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;
export class AFNode {
    node: ClassicPreset.Node;

    private constructor(node: ClassicPreset.Node) {
        this.node = node;
    }

    static async create(type: string, editor: NodeEditor<Schemes>): Promise<AFNode> {
        const node = new ClassicPreset.Node("");

        // Fill meta like color, title.
        node.meta = GetNodeMeta(type);

        // Fill sockets like input, output.
        CreateSockets(type, node);

        await editor.addNode(node);
        return new AFNode(node);
  }
}