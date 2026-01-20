@echo off

rem IS RUNING FROM WORKSPACE DIR
if not exist "%cd%/Tests/Build/assets/shaders/" mkdir "%cd%/Tests/Build/assets/shaders"

echo "Compiling shaders..."

rem Compile VERT shaders
echo "Tests/Assets/shaders/Builtin.ObjectShader.vert.glsl -> Tests/Build/assets/shaders/Builtin.ObjectShader.vert.spv"
"%VULKAN_SDK%/bin/glslc.exe" -fshader-stage=vert "%cd%/Tests/Assets/shaders/Builtin.ObjectShader.vert.glsl" -o "%cd%/Tests/Build/assets/shaders/Builtin.ObjectShader.vert.spv"
if %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

rem Compile FRAG shaders
echo "Tests/Assets/shaders/Builtin.ObjectShader.frag.glsl -> Tests/Build/assets/shaders/Builtin.ObjectShader.frag.spv"
"%VULKAN_SDK%/bin/glslc.exe" -fshader-stage=frag "%cd%/Tests/Assets/shaders/Builtin.ObjectShader.frag.glsl" -o "%cd%/Tests/Build/assets/shaders/Builtin.ObjectShader.frag.spv"
if %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

echo "Copying assets..."
echo xcopy "Tests/Assets/shaders" "Tests/Build/assets/shaders" /h /i /c /k /e /r /y
xcopy "Tests/Assets/shaders" "Tests/Build/assets/shaders" /h /i /c /k /e /r /y

echo "Done."
