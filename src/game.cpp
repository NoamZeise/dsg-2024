#include "game.h"
#include <graphics/logger.h>
#include <graphics/model_gen.h>
#include <graphics/glm_helper.h>
#include <game/random.h>
#include <fstream>

#include "consts.h"

ModelInfo::Model SPHERE_DATA =
    genSurface([](float a, float b) {
	return glm::vec3(sin(a)*cos(b), sin(a)*sin(b), cos(a));
    }, true, 3, {0.0f, 3.1415f, 0.1f}, {0.0f, 6.283f, 0.1f});

Game::Game(ResourcePool* pool) {
    arrowTex = pool->tex()->load("textures/arrow.png");
    planetSurfaceTex = {
	pool->tex()->load("textures/sphere-surface.png"),
	pool->tex()->load("textures/sphere-surface2.png"),
	pool->tex()->load("textures/goal-planet.png"),
    };
    
    
    circle = pool->tex()->load("textures/circle.png");
    
    sphere = pool->model()->load(SPHERE_DATA);
    Level test(circle, planetSurfaceTex, sphere,
	       {{glm::vec3(200, 120, 0),
		    40, glm::vec3(-0.1, 0, 0)},
		{glm::vec3(50, 220, 0),
		 35, glm::vec3(0.1, 0.05, 0)
		}});
    player = PhysObj(circle,
		     sphere,
		     pool->tex()->load(
			     "textures/player.png"),
		     glm::vec3(0, 0, 0), 10);

    player.setColour(glm::vec4(124.0f/255, 195.0f/255, 75.0f/255, 1.0f));
    
    player.setWeight(1);
    nextLevel();
}

glm::mat4 Game::Update(float dt, Input &input, camera::ThirdPerson &cam) {
    bool pressedA = false;
    if(!isFinished && !levelDone) {
	if(start && (input.c.press(0, GLFW_GAMEPAD_BUTTON_A) ||
		     input.kb.press(GLFW_KEY_SPACE))) {
	    loadLevel(currentLevel);
	    pressedA = true;
	}
	if(input.c.press(0, GLFW_GAMEPAD_BUTTON_DPAD_DOWN) ||
	   input.kb.press(GLFW_KEY_DOWN)) {
	    randomMode = !randomMode;
	    NextLevel();
	}
	if(pm.playerTouchedGoal()) {
	    levelDone = true;
	}
	if(input.c.press(0, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT) ||
	   input.kb.press(GLFW_KEY_RIGHT)) {
	    NextLevel();
	}
	if(input.c.press(0, GLFW_GAMEPAD_BUTTON_DPAD_LEFT) ||
	   input.kb.press(GLFW_KEY_LEFT)) {
	    if(!randomMode) {
		currentLevelNum -= 2;
		if(currentLevelNum < -1)
		    currentLevelNum = -1;
	    }
	    NextLevel();
	}
    

	if(start) {
	    pm.Update(dt);
	} else if(!pressedA){
	    golfUpdate(input, dt);
	}
    }

    camUpdate(dt, input, cam);
    
    if(start && !isFinished) {
	glm::vec3 controller = glm::vec3(input.c.axis(0, GLFW_GAMEPAD_AXIS_LEFT_X),
					 input.c.axis(0, GLFW_GAMEPAD_AXIS_LEFT_Y),
					 0);
	if(fabs(controller.x) < 0.05)
	    controller.x = 0;
	if(fabs(controller.y) < 0.05)
	    controller.y = 0;
	
	pm.getObj(playerID)->addVel(controller*dt*0.0001f);
    }
    
    auto target = pm.getObj(this->target);
    float scale = 0.2f;
    glm::mat4 view2d =
	glm::translate(
		glm::scale(
			glm::mat4(1.0f),
			glm::vec3(scale)),
		glm::vec3(-target->getPos().x + RENDER_WIDTH/(2*scale),
			  -target->getPos().y + RENDER_HEIGHT/(2*scale),
			  0));
    return view2d;
}

void Game::camUpdate(float dt, Input &input, camera::ThirdPerson &cam) {
    int lt = target;
    if(input.kb.press(GLFW_KEY_Q) || input.c.press(0, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER))
	target++;
    if(input.kb.press(GLFW_KEY_E) || input.c.press(0, GLFW_GAMEPAD_BUTTON_LEFT_BUMPER))
	target--;

    
    if(target < 0)
	target = pm.Count() - 1;
    if(target >= pm.Count())
	target = 0;

    bool changedTarget = target != lt;
    
    auto t = pm.getObj(target);
    if(t != nullptr) 
	cam.setTarget(t->getPos(), t->getRadius() * 10);
    glm::vec2 camVec = 
	glm::vec2(input.m.dx(),
		  input.m.dy());

    glm::vec2 arrows(0);
    /*
    if(input.kb.hold(GLFW_KEY_RIGHT))
	arrows.x += 1;
    if(input.kb.hold(GLFW_KEY_LEFT))
	arrows.x -= 1;
    if(input.kb.hold(GLFW_KEY_UP))
	arrows.y -= 1;
    if(input.kb.hold(GLFW_KEY_DOWN))
    arrows.y += 1;*/

    glm::vec2 controller = glm::vec2(input.c.axis(0, GLFW_GAMEPAD_AXIS_RIGHT_X),
				     input.c.axis(0, GLFW_GAMEPAD_AXIS_RIGHT_Y));
    
    if(fabs(controller.x) < 0.07)
	controller.x = 0;
    if(fabs(controller.y) < 0.07)
	controller.y = 0;
    
    arrows += controller;

    camVec += arrows*10.0f;

    cam.control(camVec*-dt*0.0001f);
	
    PhysObj* other = pm.nearestObj();
    if(other != nullptr && t != nullptr) {
	glm::vec3 pDir(0);
	if(target == playerID)
	    pDir = t->getPos() - other->getPos();
	else
	    pDir = t->getPos() - pm.getObj(playerID)->getPos();
	pDir += glm::vec3(0, 0, 0.4)*glm::length(pDir);
	glm::vec3 c = cam.getForward();
	float diff = 0.00005f*dt;
	    
	if(changedTarget)
	    cam.setForward(pDir);
	else if(start)
	    cam.setForward(pDir*diff + (1-diff)*c);
    }
}

void Game::golfUpdate(Input &input, float dt) {
    auto player = pm.getObj(playerID);

    const float STRIKE_LIMIT = 1000;
    const float STRIKE_INTENSITY = 0.0003f;
    
    glm::vec3 dir(0);
    if(input.kb.hold(GLFW_KEY_W))
	dir.y -= 1;
    if(input.kb.hold(GLFW_KEY_A))
	dir.x -= 1;
    if(input.kb.hold(GLFW_KEY_S))
	dir.y += 1;
    if(input.kb.hold(GLFW_KEY_D))
	dir.x += 1;
    
    glm::vec2 controller = glm::vec2(input.c.axis(0, GLFW_GAMEPAD_AXIS_LEFT_X),
				     input.c.axis(0, GLFW_GAMEPAD_AXIS_LEFT_Y));

    if(fabs(controller.x) > 0.05 || fabs(controller.y) > 0.05) {
	controller *= STRIKE_LIMIT;
	strikeDir = glm::vec3(controller.x, controller.y, 0);
    }
    if(dir != glm::vec3(0)) {	
	strikeDir += dir * dt;
	float l = glm::length(strikeDir);
	if(l > STRIKE_LIMIT)
	    strikeDir = (strikeDir/l)*STRIKE_LIMIT;
    }

    glm::vec2 arrowDir(strikeDir.x, strikeDir.y);
    float size = glm::length(arrowDir)*3;
    auto norm = arrowDir/size;
    size *= 0.1f;
    arrowMat = glmhelper::calcMatFromRect(
	    glm::vec4(player->getPos().x - size/2, player->getPos().y - size/2, size, size),
	    glm::degrees(atan2(norm.y, norm.x) + (22.0/14)), 0.9f);
	    
    if(input.kb.press(GLFW_KEY_SPACE) ||
       input.c.press(0, GLFW_GAMEPAD_BUTTON_A)) {
	player->addVel(strikeDir*STRIKE_INTENSITY);
	start = true;
    }
}

void Game::Draw(Render *render) {
    pm.Draw(render);
    if(!start) {
	render->DrawQuad(arrowTex, arrowMat);
    }
}


void Game::loadLevel(Level level) {
    pm.clear();
    pm.Add(player);
    for(auto &p: level.planets)
	pm.Add(p);
    pm.Update(1);
    start = false;
    currentLevel = level;
    target = 10000;
}

void Game::nextLevelRandom() {
    int np = game::random::posReal()*10 + 3;
    std::vector<Planet> planets;
    for(int i = 0; i < np; i++) {
	float x = game::random::real() * 700;
	float y = game::random::real() * 700;
	float r = (float)game::random::posReal() * 100 + 10;
	if(fabs(x) + r < 100)
	    x += glm::sign(x)*100;
	if(fabs(y) + r < 100)
	    y += glm::sign(y)*100;
	planets.push_back({
		glm::vec3(x, y,
			  game::random::real() * 10),
		r,
		glm::vec3(game::random::real() * 1,
			  game::random::real() * 1,
			  game::random::real() * 0.1),
		} );
    }
    loadLevel(Level(circle, planetSurfaceTex, sphere, planets));
}

void Game::nextLevel() {
    std::vector<Planet> planets;
    currentLevelNum++;
    
    std::string path = "levels/" + std::to_string(currentLevelNum) + ".txt";
    std::ifstream in(path);
    if(!in.good()) {
	isFinished = true;
	return;
    }
    float x, y, z, r, vx, vy, vz;
    while(in >> x >> y >> z >> r >> vx >> vy >> vz) {
	planets.push_back({
		glm::vec3(x, y, z), r, glm::vec3(vx, vy, vz)});
    }

    in.close();
    
    loadLevel(Level(circle, planetSurfaceTex, sphere, planets));
}

void Game::Reset() {
    currentLevelNum = 0;
    isFinished = false;
    levelDone = false;
    randomMode = false;
    nextLevel();
}

void Game::NextLevel() {
    levelDone = false;
    if(randomMode)
	nextLevelRandom();
    else
	nextLevel();    
}

Level::Level(Resource::Texture planet2d, std::vector<Resource::Texture>& planetTexs, Resource::Model sphere, std::vector<Planet> planets) {
    int i = -1;
    for(auto &p: planets) {
	if(i < planetTexs.size() - 2)
	    i++;
	else
	    i = 0;
        this->planets.push_back(
		PhysObj(planet2d,
			sphere,
			planetTexs[i],
			p.pos, p.radius));
	this->planets.back().addVel(p.vel);
    }
    if(!this->planets.empty()) {
	this->planets.back().setTex(planetTexs.back());
	this->planets.back().setColour(glm::vec4(182.0f/255, 41.0f/255, 56.0f/255, 1.0f));
    }
}
