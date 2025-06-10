from pyfbsdk import *
import os
import os.path

# Select files for import.
importFiles = FBFilePopup()
importFiles.Style = FBFilePopupStyle.kFBFilePopupOpen
importFiles.Caption = "Choose files"
importFiles.Filter = "*.fbx"
importFiles.Path = "C:\\"
if importFiles.Execute():
    print("Selected file:", importFiles.FullFilename)

app = FBApplication()
sys = FBSystem()

NO_LOAD_UI_DIALOG = False
APPEND = False
MERGE = True
OPTIONS_FOR_LOAD = True

nativeFile = os.path.abspath( os.path.join( sys.ApplicationPath, r"..\system\primitives\Cube.fbx"))
print("Source File Path", nativeFile)