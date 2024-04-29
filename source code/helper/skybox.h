#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "stb/stb_image.h"
#include <string>
#include <vector>
#include <iostream>
#include "glslprogram.h"
#include "camera.h"
class SkyBox
{
public:
    SkyBox(std::string dir, GLSLProgram& shader)
    {
        InitGL(shader);
        LoadCubemap(dir);
    }
    void InitGL(GLSLProgram& shader)
    {
        // Setup skybox VAO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    }
    void LoadCubemap(std::string dir) 
    {
        GLuint textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* image;

        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        for (GLuint i = 0; i < faces.size(); i++)
        {
            std::string path = dir + faces[i];
            image = stbi_load(path.c_str(), & width, & height, & nrComponents, 0);
            if (image) {
                //GLubyte textureColor[] = { test_color[i][0], test_color[i][1], test_color[i][2]};
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                //glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, textureColor);
                stbi_image_free(image);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(image);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        id = textureID;
    }

    void Draw(GLSLProgram &shader, Camera &camera)
    {
        //glDepthMask(GL_FALSE);
        shader.use();
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
        auto view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
        auto projection = camera.GetProjectionMatrix();

        //shader.setUniform("model", glm::scale(glm::mat4(1.0), glm::vec3(20.0, 20.0, 20.0)));
        shader.setUniform("view", view);
        shader.setUniform("projection", projection);
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        shader.setUniform("skybox", 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Set depth function back to default
        //glDepthMask(GL_TRUE);
    }

private:
    GLuint id;
    GLuint vao;
    GLuint vbo;

    std::string directory = "";

    GLfloat skyboxVertices[108] = {
        -1.0f,  1.0f, -1.0f, //luf
        -1.0f, -1.0f, -1.0f, //lbf
         1.0f, -1.0f, -1.0f, //rbf
         1.0f, -1.0f, -1.0f, //rbf
         1.0f,  1.0f, -1.0f, //ruf
        -1.0f,  1.0f, -1.0f, //luf

        //left
        -1.0f, -1.0f,  1.0f, //lbb
        -1.0f, -1.0f, -1.0f, //lbf
        -1.0f,  1.0f, -1.0f, //luf
        -1.0f,  1.0f, -1.0f, //luf
        -1.0f,  1.0f,  1.0f, //lub
        -1.0f, -1.0f,  1.0f, //lbb

        //right
         1.0f, -1.0f, -1.0f, //rbf
         1.0f, -1.0f,  1.0f, //rbb
         1.0f,  1.0f,  1.0f, //rub
         1.0f,  1.0f,  1.0f, //rub
         1.0f,  1.0f, -1.0f, //ruf
         1.0f, -1.0f, -1.0f, //rbf

         //back
        -1.0f, -1.0f,  1.0f, //lbb
        -1.0f,  1.0f,  1.0f, //lub
         1.0f,  1.0f,  1.0f, //rub
         1.0f,  1.0f,  1.0f, //rub
         1.0f, -1.0f,  1.0f, //rbb
        -1.0f, -1.0f,  1.0f, //lbb

        //up
        -1.0f,  1.0f, -1.0f, //luf
         1.0f,  1.0f, -1.0f, //ruf
         1.0f,  1.0f,  1.0f, //rub
         1.0f,  1.0f,  1.0f, //rub
        -1.0f,  1.0f,  1.0f, //lub
        -1.0f,  1.0f, -1.0f, //luf

        //bottom
        -1.0f, -1.0f, -1.0f, //lbf
        -1.0f, -1.0f,  1.0f, //lbb
         1.0f, -1.0f, -1.0f, //rbf
         1.0f, -1.0f, -1.0f, //rbf
        -1.0f, -1.0f,  1.0f, //lbb
         1.0f, -1.0f,  1.0f //rbb
    };
    //std::vector<GLfloat> skyboxVertices = {
    //    // Positions
    //    // front
    //    -1.0f,  1.0f, -1.0f, //luf
    //    -1.0f, -1.0f, -1.0f, //lbf
    //     1.0f, -1.0f, -1.0f, //rbf
    //     1.0f, -1.0f, -1.0f, //rbf
    //     1.0f,  1.0f, -1.0f, //ruf
    //    -1.0f,  1.0f, -1.0f, //luf

    //    //left
    //    -1.0f, -1.0f,  1.0f, //lbb
    //    -1.0f, -1.0f, -1.0f, //lbf
    //    -1.0f,  1.0f, -1.0f, //luf
    //    -1.0f,  1.0f, -1.0f, //luf
    //    -1.0f,  1.0f,  1.0f, //lub
    //    -1.0f, -1.0f,  1.0f, //lbb

    //    //right
    //     1.0f, -1.0f, -1.0f, //rbf
    //     1.0f, -1.0f,  1.0f, //rbb
    //     1.0f,  1.0f,  1.0f, //rub
    //     1.0f,  1.0f,  1.0f, //rub
    //     1.0f,  1.0f, -1.0f, //ruf
    //     1.0f, -1.0f, -1.0f, //rbf

    //     //back
    //    -1.0f, -1.0f,  1.0f, //lbb
    //    -1.0f,  1.0f,  1.0f, //lub
    //     1.0f,  1.0f,  1.0f, //rub
    //     1.0f,  1.0f,  1.0f, //rub
    //     1.0f, -1.0f,  1.0f, //rbb
    //    -1.0f, -1.0f,  1.0f, //lbb

    //    //up
    //    -1.0f,  1.0f, -1.0f, //luf
    //     1.0f,  1.0f, -1.0f, //ruf
    //     1.0f,  1.0f,  1.0f, //rub
    //     1.0f,  1.0f,  1.0f, //rub
    //    -1.0f,  1.0f,  1.0f, //lub
    //    -1.0f,  1.0f, -1.0f, //luf

    //    //bottom
    //    -1.0f, -1.0f, -1.0f, //lbf
    //    -1.0f, -1.0f,  1.0f, //lbb
    //     1.0f, -1.0f, -1.0f, //rbf
    //     1.0f, -1.0f, -1.0f, //rbf
    //    -1.0f, -1.0f,  1.0f, //lbb
    //     1.0f, -1.0f,  1.0f //rbb
    //};
    std::vector<std::string> faces = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "back.jpg",
        "front.jpg"
    };
    //std::vector<std::vector<int>> test_color = {
    //    {255, 0, 0},  //red
    //    {255, 255, 0}, //yellow
    //    {255, 0, 255}, //magenta
    //    {0, 255, 0}, //green
    //    {0, 0, 255}, //blue
    //    {0, 255, 255}, //cyan
    //};
};