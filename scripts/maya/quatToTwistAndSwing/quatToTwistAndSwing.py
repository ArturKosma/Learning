import maya.api.OpenMaya as om
import math

def maya_useNewAPI():
    pass

node_id = om.MTypeId(0x00227FFF)

# Decompose input quaternion into twist and swing.
def quat_twist_swing(q, twistAxis):    

    # Normalize input quaternion to avoid weird behavior.
    mag2 = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w
    if mag2 > 1e-8:
        mag = math.sqrt(mag2)
        q = om.MQuaternion(q.x/mag, q.y/mag, q.z/mag, q.w/mag)

    # Avoid division by zero.
    if twistAxis.length() < 1e-8:
        return om.MQuaternion(), q
    
    # Normalize the input twist axis.    
    twistAxis.normalize()
    
    # Project rotation axis onto twist axis.
    rotAxis = om.MVector(q.x, q.y, q.z)
    proj = twistAxis * (rotAxis * twistAxis) # Dot.
    
    # Build twist quaternion.
    twist = om.MQuaternion(proj.x, proj.y, proj.z, q.w)
    tmag2 = twist.x*twist.x + twist.y*twist.y + twist.z*twist.z + twist.w*twist.w
    if tmag2 > 1e-8:
        tmag = math.sqrt(tmag2)
        twist = om.MQuaternion(twist.x/tmag, twist.y/tmag, twist.z/tmag, twist.w/tmag)
    else:
        twist = om.MQuaternion()
    
    # Swing is what remains.
    swing = q * twist.inverse()
    return twist, swing

class QuatToTwistAndSwing(om.MPxNode):
    
    inputQuat = None
    inputTwistVec = None
    outputTwistQuat = None
    outputSwingQuat = None
    
    @staticmethod
    def creator():
        return QuatToTwistAndSwing()
        
    @staticmethod
    def initialize():
        
        # Create attributes.
        numAttr = om.MFnNumericAttribute()
                
        # Input quaternion.
        QuatToTwistAndSwing.inputQuat = numAttr.create(
            "inputQuat", "inQuat", om.MFnNumericData.k4Double
        )
        numAttr.keyable = True
        numAttr.storable = True
        numAttr.writable = True
        numAttr.readable = True
        
        # Input twist vector.
        QuatToTwistAndSwing.inputTwistVec = numAttr.create(
            "inputTwistVector", "inTwVec", om.MFnNumericData.k3Float
        )
        numAttr.keyable = True
        numAttr.storable = True
        numAttr.writable = True
        numAttr.readable = True
         
        # Output twist quat.
        QuatToTwistAndSwing.outputTwistQuat = numAttr.create(
            "outputTwistQuat", "outTwQuat", om.MFnNumericData.k4Double, 0.0
        )
        numAttr.keyable = False
        numAttr.storable = False
        numAttr.writable = False
        numAttr.readable = True
        
        # Output swing quat.
        QuatToTwistAndSwing.outputSwingQuat = numAttr.create(
            "outputSwingQuat", "outSwQuat", om.MFnNumericData.k4Double, 0.0
        )
        numAttr.keyable = False
        numAttr.storable = False
        numAttr.writable = False
        numAttr.readable = True
        
        # Add them to the node.
        QuatToTwistAndSwing.addAttribute(QuatToTwistAndSwing.inputQuat)
        QuatToTwistAndSwing.addAttribute(QuatToTwistAndSwing.inputTwistVec)
        QuatToTwistAndSwing.addAttribute(QuatToTwistAndSwing.outputTwistQuat)
        QuatToTwistAndSwing.addAttribute(QuatToTwistAndSwing.outputSwingQuat)
        
        # Make output depend on inputs.
        QuatToTwistAndSwing.attributeAffects(QuatToTwistAndSwing.inputQuat, QuatToTwistAndSwing.outputTwistQuat)
        QuatToTwistAndSwing.attributeAffects(QuatToTwistAndSwing.inputQuat, QuatToTwistAndSwing.outputSwingQuat)
        QuatToTwistAndSwing.attributeAffects(QuatToTwistAndSwing.inputTwistVec, QuatToTwistAndSwing.outputTwistQuat)
        QuatToTwistAndSwing.attributeAffects(QuatToTwistAndSwing.inputTwistVec, QuatToTwistAndSwing.outputSwingQuat)
        
    def compute(self, plug, dataBlock):
        out_tw = om.MPlug(self.thisMObject(), QuatToTwistAndSwing.outputTwistQuat)
        out_sw = om.MPlug(self.thisMObject(), QuatToTwistAndSwing.outputSwingQuat)

        is_twist = (plug == out_tw) or (plug.parent() == out_tw)
        is_swing = (plug == out_sw) or (plug.parent() == out_sw)
        if not (is_twist or is_swing):
            return
            
        # Fetch input twist vector.   
        twistVecHandle = dataBlock.inputValue(QuatToTwistAndSwing.inputTwistVec)   
        x, y, z = twistVecHandle.asFloat3()
        twistVec = om.MVector(x, y, z)
                    
        # Fetch input quaternion.
        quatHandle = dataBlock.inputValue(QuatToTwistAndSwing.inputQuat)
        x, y, z, w = quatHandle.asDouble4()
        quat = om.MQuaternion(x, y, z, w)
      
        # Get twist & swing from it.
        twist, swing = quat_twist_swing(quat, twistVec)
        
        # Set quats on the outputs.
        outHandle_twist = dataBlock.outputValue(QuatToTwistAndSwing.outputTwistQuat)
        outHandle_twist.set4Double(twist.x, twist.y, twist.z, twist.w)
        outHandle_twist.setClean()

        outHandle_swing = dataBlock.outputValue(QuatToTwistAndSwing.outputSwingQuat)
        outHandle_swing.set4Double(swing.x, swing.y, swing.z, swing.w)
        outHandle_swing.setClean()
        
def initializePlugin(mobject):
    pluginFn = om.MFnPlugin(mobject)
    pluginFn.registerNode(
        "quatToTwistAndSwing",
        node_id,
        QuatToTwistAndSwing.creator,
        QuatToTwistAndSwing.initialize
    )
    
def uninitializePlugin(mobject):
    pluginFn = om.MFnPlugin(mobject)
    pluginFn.deregisterNode(node_id)