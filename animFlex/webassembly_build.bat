@echo off

REM Navigate to the build directory.
cd cmake\build

REM Run CMake.
call emcmake cmake .. 

REM Build the project.
call ninja -t clean
call ninja

REM Return to the project root.
cd ..

REM @echo Build completed!
pause
