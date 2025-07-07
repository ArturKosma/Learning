import { NodeEditor, GetSchemes, ClassicPreset } from "rete";
import { GetNodeMeta, CreateSockets } from "./affunclib";
import { ReteViewType } from "./afmanager";

export type GraphNodeParam = {
  var_type: string;
  var_name: string;
  label: string;
  direction: string;
};

export type GraphNode = {
  class_id: string;
  node_name: string;
  params: GraphNodeParam[];
  meta: string[];
};

export const classIdToName: Map<string, string> = new Map();
export const classIdToParams: Map<string, GraphNodeParam[]> = new Map();
export const classIdToMeta: Map<string, string[]> = new Map();

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;
export class AFNodeFactory {
    node: ClassicPreset.Node;

    private constructor(node: ClassicPreset.Node) {
        this.node = node
    }

    static create(type: string, editor: NodeEditor<Schemes>, contextMenu: boolean = false, graphType: ReteViewType): AFNodeFactory {
        const node = new ClassicPreset.Node("") as ClassicPreset.Node & {
            meta: {
                type: string,
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
        node.meta = GetNodeMeta(type, graphType);

        // Fill sockets like input, output, dropdowns.
        CreateSockets(node, editor);

        if(!contextMenu) {
            editor.addNode(node);
        }
        return new AFNodeFactory(node);
  }
}