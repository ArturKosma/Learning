import clr
import bpy 
from System.Windows.Forms import OpenFileDialog
clr.AddReference('System.Windows.Forms')

# Open file dialogue and allow selecting multiple items.

file_dialog = OpenFileDialog()
file_dialog.Multiselect = True
ret = file_dialog.ShowDialog()

# After selection, go through every file.

for file in file_dialog.FileNames:
    print(file)