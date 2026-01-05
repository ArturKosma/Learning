import maya.cmds as cmds
import maya.api.OpenMaya as om

def _wm(node):
    return om.MMatrix(cmds.xform(node, q=True, ws=True, m=True))

def _parent_inv(node):
    p = cmds.listRelatives(node, p=True, f=True)
    if p:
        return om.MMatrix(cmds.getAttr(p[0] + ".worldInverseMatrix[0]"))
    return om.MMatrix()  # identity

def _world_scale(node):
    tm = om.MTransformationMatrix(_wm(node))
    s = tm.scale(om.MSpace.kWorld)
    return s[0], s[1], s[2]

def _shapes(node):
    return cmds.listRelatives(node, s=True, ni=True, f=True) or []

def _bake_scale_into_shapes(ctrl, sx, sy, sz):
    for sh in _shapes(ctrl):
        try:
            cmds.scale(sx, sy, sz, sh + ".cv[*]", r=True, os=True)
        except Exception:
            pass

def _bake_inv_rotation_into_shapes(ctrl, inv_q):
    # Convert quaternion -> euler (radians) safely
    e = inv_q.asEulerRotation()

    rx = om.MAngle(e.x).asDegrees()
    ry = om.MAngle(e.y).asDegrees()
    rz = om.MAngle(e.z).asDegrees()

    for sh in _shapes(ctrl):
        try:
            cmds.rotate(rx, ry, rz, sh + ".cv[*]", r=True, os=True)
        except Exception:
            pass

def freeze_align_axis_keep_visual(ctrl, target):
    # Bake existing world scale into shapes so scale can become 1
    sx, sy, sz = _world_scale(ctrl)

    # Control current world TR + R
    ctrl_tm = om.MTransformationMatrix(_wm(ctrl))
    t = ctrl_tm.translation(om.MSpace.kWorld)
    q_cur = ctrl_tm.rotation(asQuaternion=True)

    # Target world rotation (desired axes)
    tgt_tm = om.MTransformationMatrix(_wm(target))
    q_tgt = tgt_tm.rotation(asQuaternion=True)

    # Axis change delta; counter-rotate shapes so visuals don't change
    delta = q_tgt * q_cur.inverse()
    _bake_inv_rotation_into_shapes(ctrl, delta.inverse())

    # Set offsetParentMatrix to (same world pos) + (target rotation), no scale
    clean_world = om.MTransformationMatrix()
    clean_world.setTranslation(t, om.MSpace.kWorld)
    clean_world.setRotation(q_tgt)

    opm = clean_world.asMatrix() * _parent_inv(ctrl)
    cmds.setAttr(ctrl + ".offsetParentMatrix", list(opm), type="matrix")

    _bake_scale_into_shapes(ctrl, sx, sy, sz)

    # Zero channels
    cmds.setAttr(ctrl + ".translate", 0, 0, 0)
    cmds.setAttr(ctrl + ".rotate", 0, 0, 0)
    cmds.setAttr(ctrl + ".scale", 1, 1, 1)

    if cmds.attributeQuery("rotateAxis", node=ctrl, exists=True):
        cmds.setAttr(ctrl + ".rotateAxis", 0, 0, 0)

# ---- run ----
sel = cmds.ls(sl=True, long=True) or []
if len(sel) < 2:
    cmds.error("Select control(s) first, then the target joint LAST.")

target = sel[-1]
for ctrl in sel[:-1]:
    freeze_align_axis_keep_visual(ctrl, target)
