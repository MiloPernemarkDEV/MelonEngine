@echo off
setlocal

set BUILD_DIR=build
set CONFIG=Debug

if "%2"=="rel" set CONFIG=Release

if "%1"=="build" goto build
if "%1"=="check" goto check
if "%1"=="run" goto run
if "%1"=="clean" goto clean
if "%1"=="shaders" goto shaders

echo Usage:
echo   melon build [rel]
echo   melon check [rel]
echo   melon run
echo   melon clean
echo   melon shaders
exit /b


:configure
cmake -S . -B %BUILD_DIR%
exit /b


:shaders
echo [Melon] Compiling shaders...

pushd Shaders

echo.
echo === GLSL SHADERS ===
for %%f in (GLSL\*.vert GLSL\*.frag GLSL\*.comp) do (
    echo Compiling %%f
    glslangValidator -g -V "%%f" -o "%%f.spv"
)

echo.
echo === HLSL SHADERS ===
for %%f in (HLSL\*.hlsl) do (
    echo Compiling %%f
    dxc -T cs_6_0 -E CSmain -spirv -Zi -Qembed_debug "%%f" -Fo "%%f.spv"
)

popd

echo [Melon] Shader compilation complete.
exit /b


:check
echo [Melon] Checking (%CONFIG%)...
call :shaders
call :configure
cmake --build %BUILD_DIR% --config %CONFIG%
exit /b


:build
echo [Melon] Building (%CONFIG%)...

call :shaders

call :configure
cmake --build %BUILD_DIR% --config %CONFIG%

echo.
echo [Melon] Running Sandbox (%CONFIG%)...
%BUILD_DIR%\Sandbox\%CONFIG%\Sandbox.exe
exit /b


:run
echo [Melon] Running Debug...
%BUILD_DIR%\Sandbox\Debug\Sandbox.exe
exit /b


:clean
echo [Melon] Cleaning build folder...
rmdir /s /q %BUILD_DIR%
exit /b