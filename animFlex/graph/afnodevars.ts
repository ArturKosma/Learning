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
        case "PlaySequence":
            return {
                showTitle: true,
                title: "Play Sequence",
                showSubTitle: false
            }
        default:
            return {}
    }
} 