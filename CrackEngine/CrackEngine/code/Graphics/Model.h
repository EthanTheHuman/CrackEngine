#pragma once
#include "../GameObject.h"
#include "../../shader_m.h"
#include "../../stb_image.h"
#include <libpng/png.h>

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
    static glm::mat4 projection;
    static glm::mat4 view;
    void setShader(Shader& _shader);
    float* vertices;

protected:
    Shader shader;
    unsigned int VBO, VAO;
    unsigned int loadTexture(char const* path);
    unsigned int loadTexture(char const* path, int* _width, int* _height);
};