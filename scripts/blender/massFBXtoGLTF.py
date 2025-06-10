import clr
print("CLR path:", getattr(clr, "__file__", "virtual module"))
print("CLR dir:", dir(clr))  # Should include AddReference

import bpy 
clr.AddReference('System.Windows.Forms')
clr.AddReference("System")
from System.Windows.Forms import OpenFileDialog
from System.Windows.Forms import FolderBrowserDialog
import System

# Cleaning the scene.
def full_cleanup():
    # Delete all objects from scene.
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete()

    # Manually remove leftover data blocks.
    data_blocks = [
        bpy.data.meshes,
        bpy.data.armatures,
        bpy.data.materials,
        bpy.data.actions,
        bpy.data.images,
        bpy.data.curves,
        bpy.data.textures,
        bpy.data.cameras,
        bpy.data.lights,
        bpy.data.collections,
    ]
    for block in data_blocks:
        for item in block:
            block.remove(item, do_unlink=True)

# Clear up the scene.
bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete()

# Open file dialogue and allow selecting multiple items.
file_dialog = OpenFileDialog()
file_dialog.Multiselect = True
fileDialogResult = file_dialog.ShowDialog()

# Open folder dialogue and specify target export folder.
folder_dialog = OpenFileDialog()
folder_dialog.CheckFileExists = False
folder_dialog.FileName = "Select Folder"
folder_dialog.ValidateNames = False
folderDialogResult = folder_dialog.ShowDialog()
selected_folder = System.IO.Path.GetDirectoryName(folder_dialog.FileName)

# After selection, go through every file.
for file in file_dialog.FileNames:
    
    bpy.ops.import_scene.fbx(filepath=file) # Import FBX.
    
    #print(file)
    file_base_name = System.IO.Path.GetFileNameWithoutExtension(file)
    output_path = System.IO.Path.Combine(selected_folder, file_base_name + ".gltf")
    #print(output_path)
    
    bpy.ops.export_scene.gltf(filepath=output_path) # Export GLTF.
    
    # Cleanup for next import.
    full_cleanup()