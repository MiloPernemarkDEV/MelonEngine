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
echo [Melon] Configuring CMake...
cmake -S . -B %BUILD_DIR%
if %errorlevel% neq 0 exit /b %errorlevel%
exit /b


:shaders
echo [Melon] Compiling shaders...

pushd Shaders

echo.

for %%f in (GLSL\*.vert GLSL\*.frag GLSL\*.comp) do (
    echo Compiling %%f
    glslangValidator -g -V "%%f" -o "%%f.spv"
    if %errorlevel% neq 0 (
        popd
        exit /b %errorlevel%
    )
)

echo.

for %%f in (HLSL\*.hlsl) do (
    echo Compiling %%f
    dxc -T cs_6_0 -E CSmain -spirv -Zi -Qembed_debug "%%f" -Fo "%%f.spv"
    if %errorlevel% neq 0 (
        popd
        exit /b %errorlevel%
    )
)

popd

echo [Melon] Shader compilation complete.
exit /b


:check
echo [Melon] Checking (%CONFIG%)...

call :shaders
if %errorlevel% neq 0 exit /b %errorlevel%

call :configure
if %errorlevel% neq 0 exit /b %errorlevel%

cmake --build %BUILD_DIR% --config %CONFIG%
if %errorlevel% neq 0 exit /b %errorlevel%

echo [Melon] Build OK.
exit /b


:build
echo [Melon] Building (%CONFIG%)...

call :shaders
if %errorlevel% neq 0 exit /b %errorlevel%

call :configure
if %errorlevel% neq 0 exit /b %errorlevel%

cmake --build %BUILD_DIR% --config %CONFIG%
if %errorlevel% neq 0 exit /b %errorlevel%

echo.
echo [Melon] Running Sandbox (%CONFIG%)...
%BUILD_DIR%\Sandbox\%CONFIG%\Sandbox.exe

exit /b


:run
echo [Melon] Running (%CONFIG%)...
%BUILD_DIR%\Sandbox\Debug\Sandbox.exe
exit /b


:clean
echo [Melon] Cleaning build folder...
rmdir /s /q %BUILD_DIR%
echo [Melon] Clean complete.
exit /b