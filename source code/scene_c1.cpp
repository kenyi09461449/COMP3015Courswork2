#include "scene_c1.h"
#include <iostream>
#include "helper/glutils.h"

//SceneC1::SceneC1() :angle(0.0) {}

void SceneC1::initScene()
{
	compile();

	m_camera = new Camera(glm::vec3{ 0.0,0.0,20.0 });
	m_model = new Model("asserts/nanosuit/nanosuit.obj");
	m_dirLight = new DirLight(glm::vec3{ -0.2f, -1.0f, -0.3f });
	m_pointLight = new PointLight();
	m_spotLight = new SpotLight(glm::vec3{ 0.0,0.0,0.0 }, glm::vec3{0.0,0.0,0.0});
	m_skybox = new SkyBox("asserts/skybox/", skyShader);

	prog.printActiveUniforms();
	skyShader.printActiveUniforms();


}
void SceneC1::compile()
{
	try {
		prog.compileShader("shader/blinn_phone.vert");
		prog.compileShader("shader/blinn_phone.frag");
		prog.link();
		skyShader.compileShader("shader/skybox.vert");
		skyShader.compileShader("shader/skybox.frag");
		skyShader.link();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}


void SceneC1::update(float t)
{
	m_camera->Update(prog,t, (float)width/height);

	m_dirLight->Update(prog);
	m_pointLight->Update(prog,t);
	m_spotLight->Update(prog, *m_camera);

	
}

void SceneC1::render()
{
	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(1.0,1.0,1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	m_model->Draw(prog);
	//m_skybox->Draw(skyShader, *m_camera);
}

void SceneC1::resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}