#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "helper/model.h"

using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}

void SceneBasic_Uniform::initScene()
{
    compile();
    Model model("xxx");

    std::cout << std::endl;

    prog.printActiveUniforms();


}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float delta )
{
	//update your angle here
    
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //create the rotation matrix here and update the uniform in the shader 

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3 );

    glBindVertexArray(0);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
