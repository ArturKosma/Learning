@echo off

REM Anims source path.
set "ANIMS_SOURCE=%~dp0%artassets\anims"

REM Anims target path.
set "ANIMS_EXPORT=%~dp0%export\anims"

REM Go to packager exe.
cd ../animFlex_cooking

REM Order packaging per type.
animFlex_cooking.exe anim %ANIMS_SOURCE% %ANIMS_EXPORT%

pause