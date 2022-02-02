#pragma once
#include <glad/glad.h>
#include "pch.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];

    // Color
    glm::vec3 Color;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


struct Material
{
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    std::vector<Texture> textures;
};


class Mesh {
public:
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    //std::vector<Texture>      textures;
    Material mat;
    VAO vao;

    // constructor
    //Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material mat);

    // render the mesh
    void Draw(Shader& shader);

private:
    // render data
    VBO vbo;
    EBO ebo;
};