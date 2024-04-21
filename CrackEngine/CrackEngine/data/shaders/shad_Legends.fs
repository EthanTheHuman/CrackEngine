#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;
uniform int colorCount;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float celTolerance;
uniform float celShade;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
    vec4 celData = texture(Texture0, TexCoord).xyzw;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max((dot(norm, lightDir) + 1) / 2, 0.0);
    diff = min(diff, 1.0);
    if (diff <= celData.y / 4)
        diff = celData.w;
    else if (diff < celData.y)
        diff = celData.z;
    else
        diff = 1.0;

    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    FragColor.xyz = texture(Texture1, TexCoord).xyz * min(celData.x, diff);
    FragColor.w = texture(Texture1, TexCoord).w;
    FragColor = FragColor * vec4(ambient, 1.0);
}