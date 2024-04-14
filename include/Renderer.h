#ifndef RENDERER_H
#define RENDERER_H

#include "utils.h"
#include "Model.h"
#include "Shader.h"

class Renderer
{
private:
public:
    Renderer(){};
    ~Renderer(){};

    // void Draw(Mesh &mesh, Shader &shader);
    void Draw(Ref<Mesh> mesh, Shader &shader);
    void DrawModel(Ref<Model> model, Shader &shader);
    void ClearColor(float red, float green, float blue, float alpha);
};
#endif
