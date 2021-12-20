#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

// Output color and texture coords for fragment shader
out vec3 color;
out vec2 texCoord;

uniform float scale = 1.0f;
uniform bool flipX = false;
uniform bool flipY = false;

void main()
{
   gl_Position = vec4(aPosition.x * scale * (flipX ? -1 : 1), aPosition.y * scale * (flipY ? -1 : 1), aPosition.z * scale, 1.0);
   color = aColor;
   texCoord = aTexture;
};