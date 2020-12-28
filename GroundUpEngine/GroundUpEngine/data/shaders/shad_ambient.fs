#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;
uniform vec3 lightColor;

uniform sampler2D ourTexture;

void main()
{
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    FragColor = texture(ourTexture, TexCoord) * vec4(ambient, 1.0);
}