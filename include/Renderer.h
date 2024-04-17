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

        // void Draw(Mesh &mesh, Shader &shader);
        void Draw(Ref<Mesh> mesh, Shader &shader);
        void DrawModel(Ref<Model> model, Shader &shader);
        void ClearColor(float red, float green, float blue, float alpha);

    private:
        inline void DrawVertices(unsigned int numOfVertices);
        inline void DrawIndices(unsigned int numOfIndices);
    };
}
#endif
