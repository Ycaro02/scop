#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
out vec3 myColor;
out vec2 textureCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    myColor = aColor;
	textureCoords = aTextureCoords;
}
