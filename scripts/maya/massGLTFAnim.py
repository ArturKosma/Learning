import maya.cmds as cmds
import maya.mel as mel
import os
import interface

# Clear scene.
def ClearScene():
    cmds.select(all=True)
    cmds.delete()

# Select .fbx anims to be converted.
file_paths = cmds.fileDialog2(fileMode=4, caption="Select Files")

# Select export destination folder.
destination_folder = cmds.fileDialog2(fileMode=3, caption="Select Export Target")
if destination_folder:
    print(destination_folder)

# Loop through all selected files.    
if file_paths:
    for path in file_paths:
        filename = os.path.splitext(os.path.basename(path))[0]
        
        # Clear before import.  
        ClearScene()
        
        # Import file.
        cmds.file(path, i=True)

        # Create export path.
        export_path = os.path.join(destination_folder[0], filename + ".gltf")
        print(export_path)
        
        # Export GLTF.
        interface.exportGLTFPath(export_path)