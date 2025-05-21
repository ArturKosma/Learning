import { NodeEditor, GetSchemes, ClassicPreset } from "rete";
import { GetNodeMeta } from "./afnodevars";

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;
export class AFNode {
    node: ClassicPreset.Node;

    private constructor(node: ClassicPreset.Node) {
        this.node = node;
    }

    static async create(type: string, editor: NodeEditor<Schemes>): Promise<AFNode> {
        const node = new ClassicPreset.Node("");

        node.meta = GetNodeMeta(type);

        const socket = new ClassicPreset.Socket("socket");
        node.addInput("b", new ClassicPreset.Input(socket));

        await editor.addNode(node);
        return new AFNode(node);
  }
}