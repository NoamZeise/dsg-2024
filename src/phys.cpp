#include "phys.h"

PhysObj::PhysObj(Resource::Texture tex,
		 Resource::Model model,
		 glm::vec3 pos,
		 float radius) : Obj2D(tex), Obj3D(model) {
    this->radius = radius;
    this->pos = pos;
}

void PhysObj::addAccel(glm::vec3 accel) {
    this->accel += accel;
}

void PhysObj::Update(float dt) {
    velocity += dt*accel + bonusVel;
    bonusVel = glm::vec3(0);
    pos += dt*velocity + bonusPos;
    bonusPos = glm::vec3(0);
    this->accel = glm::vec3(0);
    Obj2D::Update(
	    glm::vec4(pos.x - radius,
		      pos.y - radius,
		      radius*2,
		      radius*2));
    Obj3D::Update(pos, radius);
}
