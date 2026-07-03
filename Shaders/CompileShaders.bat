@echo off
setlocal enabledelayedexpansion

echo ========================================
echo [Melon] Compiling GLSL shaders...
echo ========================================
for %%f in (GLSL\*.vert GLSL\*.frag GLSL\*.comp) do (
    if exist "%%f" (
        echo Compiling %%f
        glslangValidator -V "%%f" -o "%%f.spv"
    )
)

echo.
echo ========================================
echo [Melon] Compiling HLSL shaders...
echo ========================================
for %%f in (HLSL\*.hlsl) do (
    if exist "%%f" (
        echo Compiling %%f
        dxc -T cs_6_0 -E CSmain -spirv "%%f" -Fo "%%f.spv"
    )
)

echo.
echo [Melon] Shader processing done.
pause