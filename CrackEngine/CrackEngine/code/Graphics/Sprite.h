#pragma once
#include "Model.h"
#include "../Config.h"
#include "Palette.h"

class Sprite :
    public Model
{
public:
    enum ScaleType {
        Scale = 0,
        Fill,
        Absolute
    };
    //constructor and deconstructor
    Sprite();
    Sprite(char const* _filename);
    ~Sprite();

    void init() override;
    void update() override;
    void render() override;
    int setImage(int textureRef);
    int setImage(int textureRef, int width, int height);
    int setImage(std::string texturePath);
    glm::vec3& setScale(glm::vec3 _scale);
    glm::vec3& setPosition(glm::vec3 _scale);
    glm::vec3 getPosition();
    static unsigned int pixelsPerUnit;
    unsigned int spriteImage;
    int spriteWidth, spriteHeight;
    ScaleType scaleType = ScaleType::Scale;

    glm::vec3 frameScale = glm::vec3(1,1,1);
    glm::vec3 framePos = glm::vec3(0, 0, 0);
    Palette mainPalette = Palette();

protected:
    glm::vec3 getLocalPos();
    glm::vec3 getLocalScale();
    glm::vec3 convertedScale(glm::vec3 _scale);
    glm::vec3 convertedPosition(glm::vec3 _pos);
    const char* spriteFileName;

private:
};