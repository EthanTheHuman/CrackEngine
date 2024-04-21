#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../shader_m.h"
#include "ModelImport.h"

// Define your Vertex struct to hold position, normals, and texture coordinates.
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

// Define a function to load a 3D model using Assimp and transfer its data to OpenGL.
bool LoadModel(const std::string& filePath, GLuint& VAO, GLuint& VBO, GLuint& EBO, GLsizei& vertexCount) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return false;
    }

    // Extract vertex data from the Assimp mesh
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (unsigned int h = 0; h < scene->mNumMeshes; h++)
    {
        aiMesh* mesh = scene->mMeshes[h];

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            // Extract position, normal, and texture coordinates
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if (mesh->mTextureCoords[0]) { // Check if the model has texture coordinates
                vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            else {
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }

        // Extract indices for the mesh
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }
    }

    // Create and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Set the vertex count for rendering
    vertexCount = indices.size();

    return true;
}

// Define a rendering function
void RenderModel(GLuint VAO, GLsizei vertexCount) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

ModelImport::ModelImport(std::string _filePath)
{
    filePath = _filePath;
    textureFilePath = "./data/images/container2.png";
    init();
}

ModelImport::ModelImport(std::string _filePath, std::string _textureFilePath)
{
    filePath = _filePath;
    textureFilePath = _textureFilePath.c_str();
    init();
}

ModelImport::ModelImport(std::string _filePath, std::string _textureFilePath, std::string _colorFilePath)
{
    filePath = _filePath;
    textureFilePath = _textureFilePath.c_str();
    colorFilePath = _colorFilePath.c_str();
    init();
}

ModelImport::ModelImport(std::string _filePath, std::string _textureFilePath, std::string _newColorFileName, std::string _oldColorFileName)
{
    filePath = _filePath;
    textureFilePath = _textureFilePath.c_str();
    setPalette(_newColorFileName, _oldColorFileName);
    init();
}

ModelImport::~ModelImport()
{
}

void ModelImport::init()
{
    static Shader newShader("data/shaders/shad_Legends.vs", "data/shaders/shad_Legends.fs");
    setShader(newShader);
    LoadModel(filePath, VAO, VBO, EBO, vertexCount);
    texture = loadTexture(textureFilePath, &textureWidth, &textureHeight);
    colorTexture = loadTexture(colorFilePath, &colorWidth, &colorHeight);
}

void ModelImport::update()
{
}

void ModelImport::render()
{
    shader.use();
    shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("viewPos", cameraPos);

    // light properties
    static float sinus = 0.0f;
    shader.setVec3("lightPos", cos(sinus += 0.05), 0.0f, 2 + sin(sinus));
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
    shader.setFloat("celTolerance", 0.5f);
    shader.setFloat("celShade", 0.86f);

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
    glBindTexture(GL_TEXTURE_2D, texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colorTexture);

    //--------------
    auto texLoc = glGetUniformLocation(shader.ID, "Texture0");
    glUniform1i(texLoc, 0);

    texLoc = glGetUniformLocation(shader.ID, "Texture1");
    glUniform1i(texLoc, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colorTexture);

    //--------------------------------

    // render the cube
    RenderModel(VAO, vertexCount);
}

void ModelImport::setPalette(std::string _newColorFileName, std::string _oldColorFileName)
{
    mainPalette = Palette(_newColorFileName, _oldColorFileName);
}
