#ifndef PHYSMANAGER_H
#define PHYSMANAGER_H

#include "phys.h"
#include <vector>

class PhysManager {
 public:
    PhysManager();
    void Update(float dt);
    void Draw(Render* render);
    void Add(PhysObj obj);

    glm::vec3 getPos(int id);
 private:
    std::vector<PhysObj> objs;
    float G = 0.00001;
};

#endif /* PHYSMANAGER_H */
