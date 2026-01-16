import maya.cmds as cmds
import maya.api.OpenMaya as om

def _get_world_matrix(node):
    flat = cmds.getAttr(node + ".worldMatrix[0]")

    print(flat)

_get_world_matrix(cmds.ls(sl=True))