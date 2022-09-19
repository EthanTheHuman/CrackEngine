#include "Sprite.h"

Sprite::Sprite()
{
}

unsigned int Sprite::pixelsPerUnit = Config::pixelsPerUnit;

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
	static Shader newShader("data/shaders/shad_palette.vs", "data/shaders/shad_palette.fs");
	setShader(newShader);

    float vertices2[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
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
    //spriteWidth = 2;
    //spriteHeight = 256;
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

    for (int i = 0; i < mainPalette.baseColors.size(); i++) {
        GLint baseColorsLoc = glGetUniformLocation(shader.ID, ("baseColors["+i+(std::string)"]").c_str());
        glUniform3f(baseColorsLoc, mainPalette.baseColors[i].x, mainPalette.baseColors[i].y, mainPalette.baseColors[i].z);
        //shader.setVec3("baseColors[" + i + (std::string)"]", mainPalette.baseColors[i]);
    }
    for (int i = 0; i != mainPalette.convertedColors.size(); i++) {
        GLint convertedColorsLoc = glGetUniformLocation(shader.ID, ("convertedColors[" + i + (std::string)"]").c_str());
        glUniform4f(convertedColorsLoc, mainPalette.convertedColors[i].x, mainPalette.convertedColors[i].y, mainPalette.convertedColors[i].z, mainPalette.convertedColors[i].w);
    }
    shader.setInt("colorCount", mainPalette.paletteCount);

    // world transformation
    glm::mat4 model(1.0f);
    model = glm::translate(model, convertedPosition(glm::round(position) + getLocalPos()));
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, convertedScale(getLocalScale()));
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    //spriteImage = mainPalette.paletteTexIndex;
    //spriteWidth = 2;
    //spriteHeight = 256;

    glBindTexture(GL_TEXTURE_2D, spriteImage);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mainPalette.paletteTexIndex);

    //--------------
    auto texLoc = glGetUniformLocation(shader.ID, "Texture0");
    glUniform1i(texLoc, 0);

    texLoc = glGetUniformLocation(shader.ID, "Texture1");
    glUniform1i(texLoc, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteImage);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mainPalette.paletteTexIndex);
    
    //--------------------------------

    // render the cube
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

int Sprite::setImage(int textureRef)
{
    spriteImage = textureRef;
    return 0;
}

int Sprite::setImage(int textureRef, int width, int height)
{
    spriteImage = textureRef;
    spriteWidth = width;
    spriteHeight = height;
    setScale(scale);
    return 0;
}

int Sprite::setImage(std::string texturePath)
{
    return 0;
}

glm::vec3& Sprite::setScale(glm::vec3 _scale)
{
    scale = _scale;
    return scale;
}

glm::vec3& Sprite::setPosition(glm::vec3 _pos)
{
    position = _pos;
    return position;
}

glm::vec3 Sprite::getPosition()
{
    return position;
}

glm::vec3 Sprite::convertedScale(glm::vec3 _scale)
{
    glm::vec3 newScale;
    switch (scaleType)
    {
    case ScaleType::Fill:
        newScale.x = (float)(_scale.x) / (float)pixelsPerUnit;
        newScale.y = (float)(_scale.y) / (float)pixelsPerUnit;
        newScale.z = (float)_scale.z;
        break;
    case ScaleType::Scale:
        newScale.x = (float)(_scale.x * spriteWidth) / (float)pixelsPerUnit;
        newScale.y = (float)(_scale.y * spriteHeight) / (float)pixelsPerUnit;
        newScale.z = (float)_scale.z;
        break;
    case ScaleType::Absolute:
        newScale.x = (float)(_scale.x);
        newScale.y = (float)(_scale.y);
        newScale.z = (float)_scale.z;
    }
    return newScale;
}

glm::vec3 Sprite::convertedPosition(glm::vec3 _pos)
{
    glm::vec3 newPos;
    newPos.x = ((float)_pos.x + ((spriteWidth * scale.x) / 2.f)) / (float)pixelsPerUnit;
    newPos.y = ((float)_pos.y + ((spriteHeight * scale.y) / 2.f)) / (float)pixelsPerUnit;
    newPos.z = (float)_pos.z / (float)pixelsPerUnit;
    return newPos;
}

glm::vec3 Sprite::getLocalPos()
{
    glm::vec3 res(framePos.x * scale.x, framePos.y * scale.y, framePos.z * scale.z);
    return res;
}

glm::vec3 Sprite::getLocalScale()
{
    return glm::vec3(scale.x * frameScale.x, scale.y * frameScale.y, scale.z * frameScale.z);
}
