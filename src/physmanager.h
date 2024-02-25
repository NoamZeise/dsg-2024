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

    PhysObj* getObj(int id);
    PhysObj* nearestObj() {
	return nearest;
    }
    int Count() { return objs.size(); }
    void clear() { objs.clear(); }

    bool playerTouchedGoal() {
	return touchedGoal;
    }
 private:
    std::vector<PhysObj> objs;
    PhysObj* nearest = nullptr;
    float G = 0.00001;
    bool touchedGoal = false;
};

#endif /* PHYSMANAGER_H */
