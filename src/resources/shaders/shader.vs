#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

// Output color and texture coords for fragment shader
out vec3 color;
out vec2 texCoord;

uniform mat4 camMatrix;

void main()
{
   gl_Position = camMatrix * vec4(aPosition, 1.0f);

   color = aColor;
   texCoord = aTexture;
};