@echo off

REM Anims source path.
set "ANIMS_SOURCE=C:\Users\garon\Desktop\motionMatchingGLTF"

REM Anims target path.
set "ANIMS_EXPORT=C:\_programming\Assets\anims"

REM Go to packager exe.
cd ../animFlex_cooking

REM Order packaging per type.
animFlex_cooking.exe anim %ANIMS_SOURCE% %ANIMS_EXPORT%

REM Once packaged, deploy to assets.
cd %ANIMS_EXPORT%
git add .
git commit -m "assets auto deploy"
git push

pause