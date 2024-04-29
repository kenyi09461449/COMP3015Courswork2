#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glslprogram.h"
#include "camera.h"
#include <string>
#include <vector>
class Light
{
public:
	/*Attribute*/
	glm::vec3 Ambient = { 0.1,0.1,0.1 };
	glm::vec3 Diffuse = { 0.5,0.5,0.5 };
	glm::vec3 Specular = { 0.2,0.2,0.2 };
	
	virtual void disable() {
		Ambient = { 0,0,0 };
		Diffuse = { 0,0,0 };
		Specular = { 0,0,0 };
	}
};

class DirLight:public Light
{
public:
	glm::vec3 Direction;


	DirLight(glm::vec3 dir) :Direction(dir) {
	
	}
	void initialize(GLSLProgram& shader)
	{
		shader.setUniform("dirLight.direction", Direction);
		shader.setUniform("dirLight.ambient", Ambient);
		shader.setUniform("dirLight.diffuse",Diffuse);
		shader.setUniform("dirLight.specular",Specular);
	};
};

class PointLight :public Light
{
public:
	glm::vec3 Pos = {0.0,5.0,5.0};
	float Constant = 1.0;
	float Linear = 0.0015;
	float Quadratic = 0.000007;

	float RotationSpeed = 1.0; // 旋转速度，可根据需要调整
	float RotationAngle = 0.0; // 旋转角度
	PointLight() {
		Diffuse = { 0.0,0.7,0.5 };
	};

	PointLight(glm::vec3 intensity, glm::vec3 position) {
		Diffuse = intensity;
		Pos = position;
	};

	void initialize(GLSLProgram& shader) 
	{
		shader.setUniform("pointLight.position", Pos);
		shader.setUniform("pointLight.ambient", Ambient);
		shader.setUniform("pointLight.diffuse", Diffuse);
		shader.setUniform("pointLight.specular", Specular);
		shader.setUniform("pointLight.constant", Constant);
		shader.setUniform("pointLight.linear", Linear);
		shader.setUniform("pointLight.quadratic", Quadratic);
	};

};
class SpotLight :public Light
{
public:
	glm::vec3 Position;
	glm::vec3 Direction;
	float Constant = 1.0;
	float Linear = 0.045;
	float Quadratic = 0.0075;
	float CutOff = glm::cos(glm::radians(12.5f));
	float OuterCutOff = glm::cos(glm::radians(15.0f));
	SpotLight(glm::vec3 intensity, glm::vec3 position, glm::vec3 dir) :Position(position),Direction(dir) {
		Diffuse = intensity;
	}

	void initialize(GLSLProgram& shader, Camera &camera)
	{
		shader.setUniform("spotLight.position", camera.Position);
		shader.setUniform("spotLight.direction", camera.Front);
		shader.setUniform("spotLight.ambient", Ambient);
		shader.setUniform("spotLight.diffuse", Diffuse);
		shader.setUniform("spotLight.specular", Specular);
		shader.setUniform("spotLight.constant", Constant);
		shader.setUniform("spotLight.linear", Linear);
		shader.setUniform("spotLight.quadratic", Quadratic);
		shader.setUniform("spotLight.cutOff", CutOff);
		shader.setUniform("spotLight.outerCutOff", OuterCutOff);

	};
};