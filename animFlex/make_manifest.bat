@echo off
setlocal EnableExtensions EnableDelayedExpansion

if not "%~1"=="" (
  pushd "%~1" || (echo Could not open "%~1" & exit /b 1)
)

set "OUT=manifest.json"
> "%OUT%" echo [

set "first=1"
for /f "delims=" %%F in ('dir /b /a-d *.json ^| sort') do (
  set "full=%%~nxF"
  set "name=%%~nF"
  if /I not "!full!"=="%OUT%" (
    if "!first!"=="1" (
      >> "%OUT%" echo   {^"name^": ^"!name!^"}
      set "first=0"
    ) else (
      >> "%OUT%" echo , {^"name^": ^"!name!^"}
    )
  )
)

>> "%OUT%" echo ]

if not "%~1"=="" popd
echo Wrote "%OUT%"
