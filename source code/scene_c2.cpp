#include "scene_c2.h"
#include <iostream>
#include "helper/glutils.h"


void SceneC2::initScene()
{
	glClearColor(0.1, 0.1, 0.1, 1.0f);
	compile();
	//m_camera = new Camera(glm::vec3{ 0.0,0.0,20.0 });
	this->addCamera(glm::vec3{ 10,10,10 });
	int id = 0; 

	this->addPlane(id++);
	
	// this->addModel("asserts/nanosuit/nanosuit.obj", id++);
	// file parser is not powerful enough, so I set these values myself
	this->addModel("asserts/scene/low_poly_tree/Lowpoly_tree_sample.obj", id++);
	this->m_model[100]->Position = glm::vec3(10.0, 0.0, -30.0);
	this->m_model[100]->meshes[0].color = { 0.207595, 0.138513, 0.055181 };
	this->m_model[100]->meshes[1].color = { 0.256861, 0.440506, 0.110769 };

	this->addModel("asserts/scene/low_poly_tree/Lowpoly_tree_sample.obj", id++);
	this->m_model[101]->Position = glm::vec3(-10.0, 0.0, -30.0);
	this->m_model[101]->meshes[0].color = { 0.207595, 0.138513, 0.055181 };
	this->m_model[101]->meshes[1].color = { 0.256861, 0.440506, 0.110769 };

	// replace this with the physical object you want to focus
	// always assume m_physicalmodel[0] is the one you want to foces
	this->addPhysicalModel("asserts/spaceship-obj/Intergalactic_Spaceship-(Wavefront).obj", id++);
	this->m_physicalmodel[0]->Position = glm::vec3(0.0, 30.0, 0.0);
	this->addLight("DirLight", { 0.4,0.4,0.4 }, { 0,0,0 }, glm::vec3{ 0.0f, -5.f, -5.0f }, id++);
	//this->addLight("PointLight", glm::vec3{ 0.2,0.2,0.2 }, { 0,10,0 }, { 0,0,0 },id++);
	//this->addLight("SpotLight", { 0,0,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ -0.2f, -1.0f, -0.3f });
	m_skybox = new SkyBox("asserts/skybox/", skyShader);

	prog_shadow.use();
	for (auto light : m_dirLight) {
		light->initialize(prog_shadow);
	}
	for (auto light : m_pointLight) {
		light->initialize(prog_shadow);
	}
	for (auto light : m_spotLight) {
		light->initialize(prog_shadow, *m_camera);
	}

}
void SceneC2::compile()
{
	try {

		/*prog.compileShader("shader/blinn_phone.vert");
		prog.compileShader("shader/blinn_phone.frag");
		prog.link();*/
		skyShader.compileShader("shader/skybox.vert");
		skyShader.compileShader("shader/skybox.frag");
		skyShader.link();
		physics.compileShader("shader/physical_compute_force.vert");
		const GLchar* feedbackVaryings[] = { "force", "torque"};
		glTransformFeedbackVaryings(physics.getHandle(), 2, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
		physics.link();
		prog_light.compileShader("shader/blinn_phone.vert");
		prog_light.compileShader("shader/blinn_phone_light.frag");
		prog_light.link();
		generate_shadowMap.compileShader("shader/generate_shadow_map.vert");
		generate_shadowMap.compileShader("shader/generate_shadow_map.frag");
		generate_shadowMap.link();
		prog_shadow.compileShader("shader/blinn_phone_shadow.vert");
		prog_shadow.compileShader("shader/blinn_phone_shadow.frag");
		prog_shadow.link();

	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}


void SceneC2::update(float t)
{
	/*processInputEvent(*m_camera, *m_physicalmodel[0]);*/
	m_camera->Update(t,m_physicalmodel[0]->current_position, m_physicalmodel[0]->current_velocity);
	processInputEvent_keyboard(*m_physicalmodel[0]);

	checkSuccess();
}

void SceneC2::render()
{
	// Setup some OpenGL options
	m_camera->Front = glm::normalize(m_physicalmodel[0]->current_position - m_camera->Position);
	//processInputEvent(*m_camera, *m_physicalmodel[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (m_skybox) {
		m_skybox->Draw(skyShader, *m_camera);
	}

	glCullFace(GL_FRONT);
	for (int i = 0; i < m_dirLight.size();i++) {
		drawShadowMap(*m_dirLight[i], i);
		prog_shadow.use();
		glBindFramebuffer(GL_FRAMEBUFFER, m_dirLightShadowMap[i]);
		glActiveTexture(GL_TEXTURE10 + i);
		glBindTexture(GL_TEXTURE_2D, m_depthMap[i]);
		prog_shadow.setUniform("shadowMap", 10+i);
		// Now we only have one direction light, TODO: set more
		
	}
	glCullFace(GL_BACK);
	//// create a buffer to hold the printf results
	//GLuint printBuffer = createPrintBuffer();
	//// bind it to the current program
	//bindPrintBuffer(prog_shadow.getHandle(), printBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	for (auto model : m_model) {
		for (auto light : m_dirLight) {
			model->Draw(*light, *this->m_camera, prog_shadow,m_physicalmodel[0]->current_position);
		}
	}
	
	for (auto model : m_physicalmodel) {
		for (auto light : m_dirLight) {
			model->Draw(*light ,*this->m_camera, physics, prog_shadow);
		}
	}

	//for (auto model : m_lightmodel) {
	//	for (auto light : m_dirLight) {
	//		model->Draw(*light, *this->m_camera, prog_light);
	//	}
	//}

	/*cout << "output from shader:" << getPrintBufferString(printBuffer) << endl;
	deletePrintBuffer(printBuffer);*/
}

void SceneC2::resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}

void SceneC2::addPlane(int id) {
	string type = "square";
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			Model* model = new Model(type, id);
			model->Position = {-300+60*i,0,-300+60*j};
			m_model.push_back(model);
		}
	}
	
}

void SceneC2::processInputEvent_keyboard(PhysicalModel & mainModel)
{
	if (Input::Instance().IsKeyPressed(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		mainModel.addForce(mainModel.getRotation()*glm::vec3(0,4,20));
	}
	if (Input::Instance().IsKeyPressed(GLFW_KEY_W) == GLFW_PRESS) {
		constexpr float angle = -1.f * glm::pi<float>() / 180.0f;
		glm::quat rotationQuat = glm::angleAxis(angle, glm::vec3(1.0f, 0.2f, 0.0f));
		mainModel.changeRotation(rotationQuat);
	}
	if (Input::Instance().IsKeyPressed(GLFW_KEY_S) == GLFW_PRESS) {
		constexpr float angle = 1.f * glm::pi<float>() / 180.0f;
		glm::quat rotationQuat = glm::angleAxis(angle, glm::vec3(1.0f, 0.2f, 0.0f));
		mainModel.changeRotation(rotationQuat);
	}
	if (Input::Instance().IsKeyPressed(GLFW_KEY_A) == GLFW_PRESS) {
		constexpr float angle = 1.f * glm::pi<float>() / 180.0f;
		glm::quat rotationQuat = glm::angleAxis(angle, glm::vec3(0.0f, 0.2f, 1.0f));
		mainModel.changeRotation(rotationQuat);
	}
	if (Input::Instance().IsKeyPressed(GLFW_KEY_D) == GLFW_PRESS) {
		constexpr float angle = -1.f * glm::pi<float>() / 180.0f;
		glm::quat rotationQuat = glm::angleAxis(angle, glm::vec3(0.0f, 0.2f, 1.0f));
		mainModel.changeRotation(rotationQuat);
	}
	if (Input::Instance().IsKeyPressed(GLFW_KEY_R) == GLFW_PRESS) {
		reset();
	}

}

void SceneC2::addModel(const char* path, int id) {
	Model* model = new Model(path, id);
	m_model.push_back(model);
}

void SceneC2::addPhysicalModel(const char* path, int id)
{
	PhysicalModel* model = new PhysicalModel(path, id);
	m_physicalmodel.push_back(model);
}

void SceneC2::addLight(const char* type, glm::vec3 intensity, glm::vec3 position, glm::vec3 dir, int id) {
	if (type == "DirLight") {
		DirLight* light = new DirLight(dir);
		m_dirLight.push_back(light);
		light->Diffuse = intensity;
		GLuint depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);

		const GLuint SHADOW_WIDTH = width, SHADOW_HEIGHT = height;

		GLuint depthMap;
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		m_dirLightShadowMap.push_back(depthMapFBO);
		m_depthMap.push_back(depthMap);
	}
	else if (type == "PointLight") {
		PointLight* light = new PointLight(intensity, position);
		m_pointLight.push_back(light);
		string type = "light";
		Model* model = new Model(type, id);
		model->Position = position;
		m_lightmodel.push_back(model);
	}
	else if(type == "SpotLight"){
		SpotLight* light = new SpotLight(intensity, position, dir);
		m_spotLight.push_back(light);
	}
	else {
		cerr << "not supported light!" << endl;
		exit(-1);
	}
}

void SceneC2::addCamera(glm::vec3 position) {
	m_camera = new Camera(position);
}

void SceneC2::drawShadowMap(Light& light, int i) {
	// we focus on the direction light
	DirLight& dLight = dynamic_cast<DirLight&>(light);
	glBindFramebuffer(GL_FRAMEBUFFER, m_dirLightShadowMap[i]);
	glClear(GL_DEPTH_BUFFER_BIT);
	for (auto model : m_model) {
		model->DrawShadow(dLight, generate_shadowMap, false, m_physicalmodel[0]->current_position);
		//model->DrawShadow(dLight, prog);
	}
	for (auto model : m_physicalmodel) {
		model->DrawShadow(dLight, generate_shadowMap, true, m_physicalmodel[0]->current_position);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneC2::reset() {
	auto temp = m_physicalmodel[0];
	m_physicalmodel.pop_back();
	delete temp;
	
	this->addPhysicalModel("asserts/spaceship-obj/Intergalactic_Spaceship-(Wavefront).obj", 1);
	this->m_physicalmodel[0]->Position = glm::vec3(0.0, 30.0, 0.0);
	this->m_camera->Position = { 10,10,10 };
}

void SceneC2::checkSuccess() {
	if ((m_physicalmodel[0]->current_position[0] < 10    && m_physicalmodel[0]->current_position[0]  > -10) &&
		(m_physicalmodel[0]->current_position[1] > 0     && m_physicalmodel[0]->current_position[1] < 20)   &&
		(m_physicalmodel[0]->current_position[2] < -29.5 && m_physicalmodel[0]->current_position[2] > -30.5)) {
		cout << "you made it!!!!" << endl;
	}
}