#ifndef CURSOR_H
#define CURSOR_H

#include "obj.h"

class Cursor: public Obj2D {
 public:
    Cursor(Resource::Texture tex, glm::vec2 clamp);
    void Update(Input& input);
 private:
    Resource::Texture tex;
    glm::mat4 mat;

    glm::vec2 pos = glm::vec2(150, 150);

    glm::vec2 clamp;
    float size = 20;
};

#endif /* CURSOR_H */
