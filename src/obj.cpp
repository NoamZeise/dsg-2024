#include "obj.h"
#include <graphics/glm_helper.h>

Obj::Obj(Resource::Texture tex) {
    this->tex = tex;
}

void Obj::Update(glm::vec4 rect) {
    mat = glmhelper::calcMatFromRect(rect, 0, 0.5f);
}

void Obj::Draw(Render* render) {
    render->DrawQuad(tex, mat);
}
