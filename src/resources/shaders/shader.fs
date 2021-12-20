#version 330 core

out vec4 fragColor;

in vec3 color;

uniform float scale = 1.0f;

void main()
{
   fragColor = vec4(color.r * scale,color.g * scale,color.b * scale, 1.0f);
};