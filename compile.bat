@Echo off

PUSHD Sandbox
CALL glslc -Werror -fshader-stage=vertex ./Resources/Shaders/vertex.glsl -o ./Binaries/Shaders/vertex.spv
CALL glslc -Werror -fshader-stage=fragment ./Resources/Shaders/fragment.glsl -o ./Binaries/Shaders/fragment.spv
POPD

CALL :COMPILE_PROJ JadeEngine
CALL :COMPILE_PROJ Sandbox

EXIT /B 0

:COMPILE_PROJ
IF %ERRORLEVEL% GEQ 1 EXIT /B 1
ECHO Compiling %~1...
PUSHD "%~1"
CALL make
ECHO.
POPD