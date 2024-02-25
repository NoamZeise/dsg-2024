#include "game.h"
#include <graphics/logger.h>
#include <graphics/model_gen.h>
#include <graphics/glm_helper.h>

ModelInfo::Model SPHERE_DATA =
    genSurface([](float a, float b) {
	return glm::vec3(sin(a)*cos(b), sin(a)*sin(b), cos(a));
    }, true, 10, {0.0f, 3.1415f, 0.1f}, {0.0f, 6.283f, 0.1f});

Game::Game(ResourcePool* pool) {
    arrowTex = pool->tex()->load("textures/arrow.png");
    
    Resource::Model sphere = pool->model()->load(SPHERE_DATA);
    Level test(pool, sphere,
	       { {"textures/sphere-surface.png",
			  glm::vec3(150, 120, 0),
			  40}});
    player = PhysObj(pool->tex()->load("textures/circle.png"),
			sphere,
			pool->tex()->load(
				"textures/sphere-surface2.png"),
			glm::vec3(50, 70, 0),
			10);
    pm.Add(player);
    for(auto &p: test.planets)
	pm.Add(p);
    pm.Update(1);
}

void Game::Update(float dt, Input &input, camera::ThirdPerson &cam) {
    if(start) {
	pm.Update(dt);
    } else {
	golfUpdate(input, dt);
    }
    camUpdate(dt, input, cam);
}

void Game::camUpdate(float dt, Input &input, camera::ThirdPerson &cam) {
    int lt = target;
    if(input.kb.press(GLFW_KEY_COMMA))
	target++;
    if(input.kb.press(GLFW_KEY_PERIOD))
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
    if(input.kb.hold(GLFW_KEY_RIGHT))
	arrows.x += 1;
    if(input.kb.hold(GLFW_KEY_LEFT))
	arrows.x -= 1;
    if(input.kb.hold(GLFW_KEY_UP))
	arrows.y -= 1;
    if(input.kb.hold(GLFW_KEY_DOWN))
	arrows.y += 1;

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
	float diff = 0.0001f*dt;
	    
	if(changedTarget)
	    cam.setForward(pDir);
	else if(start)
	    cam.setForward(pDir*diff + (1-diff)*c);
    }
   
}

void Game::golfUpdate(Input &input, float dt) {
    auto player = pm.getObj(playerID);
    
    glm::vec3 dir(0);
    if(input.kb.hold(GLFW_KEY_W))
	dir.y -= 1;
    if(input.kb.hold(GLFW_KEY_A))
	dir.x -= 1;
    if(input.kb.hold(GLFW_KEY_S))
	dir.y += 1;
    if(input.kb.hold(GLFW_KEY_D))
	dir.x += 1;
    strikeDir += dir * dt;

    glm::vec2 arrowDir(strikeDir.x, strikeDir.y);
    float size = glm::length(arrowDir);
    auto norm = arrowDir/size;
    size *= 0.1f;
    arrowMat = glmhelper::calcMatFromRect(
	    glm::vec4(player->getPos().x - size/2, player->getPos().y - size/2, size, size),
	    glm::degrees(atan2(norm.y, norm.x) + (22.0/14)), 1.5f);
	    

    if(input.kb.press(GLFW_KEY_SPACE)) {
	player->addVel(strikeDir*0.0001f);
	start = true;
    }
}

void Game::Draw(Render *render) {
    pm.Draw(render);
    if(!start) {
	render->DrawQuad(arrowTex, arrowMat);
    }
}



Level::Level(ResourcePool* pool, Resource::Model sphere, std::vector<Planet> planets) {
    for(auto &p: planets) {
        this->planets.push_back(
		PhysObj(pool->tex()->load("textures/circle.png"),
			sphere,
			pool->tex()->load(p.planetTex),
			p.pos, p.radius));
    }
}
