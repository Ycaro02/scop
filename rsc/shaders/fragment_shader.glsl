#version 330 core

out vec4 FragColor;
in vec3 myColor;
in vec2 textureCoords;

uniform int activeTexture;
uniform sampler2D texture1;

void main()
{
    if (activeTexture == 1) {
	    vec3 texColor = texture(texture1, textureCoords).rgb;
        FragColor = vec4(texColor, 1.0);
        return ;
    }
    FragColor = vec4(myColor, 1.0);
}
