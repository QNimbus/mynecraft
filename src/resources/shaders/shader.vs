#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float scale = 1.0f;
uniform bool flipX = false;
uniform bool flipY = false;

void main()
{
   gl_Position = vec4(aPosition.x * scale * (flipX ? -1 : 1), aPosition.y * scale * (flipY ? -1 : 1), aPosition.z * scale, 1.0);
   color = aColor;
};