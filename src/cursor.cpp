#include "cursor.h"

Cursor::Cursor(Resource::Texture tex, glm::vec2 clamp) : Obj2D(tex) {
    this->clamp = clamp;
    this->colour = glm::vec4(0.4, 0.2, 0.1f, 1.0f);
}

void Cursor::Update(Input& input) {
    float s = 0.7f;
    pos.x += s*input.m.dx();
    pos.y += s*input.m.dy();
    if(pos.x < 0)
	pos.x = 0;
    if(pos.y < 0)
	pos.y = 0;
    if(pos.x > clamp.x - size)
	pos.x = clamp.x - size;
    if(pos.y > clamp.y - size)
	pos.y = clamp.y - size;
    Obj2D::Update(glm::vec4(pos.x, pos.y, size, size));
}
