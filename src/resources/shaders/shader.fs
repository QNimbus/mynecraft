#version 330 core

out vec4 fragColor;

in vec3 color;
in vec2 texCoord;

uniform float scale = 1.0f;
uniform sampler2D tex0;

void main()
{
   // fragColor = vec4(color.r * scale,color.g * scale,color.b * scale, 1.0f);
   fragColor = texture(tex0, texCoord);
};