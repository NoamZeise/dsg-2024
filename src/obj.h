#ifndef OBJ_H
#define OBJ_H

#include <manager.h>

class Obj {
 public:
    Obj(Resource::Texture tex);
    void Update(glm::vec4 rect);
    void Draw(Render *render);
 private:
    Resource::Texture tex;
    glm::mat4 mat;
};

#endif /* OBJ_H */
