#ifndef GAME_H
#define GAME_H

#include "physmanager.h"
#include <game/camera.h>

struct Planet {
    glm::vec3 pos;
    float radius;
    glm::vec3 vel = glm::vec3(0);
};

struct Level {
    Level(){}
    Level(Resource::Texture planetTex2D, std::vector<Resource::Texture> &planetTexs, Resource::Model sphere, std::vector<Planet> planets);
    std::vector<PhysObj> planets;
};

class Game {
 public:
    Game(ResourcePool* pool);

    glm::mat4 Update(float dt, Input& input, camera::ThirdPerson &cam);
    void Draw(Render* render);
    void Reset();
    bool gameFinished() { return isFinished; }
    bool isLevelDone() { return levelDone; }
    void NextLevel();
 private:

    void camUpdate(float dt, Input &input, camera::ThirdPerson &cam);
    void golfUpdate(Input &input, float dt);

    void loadLevel(Level level);

    void nextLevel();
    void nextLevelRandom();
    
    PhysManager pm;
    PhysObj player;
    int target = 10000;

    bool isFinished = false;
    bool levelDone = false;
    bool start = false;

    bool randomMode = false;
    int playerID = 0;
    glm::vec3 strikeDir = glm::vec3(0);
    Resource::Texture arrowTex;
    glm::mat4 arrowMat;

    Resource::Model sphere;

    Resource::Texture circle;
    std::vector<Resource::Texture> planetSurfaceTex;

    int currentLevelNum = 0;
    Level currentLevel;
};

#endif /* GAME_H */
