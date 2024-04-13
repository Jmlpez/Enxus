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
    void Draw(std::shared_ptr<Mesh> mesh, Shader &shader);
    void DrawModel(std::shared_ptr<Model> model, Shader &shader);
    void ClearColor(float red, float green, float blue, float alpha);
};
#endif
