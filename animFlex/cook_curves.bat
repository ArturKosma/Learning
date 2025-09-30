@echo off

REM Anims source path.
set "ANIMS_SOURCE=C:\Users\garon\Desktop\curvesSource"

REM Anims target path.
set "ANIMS_EXPORT=C:\Users\garon\Desktop\curvesOutput"

REM Go to packager exe.
cd ../animFlex_cooking

REM Generate.
REM -allowToLoop-value=1.22
REM -allowToPivot-value=0.63
REM -rootDistance
REM -rootSpeed
REM -rootYawAuthority-value=9999
REM -rootYaw
animFlex_cooking.exe animCurve %ANIMS_SOURCE% %ANIMS_EXPORT% -allowToLoop-value=1.02


pause