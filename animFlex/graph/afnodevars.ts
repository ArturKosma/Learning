import { ClassicPreset } from "rete";
import resultPoseIcon from './resultPose.png';

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
                bigIcon_path: resultPoseIcon
            };
            break;
        case "PlaySequence":
            return {
                showTitle: true,
                title: "Play Sequence",
                showSubTitle: false,
                nodeWidth: 240,
                nodeHeight: 80,
                bigIcon: false
            }
            break;
        default:
            return {}
    }
} 

export function CreateSockets(type: string, node: ClassicPreset.Node) {
    switch (type) {
        case "OutputPose":

           node.addInput("inputPose", new ClassicPreset.Input(new ClassicPreset.Socket("socket")));
           break;

        case "PlaySequence":
            
           node.addOutput("outputPose", new ClassicPreset.Input(new ClassicPreset.Socket("socket")));
           break;

        default:
            return {}
    }
}