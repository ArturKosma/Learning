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

def zeroLocalTransform(joint):
    cmds.xform(joint, os=True, t=[0, 0, 0], ro=[0, 0, 0])

def snapFKIK(limb, currentValue):

    if limb == "LeftArm":
        if currentValue == 0: # Currently FK.
            snapTransform("ctrl_LeftHand_IK", "LeftHand")
            snapTransform("ctrl_LeftForearm_IKPoleVector", "LeftForeArm", 
                          offset={"t": [0, 0, -20], "r": [0, 0, 0]})
            cmds.setAttr(f"FKIK_LeftArm.FKIKMode", 1)
            
        if currentValue == 1: # Currently IK.
            snapTransform("ctrl_LeftArm_FK", "LeftArm")
            snapTransform("ctrl_LeftForeArm_FK", "LeftForeArm")
            snapTransform("ctrl_LeftHand_FK", "LeftHand")
            cmds.setAttr(f"FKIK_LeftArm.FKIKMode", 0)

    if limb == "RightArm":
        if currentValue == 0: # Currently FK.
            snapTransform("ctrl_RightHand_IK", "RightHand")
            snapTransform("ctrl_RightForearm_IKPoleVector", "RightForeArm", 
                          offset={"t": [0, 0, 20], "r": [0, 0, 0]})
            cmds.setAttr(f"FKIK_RightArm.FKIKMode", 1)
            
        if currentValue == 1: # Currently IK.
            snapTransform("ctrl_RightArm_FK", "RightArm")
            snapTransform("ctrl_RightForeArm_FK", "RightForeArm")
            snapTransform("ctrl_RightHand_FK", "RightHand")
            cmds.setAttr(f"FKIK_RightArm.FKIKMode", 0)

    if limb == "LeftLeg":
        if currentValue == 0: # Currently FK.
            snapTransform("ctrl_LeftFoot_IK", "LeftFoot")
            snapTransform("ctrl_LeftLeg_IKPoleVector", "LeftLeg", 
                          offset={"t": [0, 0, 40], "r": [0, 0, 0]})
            zeroLocalTransform("ctrl_LeftFoot_IK_tap")
            zeroLocalTransform("ctrl_LeftFoot_IK_tip")
            zeroLocalTransform("ctrl_LeftFoot_IK_bankL")
            zeroLocalTransform("ctrl_LeftFoot_IK_bankR")
            zeroLocalTransform("ctrl_LeftFoot_IK_heel")
            cmds.setAttr(f"FKIK_LeftLeg.FKIKMode", 1)
            
        if currentValue == 1: # Currently IK.
            snapTransform("ctrl_LeftUpLeg_FK", "LeftUpLeg")
            snapTransform("ctrl_LeftLeg_FK", "LeftLeg")
            snapTransform("ctrl_LeftFoot_FK", "LeftFoot")
            snapTransform("ctrl_LeftToeBase_FK", "LeftToeBase")
            cmds.setAttr(f"FKIK_LeftLeg.FKIKMode", 0)

    if limb == "RightLeg":
        if currentValue == 0: # Currently FK.
            snapTransform("ctrl_RightFoot_IK", "RightFoot")
            snapTransform("ctrl_RightLeg_IKPoleVector", "RightLeg", 
                          offset={"t": [0, 0, -40], "r": [0, 0, 0]})
            zeroLocalTransform("ctrl_RightFoot_IK_tap")
            zeroLocalTransform("ctrl_RightFoot_IK_tip")
            zeroLocalTransform("ctrl_RightFoot_IK_bankL")
            zeroLocalTransform("ctrl_RightFoot_IK_bankR")
            zeroLocalTransform("ctrl_RightFoot_IK_heel")
            cmds.setAttr(f"FKIK_RightLeg.FKIKMode", 1)
            
        if currentValue == 1: # Currently IK.
            snapTransform("ctrl_RightUpLeg_FK", "RightUpLeg")
            snapTransform("ctrl_RightLeg_FK", "RightLeg")
            snapTransform("ctrl_RightFoot_FK", "RightFoot")
            snapTransform("ctrl_RightToeBase_FK", "RightToeBase")
            cmds.setAttr(f"FKIK_RightLeg.FKIKMode", 0)

sel = cmds.ls(selection=True)

if not sel:
    cmds.warning("No object selected")
else:
    objs = sel
    for obj in objs:
        if cmds.attributeQuery("FKIKMode", node=obj, exists=True):

            currentValue = cmds.getAttr(f"{obj}.FKIKMode")
            limb = ""

            if "LeftArm" in obj:
                snapFKIK("LeftArm", currentValue)

            if "RightArm" in obj:
                snapFKIK("RightArm", currentValue)

            if "LeftLeg" in obj:
                snapFKIK("LeftLeg", currentValue)

            if "RightLeg" in obj:
                snapFKIK("RightLeg", currentValue)