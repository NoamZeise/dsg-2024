#ifndef OBJ_H
#define OBJ_H

#include <manager.h>

class Obj2D {
 public:
    Obj2D(Resource::Texture tex);
    void Update(glm::vec4 rect);
    void Draw2D(Render *render);
 private:
    Resource::Texture tex;
    glm::mat4 mat;
};

class Obj3D {
 public:
    Obj3D(Resource::Model model);
    void Update(glm::vec3 pos, float radius);
    void Draw3D(Render *render);
 private:
    Resource::Model model;
    glm::mat4 mat;
    glm::mat4 norm;
};


#endif /* OBJ_H */
