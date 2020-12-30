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

protected:
    const char* spriteFileName;
    unsigned int spriteImage;

private:
};