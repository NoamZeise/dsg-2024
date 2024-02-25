#include <manager.h>
#include <game/camera.h>
#include <graphics/glm_helper.h>
#include <graphics/logger.h>

#include <glm/gtc/matrix_inverse.hpp>
#include <cstring>

#include "game.h"
#include "cursor.h"

// This example shows minimum functionality of the graphics environment
// With none of the optional libs
//
// The program draws a textured quad
// Press Esc to exit.

const int RENDER_WIDTH = 300;
const int RENDER_HEIGHT = 300;

int main(int argc, char** argv) {
    RenderFramework framework = RenderFramework::Vulkan;
    for(int i = 0; i < argc; i++)
    	if(strcmp(argv[i], "opengl") == 0)
    	    framework = RenderFramework::OpenGL;
    ManagerState state;
    state.cursor = cursorState::disabled;
    state.defaultRenderer = framework;
    state.windowTitle = "DSG";
    state.windowWidth  = RENDER_WIDTH * 2.5;
    state.windowHeight = RENDER_HEIGHT * 2.5;
    state.conf.target_resolution[0] = RENDER_WIDTH;
    state.conf.target_resolution[1] = RENDER_HEIGHT;
    state.conf.multisampling = false;
    state.conf.texture_filter_nearest = true;
    state.conf.vsync = true;

    Manager manager(state);
    ResourcePool* pool = manager.render->pool();

    Game game(pool);

    auto cursor = Cursor(pool->tex()->load("textures/circle.png"),
			 glm::vec2(RENDER_WIDTH, RENDER_HEIGHT));
    
    manager.render->LoadResourcesToGPU(pool);
    manager.render->UseLoadedResources();
    
    camera::ThirdPerson cam;
    
    while(!glfwWindowShouldClose(manager.window)) {
	manager.update();
	
	float dt = manager.timer.dt();	
	if(dt < 100) {
	    cursor.Update(manager.input);
	    game.Update(dt, manager.input, cam);
	}
	
	if(manager.input.kb.press(GLFW_KEY_ESCAPE))
	    glfwSetWindowShouldClose(manager.window, GLFW_TRUE);
	
	if(manager.winActive()) {
	    manager.render->set3DViewMat(cam.getView(), cam.getPos());
	    
	    game.Draw(manager.render);
	    //cursor.Draw2D(manager.render);
	    
	    manager.render->EndDraw();
	}
    }
}
