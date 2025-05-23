import { ClassicPreset } from "rete";
import resultPoseIcon from './resultPose.png';
import poseIcon_connected from './pinPose_connected.png';
import poseIcon_disconnected from './pinPose_disconnected.png';

export const $socketmargin = 6;
export const $socketsize = 16;
export function GetNodeMeta(type: string): any {
    switch (type) {
        case "OutputPose":
            return {
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

export function CreateSockets(type: string, node: ClassicPreset.Node) {

    const socket = new ClassicPreset.Socket("socket");

    // Socket meta.
    socket.meta = {
         socketIconConnected_path: poseIcon_connected,
         socketIconDisconnected_path: poseIcon_disconnected
    }

    switch (type) {
        case "OutputPose":

           const input = new ClassicPreset.Input(socket);
           input.label = "Result";
           node.addInput("inputPose", input);
           break;

        case "PlaySequence":
            
           const output = new ClassicPreset.Output(socket);
           node.addOutput("outputPose", output);
           break;

        default:
            return {}
    }
}