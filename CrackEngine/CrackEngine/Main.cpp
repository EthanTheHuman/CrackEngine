#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "includes/json.hpp"
#include "code/Engine/CrackEngine.h"

// for convenience
using json = nlohmann::json;

unsigned int loadTexture(const char* path);

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    Crack::CrackEngine::Instance()->Init();

    while (true)
    {
        Crack::CrackEngine::Instance()->Step();
        Crack::CrackEngine::Instance()->Render();
    }

    glfwTerminate();
    return 0;
}