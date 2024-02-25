#ifndef OBJ_H
#define OBJ_H

#include <manager.h>

class Obj2D {
 public:
    Obj2D(){}
    Obj2D(Resource::Texture tex);
    void Update(glm::vec4 rect);
    void Draw2D(Render *render);
 protected:
    Resource::Texture tex;
    glm::mat4 mat;
    glm::vec4 colour = glm::vec4(1.0f);
};

class Obj3D {
 public:
    Obj3D(){}
    Obj3D(Resource::Model model, Resource::Texture tex);
    void Update(glm::vec3 pos, float radius);
    void Draw3D(Render *render);
 protected:
    Resource::Model model;
    Resource::Texture tex;
    glm::mat4 mat;
    glm::mat4 norm;
};


#endif /* OBJ_H */
