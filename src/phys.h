#ifndef PHYS_H
#define PHYS_H

#include "obj.h"

class PhysObj : public Obj {
 public:
    PhysObj(Resource::Texture tex,
	    glm::vec3 pos,
	    float radius);
    void addAccel(glm::vec3 accel);
    void addVel(glm::vec3 vel) {
	this->bonusVel += vel;
    }
    void addPos(glm::vec3 pos) {
	this->bonusPos = pos;
    }
    void Update(float dt);
    
    glm::vec3 getPos() { return pos; }
    float getSize() { return radius*radius*weight; }
    float getRadius() { return radius; }
    glm::vec3 getVel() { return velocity; }
 private:
    glm::vec3 accel = glm::vec3(0);
    glm::vec3 velocity = glm::vec3(0);
    glm::vec3 bonusVel = glm::vec3(0);
    glm::vec3 pos = glm::vec3(0);
    glm::vec3 bonusPos = glm::vec3(0);
    float radius;
    float weight = 1.0f;
};


#endif /* PHYS_H */
