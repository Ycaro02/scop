#version 330 core

out vec4 FragColor;
in vec3 myColor;
in vec2 textureCoords;

uniform int activeTexture;
uniform sampler2D texture1;

void main()
{
    if (activeTexture == 1) {
	    vec4 texColor = texture(texture1, textureCoords);
        FragColor = texColor;
        return ;
    }
    FragColor = vec4(myColor, 1.0);
}
