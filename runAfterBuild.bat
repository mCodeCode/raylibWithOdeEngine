@ECHO OFF

move /Y build_FOLDER\sphere.exe .\

@RD /S /Q build_FOLDER 

start "%~dp0" "sphere.exe"


