#ifndef GAME_H
#define GAME_H

#include "physmanager.h"
#include <game/camera.h>

class Game {
 public:
    Game(ResourcePool* pool);

    void Update(float dt, Input& input, camera::ThirdPerson &cam);
    void Draw(Render* render);
 private:

    void camUpdate(float dt, Input &input, camera::ThirdPerson &cam);
    void golfUpdate(Input &input, float dt);

    
    PhysManager pm;
    PhysObj player;
    int target = 10000;

    bool start = false;
    int playerID = 0;
    glm::vec3 strikeDir = glm::vec3(0);
    Resource::Texture arrowTex;
    glm::mat4 arrowMat;
};

struct Planet {
    std::string planetTex;
    glm::vec3 pos;
    float radius;
};

struct Level {
    Level(ResourcePool* pool, Resource::Model sphere, std::vector<Planet> planets);    
    std::vector<PhysObj> planets;
};

#endif /* GAME_H */
