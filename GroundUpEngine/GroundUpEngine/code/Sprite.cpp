#include "Sprite.h"

Sprite::Sprite()
{
}

unsigned int Sprite::pixelsPerUnit = 400;

Sprite::Sprite(char const* _filename)
{
    spriteFileName = _filename;
    init();
}

Sprite::~Sprite()
{
    delete[] vertices;
}

void Sprite::init()
{
	Shader newShader("data/shaders/shad_ambient.vs", "data/shaders/shad_ambient.fs");
	setShader(newShader);

    float vertices2[] = {
        // positions          // normals           // texture coords
        -0.0f, -0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
         1.0f, -0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        -0.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -0.0f, -0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    };

    // first, configure the cube's VAO (and VBO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glEnableVertexAttribArray(0);

    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    spriteImage = loadTexture(spriteFileName, &spriteWidth, &spriteHeight);
    std::cout << "Width: " << spriteWidth << std::endl;
    std::cout << "Height: " << spriteHeight << std::endl;
    setScale(glm::vec3(1, 1, 1));

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
}

void Sprite::update()
{
}

void Sprite::render()
{
    shader.use();
    shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("viewPos", cameraPos);

    // light properties
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("light.constant", 1.0f);
    shader.setFloat("light.linear", 0.09f);
    shader.setFloat("light.quadratic", 0.032f);

    // material properties
    shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 64.0f);

    // world transformation
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteImage);

    // render the cube
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

glm::vec3& Sprite::setScale(glm::vec3 _scale)
{
    scale.x = (float)(_scale.x * spriteWidth) / (float)pixelsPerUnit;
    scale.y = (float)(_scale.y * spriteHeight) / (float)pixelsPerUnit;
    scale.z = (float)_scale.z;
    return scale;
}

glm::vec3& Sprite::setPosition(glm::vec3 _pos)
{
    position.x = (float)_pos.x / (float)pixelsPerUnit;
    position.y = (float)_pos.y / (float)pixelsPerUnit;
    position.z = (float)_pos.z / (float)pixelsPerUnit;
    return position;
}
