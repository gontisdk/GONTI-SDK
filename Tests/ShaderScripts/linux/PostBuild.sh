mkdir -p Tests/Build/assets
mkdir -p Tests/Build/assets/shaders

echo "Compiling shaders..."

rem Compile VERT shaders
echo "Tests/Assets/shaders/Builtin.ObjectShader.vert.glsl -> Tests/Build/assets/shaders/Builtin.ObjectShader.vert.spv"
%VULKAN_SDK%/bin/glslc.exe -fshader-state=vert Tests/Assets/shaders/Builtin.ObjectShader.vert.glsl -o Tests/Build/assets/shaders/Builtin.ObjectShader.vert.spv
ERRORLEVEL=$?
if [$ERRORLEVEL -ne 0]
then 
echo "Error: "$ERRORLEVEL && exit
fi

rem Compile FRAG shaders
echo "Tests/Assets/shaders/Builtin.ObjectShader.frag.glsl -> Tests/Build/assets/shaders/Builtin.ObjectShader.frag.spv"
%VULKAN_SDK%/bin/glslc.exe -fshader-state=frag Tests/Assets/shaders/Builtin.ObjectShader.frag.glsl -o Tests/Build/assets/shaders/Builtin.ObjectShader.frag.spv
ERRORLEVEL=$?
if [$ERRORLEVEL -ne 0]
then 
echo "Error: "$ERRORLEVEL && exit
fi

echo "Copying assets..."
echo cp -R "Tests/Assets/shaders" "Tests/Build/assets/shaders"
cp -R "Tests/Assets/shaders" "Tests/Build/assets/shaders"

echo "Done."