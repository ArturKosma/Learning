import { ClassicPreset } from "rete";

export const $nodewidth = 240;
export const $nodeheight = 160;
export const $socketmargin = 6;
export const $socketsize = 16;
export function GetNodeMeta(type: string): any {
    switch (type) {
        case "OutputPose":
            return {
                showTitle: true,
                title: "Output Pose",
                showSubTitle: true,
                subTitle: "AnimGraph"
            };
            break;
        case "PlaySequence":
            return {
                showTitle: true,
                title: "Play Sequence",
                showSubTitle: false
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