import maya.cmds as cmds

def snapTransform(beingMoved, movedTo, offset=None):

    pos = cmds.xform(movedTo, q=True, ws=True, t=True)
    rot = cmds.xform(movedTo, q=True, ws=True, ro=True)

    cmds.xform(beingMoved, ws=True, t=pos)
    cmds.xform(beingMoved, ws=True, ro=rot)

    local_pos = cmds.xform(beingMoved, q=True, os=True, t=True)
    local_rot = cmds.xform(beingMoved, q=True, os=True, ro=True)

    if offset:
        local_pos = [
            local_pos[0] + offset.get("t", [0, 0, 0])[0],
            local_pos[1] + offset.get("t", [0, 0, 0])[1],
            local_pos[2] + offset.get("t", [0, 0, 0])[2],
        ]

        local_rot = [
            local_rot[0] + offset.get("r", [0, 0, 0])[0],
            local_rot[1] + offset.get("r", [0, 0, 0])[1],
            local_rot[2] + offset.get("r", [0, 0, 0])[2],
        ]

    cmds.xform(beingMoved, os=True, t=local_pos)
    cmds.xform(beingMoved, os=True, ro=local_rot)

def snapFKIK(limb, currentValue):

    if limb == "LeftArm":
        if currentValue == 0: # FK.
            snapTransform("ctrl_LeftHand_IK", "LeftHand")
            snapTransform("ctrl_LeftForearm_IKPoleVector", "LeftForeArm", 
                          offset={"t": [0, 0, -20], "r": [0, 0, 0]})
            cmds.setAttr(f"FKIK_LeftArm.FKIKMode", 1)
            
        if currentValue == 1: # IK.
            snapTransform("ctrl_LeftArm_FK", "LeftArm")
            snapTransform("ctrl_LeftForeArm_FK", "LeftForeArm")
            snapTransform("ctrl_LeftHand_FK", "LeftHand")
            cmds.setAttr(f"FKIK_LeftArm.FKIKMode", 0)

sel = cmds.ls(selection=True)

if not sel:
    cmds.warning("No object selected")
else:
    obj = sel[0]
    if cmds.attributeQuery("FKIKMode", node=obj, exists=True):

        currentValue = cmds.getAttr(f"{obj}.FKIKMode")
        limb = ""

        if "LeftArm" in obj:
            limb = "LeftArm"
        
        if limb == "LeftArm":
            snapFKIK("LeftArm", currentValue)
			
