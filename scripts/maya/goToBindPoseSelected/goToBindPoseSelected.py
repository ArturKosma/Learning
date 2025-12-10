import maya.api.OpenMaya as om
import maya.cmds as cmds

joints = cmds.ls(sl=True, type="joint")
cluster = cmds.ls("skinCluster4")[0]

influences = cmds.skinCluster(cluster, q=True, inf=True) or []
indices = cmds.getAttr(cluster + ".matrix", multiIndices=True)

inf_to_index = dict(zip(influences, indices))

for joint in joints:
    
    infs = cmds.skinCluster(cluster, q=True, inf=True) or []
    idx = inf_to_index[joint]
    
    plug = "%s.bindPreMatrix[%d]" % (cluster, idx)
    m_list = cmds.getAttr(plug)
    bind_pre = om.MMatrix(m_list)
    bind_world = bind_pre.inverse()
    cmds.xform(joint, ws=True, m=bind_world)