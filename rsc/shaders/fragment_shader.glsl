#version 330 core

out vec4 FragColor;
in vec3 myColor;
in vec2 textureCoords;

uniform sampler2D texture1;
uniform float textureIntensity;

void main()
{
	float colorIntensity = 1 - textureIntensity;;
	vec4 texColor = texture(texture1, textureCoords);
	FragColor = (vec4(myColor, 1.0) * colorIntensity) + (texColor * textureIntensity);
}
