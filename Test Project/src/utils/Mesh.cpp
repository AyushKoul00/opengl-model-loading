#include "pch.h"
#include "Mesh.h"

// constructor
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material mat)
    : vertices(vertices), indices(indices), mat(mat)
{
    vao.Bind();
    
    vbo = VBO((GLfloat*)vertices.data(), vertices.size() * sizeof(Vertex));
    ebo = EBO((GLuint*)indices.data(), indices.size() * sizeof(unsigned int));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    vao.LinkAttrib(vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    vao.LinkAttrib(vbo, 4, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    vao.LinkAttrib(vbo, 5, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    vao.LinkAttrib(vbo, 6, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));


    vao.LinkAttrib(vbo, 7, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    vao.Unbind();
}

// render the mesh
void Mesh::Draw(Shader& shader)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < mat.textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        //std::string number;
        std::string name = mat.textures[i].type;
        if (name == "texture_diffuse")
            shader.setInt("material.diffuseTexture", i);
        else if (name == "texture_specular")
            shader.setInt("material.specularTexture", i);
        else if (name == "texture_normal")
            shader.setInt("material.normalTexture", i);
        else if (name == "texture_height")
            shader.setInt("material.heightTexture", i);

        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, mat.textures[i].id);
    }



    shader.setVec3("material.ambientColor", mat.ambientColor);
    shader.setVec3("material.diffuseColor", mat.diffuseColor);
    shader.setVec3("material.specularColor", mat.specularColor);

    // draw mesh
    vao.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    vao.Unbind();

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}