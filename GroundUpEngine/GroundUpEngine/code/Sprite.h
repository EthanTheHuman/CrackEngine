#pragma once
#include "Model.h"
class Sprite :
    public Model
{
public:
    //constructor and deconstructor
    Sprite();
    Sprite(char const* _filename);
    ~Sprite();

    void init() override;
    void update() override;
    void render() override;
    glm::vec3& setScale(glm::vec3 _scale);
    glm::vec3& setPosition(glm::vec3 _scale);
    glm::vec3 getPosition();
    static unsigned int pixelsPerUnit;

protected:
    const char* spriteFileName;
    unsigned int spriteImage;
    int spriteWidth, spriteHeight;

private:
};