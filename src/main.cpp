#include <manager.h>
#include <game/camera.h>
#include <graphics/glm_helper.h>
#include <graphics/logger.h>

#include <glm/gtc/matrix_inverse.hpp>
#include <cstring>

#include "game.h"
#include "cursor.h"
#include "consts.h"

// This example shows minimum functionality of the graphics environment
// With none of the optional libs
//
// The program draws a textured quad
// Press Esc to exit.

int main(int argc, char** argv) {
    RenderFramework framework = RenderFramework::Vulkan;
    for(int i = 0; i < argc; i++)
    	if(strcmp(argv[i], "opengl") == 0)
    	    framework = RenderFramework::OpenGL;
    ManagerState state;
    state.cursor = cursorState::normal;
    state.defaultRenderer = framework;
    state.windowTitle = "DSG";
    state.windowWidth  = RENDER_WIDTH * 2.5;
    state.windowHeight = RENDER_HEIGHT * 2.5;
    state.conf.target_resolution[0] = RENDER_WIDTH;
    state.conf.target_resolution[1] = RENDER_HEIGHT;
    state.conf.multisampling = false;
    state.conf.texture_filter_nearest = true;
    state.conf.vsync = true;
    state.conf.clear_colour[0] = 13.0f/255;
    state.conf.clear_colour[1] = 33.0f/255;
    state.conf.clear_colour[2] = 69.0f/255;
    state.conf.depth_range_3D[1] = 10000.0f;

    Manager manager(state);
    ResourcePool* pool = manager.render->pool();

    Game game(pool);

    Resource::Texture winScreen = pool->tex()->load("textures/victory.png");
    Resource::Texture nextLvl = pool->tex()->load("textures/next-level.png");
    
    manager.render->LoadResourcesToGPU(pool);
    manager.render->UseLoadedResources();
    
    camera::ThirdPerson cam;

    glm::mat4 view2D(1);

    bool won = false;
    
    while(!glfwWindowShouldClose(manager.window)) {
	manager.update();
	
	float dt = manager.timer.dt();	
	if(dt < 100) {

	    view2D = game.Update(dt, manager.input, cam);
	    if(won &&
	       (manager.input.c.press(0, GLFW_GAMEPAD_BUTTON_A) ||
		manager.input.kb.press(GLFW_KEY_SPACE))) {
		won = false;
		game.Reset();
	    }
	    else if(game.gameFinished()) {
		won = true;
	    }
	    if(!won && game.isLevelDone() &&
	       (manager.input.c.press(0, GLFW_GAMEPAD_BUTTON_A) ||
		manager.input.kb.press(GLFW_KEY_SPACE))) {
		game.NextLevel();
	    }
	}
	
	if(manager.input.kb.press(GLFW_KEY_F) ||
	   manager.input.c.press(0, GLFW_GAMEPAD_BUTTON_BACK))
	    manager.toggleFullscreen();
	
	if(manager.winActive()) {
	    manager.render->set3DViewMat(cam.getView(), cam.getPos());
	    manager.render->set2DViewMat(view2D);
	    
	    game.Draw(manager.render);

	    if(won) {
                manager.render->DrawQuad(winScreen,					 glm::inverse(view2D)*glmhelper::calcMatFromRect(glm::vec4(0, 0, RENDER_WIDTH, RENDER_HEIGHT), 0, 1.5f)                    );
	    }
	    if(game.isLevelDone()) {
                manager.render->DrawQuad(nextLvl,					 glm::inverse(view2D)*glmhelper::calcMatFromRect(glm::vec4(0, 0, RENDER_WIDTH, RENDER_HEIGHT), 0, 1.5f)                    );
	    }
	    
	    manager.render->EndDraw();
	}
    }
}
