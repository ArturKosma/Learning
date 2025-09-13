@echo off

REM Anims source path.
set "ANIMS_SOURCE=C:\Users\garon\Desktop\curvesSource"

REM Anims target path.
set "ANIMS_EXPORT=C:\Users\garon\Desktop\curvesOutput"

REM Go to packager exe.
cd ../animFlex_cooking

REM Order packaging per type.
animFlex_cooking.exe animCurve %ANIMS_SOURCE% %ANIMS_EXPORT% -feetGroundedL-valueMin=0.0-valueMax=0.18

pause