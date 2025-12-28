import maya.api.OpenMaya as om
import math

def maya_useNewAPI():
    pass

node_id = om.MTypeId(0x00311FFF)

# Safe slerp on shortest arc.
def quat_slerp(q0, q1, t):
    
    # Make sure input quaternions are normalized.
    q0 = q0.normal()
    q1 = q1.normal()

    # Force shortest arc.
    if (q0.x*q1.x + q0.y+q1.y + q0.z+q1.z + q0.w+q1.w) < 0.0:
        q1 = om.MQuaternion(-q1.x, -q1.y, -q1.z, -q1.w)

    return om.MQuaternion.slerp(q0, q1, t)

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

# Get final euler from the driver.
def euler_from_driver(inLocalPose, inLocalRefPose, inDriverMode, inTwistVector, inScale):

    # Find delta matrix.
    localPose = inLocalPose.asMatrix()
    localRefPose = inLocalRefPose.asMatrix()
    localRefPoseInv = localRefPose.inverse()
    localDelta = localPose * localRefPoseInv
    localDeltaTR = om.MTransformationMatrix(localDelta)

    # Get twist & swing from the delta.
    localDeltaQuat = localDeltaTR.rotation(asQuaternion=True)
    twist, swing = quat_twist_swing(localDeltaQuat, inTwistVector)

    # Decide which quaternion we use.
    driverQuat = None
    if inDriverMode == 0:
        driverQuat = twist
    elif inDriverMode == 1:
        driverQuat = swing
    elif inDriverMode == 2:
        driverQuat = localDeltaQuat
    driverQuat = driverQuat.normal()

    # Inverse the quaternion if scale is negative.
    if inScale < 0.0:
        driverQuat = driverQuat.inverse()

    # Make sure scale is <0.0, 1.0>
    inScale = abs(inScale)

    # Slerp the quat.
    outQuat = quat_slerp(om.MQuaternion(0.0, 0.0, 0.0, 1.0), driverQuat, inScale)
    outEuler = outQuat.asEulerRotation()
    outEuler.x = math.degrees(outEuler.x)
    outEuler.y = math.degrees(outEuler.y)
    outEuler.z = math.degrees(outEuler.z)
    return outEuler

class CorrectiveDriver(om.MPxNode):
    
    inputLocalPose = None
    inputLocalRefPose = None
    inputDriverMode = None
    inputTwistVector = None
    inputScale = None
    outputEuler = None
    
    @staticmethod
    def creator():
        return CorrectiveDriver()
        
    @staticmethod
    def initialize():
        
        # Create attributes.
        numAttr = om.MFnNumericAttribute()
        matAttr = om.MFnMatrixAttribute()
        enumAttr = om.MFnEnumAttribute()
                
        # Input local pose.
        CorrectiveDriver.inputLocalPose = matAttr.create(
            "inputPose", "inPose", om.MFnMatrixAttribute.kDouble
        )
        matAttr.keyable = True
        matAttr.storable = True
        matAttr.writable = True
        matAttr.readable = True
        
       # Input local ref pose.
        CorrectiveDriver.inputLocalRefPose = matAttr.create(
            "inputRefPose", "inRefPose", om.MFnMatrixAttribute.kDouble
        )
        matAttr.keyable = True
        matAttr.storable = True
        matAttr.writable = True
        matAttr.readable = True
         
        # Input driver mode.
        CorrectiveDriver.inputDriverMode = enumAttr.create(
            "inputDriverMode", "inDrvMode", 0
        )
        enumAttr.addField("TwistOnly", 0)
        enumAttr.addField("SwingOnly", 1)
        enumAttr.addField("TwistAndSwing", 2)
        enumAttr.keyable = True
        enumAttr.storable = True
        enumAttr.writable = True
        enumAttr.readable = True
        enumAttr.channelBox = True

        # Input twist vector.
        CorrectiveDriver.inputTwistVector = numAttr.create(
            "inputTwistVector", "inTwVec", om.MFnNumericData.k3Float
        )
        numAttr.keyable = True
        numAttr.storable = True
        numAttr.writable = True
        numAttr.readable = True
        
        # Input scale.
        CorrectiveDriver.inputScale = numAttr.create(
            "inputScale", "inScale", om.MFnNumericData.kDouble, 0.0
        )
        numAttr.setMin(-1.0)
        numAttr.setMax(1.0)
        numAttr.keyable = True
        numAttr.storable = True
        numAttr.writable = True
        numAttr.readable = True
        numAttr.channelBox = True

        # Output euler.
        CorrectiveDriver.outputEuler = numAttr.create(
            "outputEuler", "outEuler", om.MFnNumericData.k3Double
        )
        numAttr.keyable = False
        numAttr.storable = False
        numAttr.writable = False
        numAttr.readable = True
        
        # Add them to the node.
        CorrectiveDriver.addAttribute(CorrectiveDriver.inputLocalPose)
        CorrectiveDriver.addAttribute(CorrectiveDriver.inputLocalRefPose)
        CorrectiveDriver.addAttribute(CorrectiveDriver.inputDriverMode)
        CorrectiveDriver.addAttribute(CorrectiveDriver.inputTwistVector)
        CorrectiveDriver.addAttribute(CorrectiveDriver.inputScale)
        CorrectiveDriver.addAttribute(CorrectiveDriver.outputEuler)
        
        # Make output depend on inputs.
        CorrectiveDriver.attributeAffects(CorrectiveDriver.inputLocalPose, CorrectiveDriver.outputEuler)
        CorrectiveDriver.attributeAffects(CorrectiveDriver.inputLocalRefPose, CorrectiveDriver.outputEuler)
        CorrectiveDriver.attributeAffects(CorrectiveDriver.inputDriverMode, CorrectiveDriver.outputEuler)
        CorrectiveDriver.attributeAffects(CorrectiveDriver.inputTwistVector, CorrectiveDriver.outputEuler)
        CorrectiveDriver.attributeAffects(CorrectiveDriver.inputScale, CorrectiveDriver.outputEuler)
        
    def compute(self, plug, dataBlock):
            
        # Fetch input local pose.   
        localPoseHandle = dataBlock.inputValue(CorrectiveDriver.inputLocalPose).asMatrix()   
        localPoseM = om.MTransformationMatrix(localPoseHandle)

        # Fetch input local ref pose.   
        localRefPoseHandle = dataBlock.inputValue(CorrectiveDriver.inputLocalRefPose).asMatrix()   
        localRefPoseM = om.MTransformationMatrix(localRefPoseHandle)

        # Fetch input driver mode.
        driverMode = dataBlock.inputValue(CorrectiveDriver.inputDriverMode).asShort()

        # Fetch input twist vector.
        twistVectorHandle = dataBlock.inputValue(CorrectiveDriver.inputTwistVector)   
        x, y, z = twistVectorHandle.asFloat3()
        twistVector = om.MVector(x, y, z)
                    
        # Fetch input scale.
        inputScale = dataBlock.inputValue(CorrectiveDriver.inputScale).asDouble()
      
        # Get euler from the driver.
        outEulerX, outEulerY, outEulerZ = euler_from_driver(localPoseM, localRefPoseM, driverMode, twistVector, inputScale)
        
        # Set euler output.
        eulerHandle = dataBlock.outputValue(CorrectiveDriver.outputEuler)
        eulerHandle.set3Double(outEulerX, outEulerY, outEulerZ)
        eulerHandle.setClean()
        
def initializePlugin(mobject):
    pluginFn = om.MFnPlugin(mobject)
    pluginFn.registerNode(
        "correctiveDriver",
        node_id,
        CorrectiveDriver.creator,
        CorrectiveDriver.initialize
    )
    
def uninitializePlugin(mobject):
    pluginFn = om.MFnPlugin(mobject)
    pluginFn.deregisterNode(node_id)