#pragma once
#include "./Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>
#include "Palette.h"

class ModelImport: public Model
{
public:
    ModelImport();
    ModelImport(std::string _filePath);
    ModelImport(std::string _filePath, std::string _textureFilePath);
    ModelImport(std::string _filePath, std::string _textureFilePath, std::string _colorFilePath);
    ModelImport(std::string _filePath, std::string _textureFilePath, std::string _newColorFileName, std::string _oldColorFileName);
    ~ModelImport();
    void init() override;
    void update() override;
    void render() override;
    void setPalette(std::string _newColorFileName, std::string _oldColorFileName);

    std::string filePath = "";
    const char* textureFilePath = "";
    int textureWidth = 0;
    int textureHeight = 0;
    const char* colorFilePath = "";
    int colorWidth = 0;
    int colorHeight = 0;
    GLuint EBO = 0;
    GLsizei vertexCount = 0;
    GLuint texture = 0;
    GLuint colorTexture = 0;
    Palette mainPalette = Palette();
};

