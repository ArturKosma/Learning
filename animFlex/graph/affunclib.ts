import { ClassicPreset, NodeEditor, GetSchemes } from "rete";
import {DropdownControl} from './afdropdown'; 
import resultPoseIcon from './resultPose.png';
import poseIcon_connected from './pinPose_connected.png';
import poseIcon_disconnected from './pinPose_disconnected.png';

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

export const $socketmargin = 6;
export const $socketsize = 32;

export function GetNodeMeta(type: string): any {
    switch (type) {
        case "OutputPose":
            return {
                isRemovable: false,
                showTitle: true,
                title: "Output Pose",
                showSubTitle: true,
                subTitle: "AnimGraph",
                nodeWidth: 240,
                nodeHeight: 160,
                bigIcon: true,
                bigIcon_path: resultPoseIcon,
            };
            break;
        case "PlaySequence":
            return {
                isRemovable: true,
                showTitle: true,
                title: "Play Sequence",
                showSubTitle: false,
                nodeWidth: 240,
                nodeHeight: 80,
                bigIcon: false,
            }
            break;
        default:
            return {}
    }
} 

export function CreateSockets(type: string, node: ClassicPreset.Node, editor: NodeEditor<Schemes>) {

    const uid = crypto.randomUUID();
    const socket = new ClassicPreset.Socket(uid) as ClassicPreset.Socket & {
        meta?: {
            editor: NodeEditor<Schemes>,
            node: ClassicPreset.Node,
            socketIconConnected_path: string,
            socketIconDisconnected_path: string
        }
    };

    // Socket meta.
    socket.meta = {
         editor,
         node,
         socketIconConnected_path: poseIcon_connected,
         socketIconDisconnected_path: poseIcon_disconnected
    }

    switch (type) {
        case "OutputPose":

           const input = new ClassicPreset.Input(socket);
           input.label = "Result";
           node.addInput(uid, input);
           break;

        case "PlaySequence":
            
           const output = new ClassicPreset.Output(socket);
           node.addOutput(uid, output);
           node.addControl("sequence-dropdown", new DropdownControl())
           break;

        default:
            return {}
    }
}