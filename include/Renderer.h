#ifndef RENDERER_H
#define RENDERER_H

#include "base.h"
#include "Model.h"
#include "Shader.h"

namespace Enxus
{
    class Renderer
    {
    public:
        Renderer(){};
        ~Renderer(){};

        void DrawMesh(const Ref<Mesh> &mesh, const Ref<Shader> &shader);
        void DrawModel(const Ref<Model> &model, const Ref<Shader> &shader);
        void ClearColor(float red, float green, float blue, float alpha);

    private:
        inline void DrawVertices(unsigned int numOfVertices);
        inline void DrawIndices(unsigned int numOfIndices);
    };
}
#endif
