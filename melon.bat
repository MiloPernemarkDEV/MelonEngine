@echo off
setlocal enabledelayedexpansion

set BUILD_DIR=build
set CONFIG=Debug

if "%2"=="rel" set CONFIG=Release

if "%1"=="build" goto build
if "%1"=="check" goto check
if "%1"=="run"   goto run
if "%1"=="clean" goto clean
if "%1"=="shaders" goto shaders

:usage
echo Usage:
echo   melon build [rel]
echo   melon check [rel]
echo   melon run [rel]
echo   melon shaders
echo   melon clean
exit /b 0


:configure
if exist %BUILD_DIR%\CMakeCache.txt (
    echo [Melon] CMake already configured. Skipping...
    exit /b 0
)

echo [Melon] Configuring project...
cmake -S . -B %BUILD_DIR%
if %errorlevel% neq 0 exit /b %errorlevel%
exit /b 0


:shaders
echo [Melon] Compiling shaders...

if not exist Shaders (
    echo [Melon] Error: Shaders folder not found
    exit /b 1
)

pushd Shaders

for /r %%f in (*.vert *.frag *.comp *.hlsl) do (
    echo Compiling %%f

    if "%%~xf"==".hlsl" (
        dxc -T cs_6_0 -E main -spirv -Zi "%%f" -Fo "%%f.spv"
    ) else (
        glslangValidator -V "%%f" -o "%%f.spv"
    )

    if !errorlevel! neq 0 (
        popd
        exit /b !errorlevel!
    )
)

popd
echo [Melon] Shaders compiled.
exit /b 0


:check
call :shaders
if %errorlevel% neq 0 exit /b %errorlevel%

call :configure
if %errorlevel% neq 0 exit /b %errorlevel%

echo [Melon] Building (%CONFIG%)...
cmake --build %BUILD_DIR% --config %CONFIG%
if %errorlevel% neq 0 exit /b %errorlevel%

echo [Melon] Build OK.
exit /b 0


:build
call :check
if %errorlevel% neq 0 exit /b %errorlevel%

goto run


:run
echo [Melon] Running Sandbox (%CONFIG%)...

set EXE=%BUILD_DIR%\Sandbox\%CONFIG%\Sandbox.exe

if exist "%EXE%" (
    "%EXE%"
) else (
    echo [Melon] Error: Sandbox.exe not found:
    echo %EXE%
    exit /b 1
)

exit /b 0


:clean
echo [Melon] Cleaning build...
if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
echo [Melon] Clean complete.
exit /b 0