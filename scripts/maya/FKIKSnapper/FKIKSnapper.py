import maya.cmds as cmds
import maya.api.OpenMaya as om

def _get_world_matrix(node):

    mat = cmds.xform(node, q=True, ws=True, m=True)

    return om.MMatrix(mat)

def ensure_matrix_attr(node, attr):
    plug = f"{node}.{attr}"
    if not cmds.objExists(plug):
        cmds.addAttr(node, longName=attr, attributeType="matrix")
        cmds.setAttr(plug, channelBox=True)
    return plug

def write_relative_matrix_to_attr(attr_owner, attr_name="FKIKChildOffset"):
    sel = cmds.ls(sl=True, long=True) or []
    if len(sel) < 2:
        raise RuntimeError("Select at least two transforms: first=A, second=B")

    a = sel[0]
    b = sel[1]

    a_w = _get_world_matrix(a)
    b_w = _get_world_matrix(b)

    rel = b_w * a_w.inverse()

    plug = ensure_matrix_attr(attr_owner, attr_name)

    # Set matrix attribute (needs 16 floats + type="matrix")
    cmds.setAttr(plug, *list(rel), type="matrix")

    return a, b, plug

# Usage:
# Select A then B, and store on B (or change to any node you want)
a, b, plug = write_relative_matrix_to_attr(attr_owner=cmds.ls(sl=True, long=True)[1],
                                          attr_name="FKIKChildOffset")
print("Wrote inv(A_world) * B_world to:", plug)