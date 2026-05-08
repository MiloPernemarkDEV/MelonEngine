@echo off
setlocal

set BUILD_DIR=build

if "%1"=="build" goto build
if "%1"=="check" goto check
if "%1"=="clean" goto clean
if "%1"=="run"   goto run


echo Usage:
echo   melon build   - build + run
echo   melon check   - just compile
echo   melon clean   - delete build folder
echo   melon run     - run Sandbox
exit /b

:check
echo [Melon] Configuring project...
cmake -S . -B %BUILD_DIR%
echo [Melon] Building...
cmake --build %BUILD_DIR%
exit /b

:build
echo [Melon] Configuring project...
cmake -S . -B %BUILD_DIR%
echo [Melon] Building...
cmake --build %BUILD_DIR%

echo [Melon] Running Sandbox...
%BUILD_DIR%\Sandbox\Debug\Sandbox.exe
exit /b

:clean
echo [Melon] Cleaning build folder...
rmdir /s /q %BUILD_DIR%
exit /b

:run
echo [Melon] Running Sandbox...
%BUILD_DIR%\Sandbox\Debug\Sandbox.exe
exit /b