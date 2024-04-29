#ifndef MODEL_H
#define MODEL_H
#include "camera.h"
#include "glslprogram.h"
#include "mesh.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "light.h"

//create opengl texture obj
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);


class Model
{
public:
    /*attritbute*/
    glm::vec3 Position = glm::vec3(0.0,0.0,0.0);
    glm::vec3 Scale = glm::vec3(1.0,1.0, 1.0);
    glm::mat4 transform;
    int id;

    Model(string type, int id)
    {
		if (type == "square") {
            Scale = { 30.f,30.f,30.f };
            vector<Vertex> vertices;
			GLfloat nvertices[32] = {
	            //bottom
	            -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //lbf
	            -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //lbb
	             1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //rbf
	             1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, //rbb
			};
            for (unsigned int i = 0; i < 32; i += 8)
            {
                Vertex vertex;
                vertex.Position = glm::vec3(nvertices[i], nvertices[i + 1], nvertices[i + 2]);
                vertex.Normal = glm::vec3(nvertices[i + 3], nvertices[i + 4], nvertices[i + 5]);
                vertex.TexCoords = glm::vec2(nvertices[i + 6], nvertices[i + 7]);
                vertices.push_back(vertex);
            }
            vector<unsigned int> indices;
            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);
            indices.push_back(1);
            indices.push_back(2);
            indices.push_back(3);
            vector<Texture> textures;
            for (int i = 0; i < 2; i++) {
                Texture texture;
                texture.id = TextureFromFile("detail.BMP", "asserts/scene");
                texture.path = "asserts/scene/detail.BMP";
                if (i == 1) {
                    texture.type = "diffuse";
                }
                else {
                    texture.type = "specular";
                }
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
            
            this->meshes.push_back(Mesh(vertices,indices,textures));
            
		}
        else if (type == "light") {
            Scale = { 0.1,0.1,0.1 };
            vector<Vertex> vertices;
            //GLfloat nvertices[108] = {
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
            GLfloat nvertices[24] = {
                -1.0f,  1.0f, -1.0f, //luf0
                -1.0f, -1.0f, -1.0f, //lbf1
                 1.0f, -1.0f, -1.0f, //rbf2
                 1.0f,  1.0f, -1.0f, //ruf3
                -1.0f, -1.0f,  1.0f, //lbb4
                -1.0f,  1.0f,  1.0f, //lub5
                 1.0f, -1.0f,  1.0f, //rbb6
                 1.0f,  1.0f,  1.0f, //rub7

            };
            for (unsigned int i = 0; i < 24; i += 3)
            {
                Vertex vertex;
                vertex.Position = glm::vec3(nvertices[i], nvertices[i + 1], nvertices[i + 2]);
                vertices.push_back(vertex);
            }
            vector<unsigned int> indices;
            indices.push_back(0); indices.push_back(1); indices.push_back(2); indices.push_back(2); indices.push_back(3); indices.push_back(0);
            indices.push_back(4); indices.push_back(1); indices.push_back(0); indices.push_back(0); indices.push_back(5); indices.push_back(4);
            indices.push_back(2); indices.push_back(6); indices.push_back(7); indices.push_back(7); indices.push_back(3); indices.push_back(2);
            indices.push_back(4); indices.push_back(5); indices.push_back(7); indices.push_back(7); indices.push_back(6); indices.push_back(4);
            indices.push_back(0); indices.push_back(3); indices.push_back(7); indices.push_back(7); indices.push_back(5); indices.push_back(0);
            indices.push_back(1); indices.push_back(6); indices.push_back(2); indices.push_back(2); indices.push_back(4); indices.push_back(6);
            vector<Texture> textures;

            this->meshes.push_back(Mesh(vertices, indices, textures));
        }
        this->id = id;
        transform = glm::scale(glm::mat4(1.0), Scale);
        transform[3] = glm::vec4(Position, 1.0f);
    }

    Model(const char* path, int id)
    {
        loadModel(path);

        this->id = id;
        transform = glm::scale(glm::mat4(1.0), Scale);
        transform[3] = glm::vec4(Position, 1.0f);
    }
    void Draw(DirLight& light, Camera& camera,GLSLProgram& shader, glm::vec3 target);

    void DrawShadow(DirLight& light, GLSLProgram& shader, bool isPhysicalObject, glm::vec3 target);
    
public:
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
protected:
    
    string directory;
    /*  º¯Êý   */
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    //get 
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

class PhysicalModel : public Model
{
public:
    glm::vec3 current_position;
    glm::vec3 current_velocity;
    PhysicalModel(const char* path, int id):Model(path, id) {
        partical_mass = 1.0;
        
        for (auto& i : this->meshes) {
            for (auto& j : i.vertices) {
                body_mass += partical_mass;
                cm_postion += partical_mass * j.Position;
            }
        }
        cm_postion /= body_mass;
        origin_cm_position = cm_postion;
        inertia = { {0,0,0}, {0,0,0}, {0,0,0} };

        velocity = { 0,0,0 };
        force = { 0,0,0 };
        torque = { 0,0,0 };
        angular_velocity = {0,0,0};

        for (auto& i : this->meshes) {
            for (auto& j : i.vertices) {
                inertia += partical_mass * (glm::dot((j.Position - cm_postion), j.Position - cm_postion) * glm::mat3(1)
                    - glm::outerProduct(j.Position - cm_postion, j.Position - cm_postion));
            }
        }
        origin_inverse_inertia = glm::inverse(inertia);
        inverse_inertia = origin_inverse_inertia;
        angular_momentum = inertia * angular_velocity;
        rotation = glm::mat3(1);
        rotation_quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        for (int i = 0; i < this->meshes.size(); i++) {
            // generate(copy) this meshes' VAO,VBO,EBO, since the calculation will change the data in VBO
            unsigned int temp_vao, temp_vbo, temp_ebo;
            glGenVertexArrays(1, &temp_vao);
            glGenBuffers(1, &temp_vbo);
            glGenBuffers(1, &temp_ebo);

            glBindVertexArray(temp_vao);
            glBindBuffer(GL_ARRAY_BUFFER, temp_vbo);
            glBufferData(GL_ARRAY_BUFFER, meshes[i].vertices.size() * sizeof(Vertex), &meshes[i].vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[i].indices.size() * sizeof(unsigned int), &meshes[i].indices[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            temp_vaos.push_back(temp_vao);
            temp_vbos.push_back(temp_vbo);
            temp_ebos.push_back(temp_ebo);

            unsigned int feedbackBuffer;

            glGenBuffers(1, &feedbackBuffer);
            glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, feedbackBuffer);
            glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, 6 * meshes[i].vertices.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedbackBuffer);
            feedbackBuffers.push_back(feedbackBuffer);
        }
        
        collision_stiffness = 10e4f;

        // assume that you want max falling velocity = 60m/s, the frictional force by air is linear here 
        // for each particle we have gravity = p_mass * 9.8 and frictional force = k * 60
        damping_stiffness_byair = partical_mass *9.8/60;
        damping_stiffness_byground = 1000;
        current_position = Position;
        current_velocity = { 0,0,0 };
    }

    ~PhysicalModel() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        for (int i = 0; i < meshes.size(); i++) {
            glDeleteVertexArrays(1, &temp_vbos[i]);
            glDeleteBuffers(1, &temp_vbos[i]);
            glDeleteBuffers(1, &temp_ebos[i]);
            glDeleteBuffers(1, &feedbackBuffers[i]);
        }
        
    }

    void Draw(DirLight& light, Camera& camera,GLSLProgram& physicalShader, GLSLProgram& renderShader);

    void DrawShadow(DirLight& light, GLSLProgram& renderShader, bool isPhysicalObject, glm::vec3 target);

    void addForce(glm::vec3 force);

    glm::mat3x3 getRotation();

    void changeRotation(glm::quat rot);
private:

    

    float dt_inv = 60*10;
    vector<unsigned int> temp_vaos;
    vector<unsigned int> temp_vbos;
    vector<unsigned int> temp_ebos;
    vector<unsigned int> feedbackBuffers;
    const glm::vec3 gravity = { 0.0,-9.8,0.0 };

    // now we have to use current_position to track the current postion of this object's position in world coordinate system
    // you can not simply store it in Position because in computing physics we use Position + current_position - cm_postion to do that
    // if you want to store it in Position you have to compute the cm_position again in the world coordinate system and we want to avoid that
    // and current_position will always be equal to Position + current_position - cm_postion

    glm::vec3 cm_postion;
    glm::vec3 origin_cm_position;
    glm::vec3 velocity;
    glm::vec3 force;
    glm::vec3 torque;
    glm::vec3 angular_velocity;
    float partical_mass;
    float body_mass = 0.0;
    glm::quat rotation_quat;
    glm::mat3x3 rotation;
    glm::vec3 angular_momentum;
    glm::mat3x3 inertia;
    glm::mat3x3 inverse_inertia;
    glm::mat3x3 origin_inverse_inertia;
    float collision_stiffness;
    float damping_stiffness_byair;
    float damping_stiffness_byground;
    

    glm::quat quat_multi(glm::quat p, glm::quat q) {
        return glm::quat(
            p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3],
            p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2],
            p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1],
            p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0]
        );
    }

    glm::mat3x3 quat_to_mat(glm::quat q) {
        return glm::mat3x3(
            { q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z, 2 * (q.x * q.y - q.w * q.z), 2 * (q.x * q.z + q.w * q.y) },
            { 2 * (q.x * q.y + q.w * q.z), (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z), 2 * (q.y * q.z - q.w * q.x) },
            { 2 * (q.x * q.z - q.w * q.y) , 2 * (q.y * q.z + q.w * q.x), (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z) }
        );
    }
};


#endif