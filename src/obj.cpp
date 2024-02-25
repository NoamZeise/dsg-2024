#include "obj.h"
#include <graphics/glm_helper.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <graphics/logger.h>

Obj2D::Obj2D(Resource::Texture tex) {
    this->tex = tex;
}

void Obj2D::Update(glm::vec4 rect) {
    mat = glmhelper::calcMatFromRect(rect, 0, 0.5f);
}

void Obj2D::Draw2D(Render *render) { render->DrawQuad(tex, mat, colour); }

Obj3D::Obj3D(Resource::Model model, Resource::Texture tex) {
    this->model = model;
    this->tex = tex;
}

void Obj3D::Update(glm::vec3 pos, float radius) {
    mat =  glm::scale(
	    glm::translate(glm::mat4(1.0f), pos),
	    glm::vec3(radius));
	   
    norm = glm::inverseTranspose(mat);
}

void Obj3D::Draw3D(Render *render) {
    render->DrawModel(model, mat, norm, glm::vec4(1.0f),
		      &tex);
}
