#pragma once


#include <glad/glad.h> // holds all OpenGL type declarations
#include "shaderprintf.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "glslprogram.h"
#include <string>
#include <vector>
#include "input.h"
//#include <Windows.h>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
//    glm::vec3 Tangent;

};

struct Texture {
    unsigned int id;
    string type;    //texture_diffuse, texture_specular,texture_normal,texture_height
    string path;
};

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    glm::vec3    color;
    GLfloat shininess = 32.0;
    unsigned int VAO;
    unsigned int VBO, EBO;
    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(GLSLProgram& shader)
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        shader.setUniform("material.shininess", shininess);
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string textureName = textures[i].type;
            const string materialPrefix = "material.";
            if (textureName == "diffuse"){
                number = std::to_string(diffuseNr++);
                shader.setUniform((materialPrefix + "diffuse_exist").c_str(), true);
            }
            else if (textureName == "specular") {
                number = std::to_string(specularNr++); // transfer unsigned int to string
                shader.setUniform((materialPrefix + "specular_exist").c_str(), true);
            } 
            else if (textureName == "normal") {
                number = std::to_string(normalNr++); // transfer unsigned int to string
                shader.setUniform((materialPrefix + "normal_exist").c_str(), true);
            }
            else if (textureName == "height") {
                number = std::to_string(heightNr++); // transfer unsigned int to string
            }
                
            
            // set the sampler to the correct opengl texture unit
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
            shader.setUniform((materialPrefix + textureName).c_str(), i);
            // and finally bind the texture
            /*glBindTexture(GL_TEXTURE_2D, textures[i].id);*/
        }
        if (textures.size() == 0) {
            shader.setUniform("material.diffuse_exist", false);
            shader.setUniform("material.color", this->color);
        }
        
        // create a buffer to hold the printf results
        //GLuint printBuffer = createPrintBuffer();
        // bind it to the current program
        //bindPrintBuffer(shader.getHandle(), printBuffer);
        //shader.setUniform("mouse", glm::vec2(Input::Instance().GetMousePosition().first, Input::Instance().GetMousePosition().second));
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        //cout << "output from shader:" << getPrintBufferString(printBuffer) << endl;
        //deletePrintBuffer(printBuffer);
        //set everything back to defaults 
        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void Draw_depth(GLSLProgram& shader) {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    // render data 
    

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        //// vertex tangent
        //glEnableVertexAttribArray(3);
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        //// vertex bitangent
        //glEnableVertexAttribArray(4);
        //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        //// ids
        //glEnableVertexAttribArray(5);
        //glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        //// weights
        //glEnableVertexAttribArray(6);
        //glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        glBindVertexArray(0);
    }
};
