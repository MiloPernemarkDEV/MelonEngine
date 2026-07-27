::  melon build to in file changes
::  melon build -c to build + configuring to generate compile_commands.json for clangd to read
::  melon build -r to build in release
::  melon clean to remove the build directories
::  melon run to launch the executable located in build
::  melon run -r to launch the executable located in build_release

@echo off

set BUILD_DIR=build
set RELEASE_DIR=build_release

if "%1"=="build" goto build
if "%1"=="clean" goto clean
if "%1"=="run" goto run

echo Unknown command
melon build
melon build -c
melon clean
melon run

exit /b

:configure

echo [Melon] Configuring CMake...

cmake -B %BUILD_DIR% -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if %errorlevel% neq 0 exit /b %errorlevel%


echo [Melon] Updating clangd database...

copy /Y %BUILD_DIR%\compile_commands.json .

exit /b 0

:build

if "%2"=="-c" (
    call :configure
    if %errorlevel% neq 0 exit /b %errorlevel%
)

if "%2"=="-r" (
    echo building..
    cmake -B %RELEASE_DIR% -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cmake --build %RELEASE_DIR%
    exit /b

    exit /b
)

echo building..
cmake --build %BUILD_DIR%

exit /b

:clean
echo Cleaning..
if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
if exist %RELEASE_DIR% rmdir /s /q %RELEASE_DIR%

exit /b

:run
echo Running app..

if "%2"=="-r" (
    %RELEASE_DIR%\Sandbox\Sandbox.exe
    exit /b
)

%BUILD_DIR%\Sandbox\Sandbox.exe
exit /b
