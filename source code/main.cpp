#include "helper/scene.h"
#include "helper/scenerunner.h"
#include "scene_c2.h"

int main(int argc, char* argv[])
{
	SceneRunner runner("c2");

	std::unique_ptr<Scene> scene;

	SceneC2* current_scene = new SceneC2();
	

	scene = std::unique_ptr<Scene>(current_scene);


	return runner.run(*scene);
}