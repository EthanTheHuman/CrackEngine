#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform vec3[] baseColors;
uniform vec4[] convertedColors;
uniform int colorCount;
uniform vec3 lightColor;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    FragColor = texture(Texture0, TexCoord);
    vec2 palMapCoord = vec2(0,0);
    for (int i = 0; i < 256; i++)
    {
        if (i < colorCount)
        {
            if (FragColor.xyz == texture(Texture1, vec2(0, (i/256.f))).xyz)
                {
                    FragColor.xyz = texture(Texture1, vec2(0.9, (i/256.f))).xyz;
                    FragColor.w *= texture(Texture1, vec2(0.9, (i/256.f))).w;
                    FragColor = FragColor * vec4(ambient, 1.0);
                    return;
                }
        }
    }
    FragColor = FragColor * vec4(ambient, 1.0);
}