#include "physmanager.h"

PhysManager::PhysManager() {}


void PhysManager::Add(PhysObj obj) {
    objs.push_back(obj);
}

void PhysManager::Update(float dt) {
    float nearestToPlayer = 0;
    for(int i = 0; i < objs.size(); i++) {
	for(int j = i + 1; j < objs.size(); j++) {
	    glm::vec3 d = objs[i].getPos() - objs[j].getPos();
	    float r = glm::dot(d, d);
	    float m1 = objs[i].getMass();
	    float m2 = objs[j].getMass();
	    objs[i].addAccel((G*m2/r)*(-d));
	    objs[j].addAccel((G*m1/r)*( d));
	    
	    float min = objs[i].getRadius() + objs[j].getRadius();
	    float diff2 = min*min  - r;

	    if(i == 0) {
		if(nearestToPlayer == 0 ||
		   nearestToPlayer > r) {
		    nearest = &objs[j];
		    nearestToPlayer = r;
		}		
	    }
	    
	    if(diff2 > 0) {
		float diff = glm::length(d);
		float x = min - diff;
		glm::vec3 n = d/diff;
		float M = m1+m2;
		
		float mr = x/M;		
		objs[i].addPos(m2*mr*( n));
		objs[j].addPos(m1*mr*(-n));
		
		glm::vec3 u1 = glm::dot(objs[i].getVel(), -n)*(-n);
		glm::vec3 u2 = glm::dot(objs[j].getVel(), n)*(n);
		glm::vec3 v1 = ((m1 - m2)*u1 + 2*m2*u2)/M;
		glm::vec3 v2 = (2*m1*u1 + (m2 - m1)*u2)/M;
		objs[i].addVel(v1 - u1);
		objs[j].addVel(v2 - u2);
	    }
	}
	objs[i].Update(dt);
    }
}

void PhysManager::Draw(Render* render) {
    for(auto &obj: objs)
	obj.Draw3D(render);
    for(auto &obj: objs)
    	obj.Draw2D(render);
}


PhysObj* PhysManager::getObj(int id) {
    if(id >= objs.size() || id < 0)
	return nullptr;
    return &objs[id];
}
