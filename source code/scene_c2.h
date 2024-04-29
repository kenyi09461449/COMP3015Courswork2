#pragma once

#include "helper/scene.h"
#include <glad/glad.h>
#include "helper/glslprogram.h"
#include "helper/camera.h"
#include "helper/model.h"
#include "helper/light.h"
#include <memory>
#include "helper/skybox.h"
#include <vector>

class SceneC2 : public Scene
{
private:
    GLuint vaoHandle;
    GLSLProgram prog;
    GLSLProgram skyShader;
    GLSLProgram physics;
    GLSLProgram prog_light;
    GLSLProgram generate_shadowMap;
    GLSLProgram prog_shadow;

    SkyBox* m_skybox;
    

    vector<Model*> m_model;
    vector<Model*> m_lightmodel;
    vector<PhysicalModel*> m_physicalmodel;
    vector<DirLight*> m_dirLight;
    vector<PointLight*> m_pointLight;
    vector<GLuint> m_depthMap;
    vector<GLuint> m_dirLightShadowMap;
    vector<SpotLight*> m_spotLight;
    
    float angle;
    void compile();

public:
    Camera* m_camera;
    SceneC2() = default;
    ~SceneC2() = default;
    
    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
    void addLight(const char* type, glm::vec3 intensity, glm::vec3 position, glm::vec3 dir, int id);
    void addModel(const char* path, int id);
    void addPhysicalModel(const char* path, int id);
    void addCamera(glm::vec3 position);
    void drawShadowMap(Light& light, int i);
    void addPlane(int id);
    void processInputEvent_keyboard(PhysicalModel& mainModel);
    void reset();

    void checkSuccess();
};
