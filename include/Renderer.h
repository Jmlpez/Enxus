#ifndef RENDERER_H
#define RENDERER_H

#include "utils.h"
#include "Mesh.h"
#include "Shader.h"

class Renderer
{
private:
public:
    Renderer(){};
    ~Renderer(){};

    void Draw(Mesh &mesh, Shader &shader);
};
#endif
