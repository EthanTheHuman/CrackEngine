#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 pos;   // screen position <-1,+1>

uniform vec4 quadDimensions;
uniform float gameWidth;
uniform float gameHeight;

void main()
{
    pos=aPos;
    gl_Position = vec4(aPos.x * gameWidth, aPos.y * gameHeight, 0.0, 1.0);
    TexCoords = aTexCoords;
}  