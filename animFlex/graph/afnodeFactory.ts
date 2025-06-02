import { NodeEditor, GetSchemes, ClassicPreset } from "rete";
import { GetNodeMeta, CreateSockets } from "./affunclib";

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;
export class AFNodeFactory {
    node: ClassicPreset.Node;

    private constructor(node: ClassicPreset.Node) {
        this.node = node
    }

    static create(type: string, editor: NodeEditor<Schemes>, contextMenu: boolean = false): AFNodeFactory {
        const node = new ClassicPreset.Node("") as ClassicPreset.Node & {
            meta: {
                isRemovable: boolean,
                showTitle: boolean,
                title: string,
                showSubTitle: boolean,
                subTitle: string,
                nodeWidth: number,
                nodeHeight: number,
                bigIcon: boolean,
                bigIcon_path: string,
            }
        };

        // Fill meta like color, title.
        node.meta = GetNodeMeta(type);

        // Fill sockets like input, output, dropdowns.
        CreateSockets(type, node, editor);

        if(!contextMenu) {
            editor.addNode(node);
        }
        return new AFNodeFactory(node);
  }
}