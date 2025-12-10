import maya.cmds as cmds
import maya.api.OpenMaya as om
import math
import maya.mel as mel

joints = cmds.ls(sl=True, type="joint")
bindPoseName = "bindPose1"

# For each joint.
for joint in joints:
    
    mel.eval(f'dagPose -rm -bp -n "{bindPoseName}" "{joint}";')