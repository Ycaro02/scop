#version 330 core

out vec4 FragColor;
in vec3 myColor;
in vec2 textureCoords;

uniform sampler2D texture1;

void main()
{
	vec3 texColor = texture(texture1, textureCoords).rgb;
    FragColor = vec4(texColor, 1.0);
}
