#pragma once
#include "GameObject.h"
#include "../shader_m.h"
#include "../stb_image.h"

class Model :
    public GameObject
{
public:
    Model();
    ~Model();

    void init() override;
    void update() override;
    void render() override;
    Shader getShader();
    static glm::vec3 cameraPos;
    void setShader(Shader& _shader);
    float* vertices;

protected:
    Shader shader;
    unsigned int VBO, VAO;
    unsigned int loadTexture(char const* path);
};