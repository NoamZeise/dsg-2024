#ifndef PHYS_H
#define PHYS_H

#include "obj.h"

class PhysObj : public Obj2D, public Obj3D {
 public:
    PhysObj(){}
    PhysObj(Resource::Texture tex,
	    Resource::Model model,
	    Resource::Texture modelTex,
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
    float getMass() { return radius*radius*weight; }
    float getRadius() { return radius; }
    glm::vec3 getVel() { return velocity; }
    void setWeight(float weight) {
	this->weight = weight;
    }
 private:
    glm::vec3 accel = glm::vec3(0);
    glm::vec3 velocity = glm::vec3(0);
    glm::vec3 bonusVel = glm::vec3(0);
    glm::vec3 pos = glm::vec3(0);
    glm::vec3 bonusPos = glm::vec3(0);
    float weight = 1.0f;
    float radius;
};


#endif /* PHYS_H */
