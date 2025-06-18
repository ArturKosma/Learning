@echo off

REM Cache paths.
set PROJ_PATH=C:\_programming\Learning\animFlex
set BUILD_PATH=C:\_programming\Learning\animFlex\cmake\build
set DEST_PATH=C:\_programming\Learning\docs
set RETE_PATH=C:\_programming\Learning\animFlex\graph
set SRC_PATH=C:\_programming\Learning\animFlex\source

REM Navigate to the build directory.
cd cmake\build

REM Run CMake.
call emcmake cmake .. 

REM Build the project.
call ninja -t clean
call ninja

REM Build the graph manifest.
cd "%PROJ_PATH%\"
call python build_manifest.py "%SRC_PATH%"

REM Copy graph manifest.
copy graphManifest.json "%DEST_PATH%\"

REM Copy build.
cd "%BUILD_PATH%\"

copy animFlex.wasm "%DEST_PATH%\"
copy animFlex.js "%DEST_PATH%\"
copy animFlex.html "%DEST_PATH%\"
copy animFlex.data "%DEST_PATH%\"

REM Paste build.
cd "%DEST_PATH%\"

REM Remove any previously generated assets by vite.
if exist "assets" (
    rmdir /s /q "assets"
)

del index.html
rename animFlex.html index.html

REM Build rete.
cd "%RETE_PATH%\"
call npx vite build

REM @echo Build completed!
pause
