@echo off

echo =========================
echo Compiling GLSL shaders...
echo =========================

for %%f in (GLSL\*.vert GLSL\*.frag GLSL\*.comp) do (
    echo Compiling %%f
    glslangValidator -V "%%f" -o "%%f.spv"
)

echo.
echo =========================
echo Compiling HLSL shaders...
echo =========================

for %%f in (HLSL\*.hlsl) do (
    echo Compiling %%f
    dxc -T cs_6_0 -E CSmain -spirv "%%f" -Fo "%%f.spv"
)

echo.
echo Done.
pause