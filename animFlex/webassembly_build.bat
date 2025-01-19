REM Path to Emscripten SDK.
REM set EMSDK_PATH=C:\_programming\emscripten\emsdk

REM Activate Emscripten environment.
REM call %EMSDK_PATH%\emsdk_env.bat

REM Navigate to the build directory.
cd cmake\build

REM Run CMake.
call emcmake cmake ..

REM Build the project.
call ninja -t clean
call ninja

REM Return to the project root.
cd ..

REM Test host.
REM call emrun build\animFlex.html

REM @echo Build completed!
pause
