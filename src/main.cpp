#include <manager.h>
#include <game/camera.h>
#include <graphics/glm_helper.h>
#include <graphics/model_gen.h>

#include <glm/gtc/matrix_inverse.hpp>

#include "physmanager.h"

// This example shows minimum functionality of the graphics environment
// With none of the optional libs
//
// The program draws a textured quad
// Press Esc to exit.

int main() {
    ManagerState state;
    state.cursor = cursorState::disabled;
    state.windowTitle = "DSG";
    state.conf.target_resolution[0] = 300;
    state.conf.target_resolution[1] = 300;
    state.conf.multisampling = false;
    state.conf.texture_filter_nearest = true;
    state.conf.vsync = true;

    Manager manager(state);
    ResourcePool* pool = manager.render->pool();

    PhysManager pm;

    pm.Add(PhysObj(pool->tex()->load("textures/circle.png"), glm::vec3(150, 120, 0), 40));
    PhysObj b = PhysObj(pool->tex()->load("textures/circle.png"), glm::vec3(50, 70, 0), 10);
    b.addVel(glm::vec3(0.010, -0.010, 0));
    pm.Add(b);

    ModelInfo::Model sphereData = genSurface([](float a, float b) {
	return glm::vec3(sin(a)*cos(b), sin(a)*sin(b), cos(a));
    }, true, 10, {0.0f, 3.1415f, 0.1f}, {0.0f, 6.283f, 0.1f});
    sphereData.meshes[0].diffuseTextures.push_back("sphere-surface.png");
    Resource::Model sphere = pool->model()->load(sphereData);
    
    manager.render->LoadResourcesToGPU(pool);
    manager.render->UseLoadedResources();

    camera::ThirdPerson cam;

    while(!glfwWindowShouldClose(manager.window)) {
	manager.update();
	float dt = manager.timer.dt();


	glm::vec3 spherePos = glm::vec3(-10, 0, 0);
	
	if(dt < 100) {
	    pm.Update(dt);
	    cam.setTarget(spherePos, 5);
	    cam.control(dt*0.0001f*glm::vec2(manager.input.m.dx(), manager.input.m.dy()));
	}
	
	if(manager.input.kb.press(GLFW_KEY_ESCAPE))
	    glfwSetWindowShouldClose(manager.window, GLFW_TRUE);	
	
	if(manager.winActive()) {
	    manager.render->set3DViewMat(cam.getView(), cam.getPos());
	    
	    pm.Draw(manager.render);

	    glm::mat4 sphereMat = glm::translate(glm::mat4(1.0f), spherePos);
	    manager.render->DrawModel(sphere, sphereMat, glm::inverseTranspose(sphereMat));
	    
	    manager.render->EndDraw();
	}
    }
}
