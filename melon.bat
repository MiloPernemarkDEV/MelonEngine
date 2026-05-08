@echo off
setlocal

set BUILD_DIR=build
set CONFIG=Debug

if "%2"=="rel" set CONFIG=Release

if "%1"=="build" goto build
if "%1"=="check" goto check
if "%1"=="run" goto run
if "%1"=="clean" goto clean

echo Usage:
echo   melon build [rel]
echo   melon check [rel]
echo   melon run
echo   melon clean
exit /b

:configure
cmake -S . -B %BUILD_DIR%
exit /b

:check
echo [Melon] Checking (%CONFIG%)...
call :configure
cmake --build %BUILD_DIR% --config %CONFIG%
exit /b

:build
echo [Melon] Building (%CONFIG%)...
call :configure
cmake --build %BUILD_DIR% --config %CONFIG%

echo [Melon] Running Sandbox (%CONFIG%)...
%BUILD_DIR%\Sandbox\%CONFIG%\Sandbox.exe
exit /b

:run
echo [Melon] Running Debug...
%BUILD_DIR%\Sandbox\Debug\Sandbox.exe
exit /b

:clean
echo [Melon] Cleaning...
rmdir /s /q %BUILD_DIR%
exit /b