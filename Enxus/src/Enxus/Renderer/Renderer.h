#ifndef RENDERER_H
#define RENDERER_H

#include "Base.h"
#include "Model.h"
#include "Shader.h"

namespace Enxus
{
    class Renderer
    {
    public:
        Renderer(){};
        ~Renderer(){};
        static void Init();
        static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
        static void DrawMesh(const Ref<Mesh> &mesh, const Ref<Shader> &shader);
        static void DrawModel(const Ref<Model> &model, const Ref<Shader> &shader);
        static void ClearColor(float red, float green, float blue, float alpha);
        static void Clear();

    private:
        static void DrawVertices(unsigned int numOfVertices);
        static void DrawIndices(unsigned int numOfIndices);
    };
}
#endif
