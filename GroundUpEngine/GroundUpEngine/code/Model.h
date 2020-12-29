#pragma once
#include "GameObject.h"
#include "../shader_m.h"

class Model :
    public GameObject
{
private:
    Shader shader;
    float vertices;
    unsigned int VBO, VAO;
};