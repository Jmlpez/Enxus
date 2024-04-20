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

        static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) { s_RendererInstance->SetViewportImpl(x, y, width, height); }
        static void DrawMesh(const Ref<Mesh> &mesh, const Ref<Shader> &shader) { s_RendererInstance->DrawMeshImpl(mesh, shader); }
        static void DrawModel(const Ref<Model> &model, const Ref<Shader> &shader) { s_RendererInstance->DrawModelImpl(model, shader); }
        static void ClearColor(float red, float green, float blue, float alpha) { s_RendererInstance->ClearColorImpl(red, green, blue, alpha); }

    private:
        void SetViewportImpl(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
        void DrawMeshImpl(const Ref<Mesh> &mesh, const Ref<Shader> &shader);
        void DrawModelImpl(const Ref<Model> &model, const Ref<Shader> &shader);
        void ClearColorImpl(float red, float green, float blue, float alpha);
        inline void DrawVertices(unsigned int numOfVertices);
        inline void DrawIndices(unsigned int numOfIndices);

    private:
        static Renderer *s_RendererInstance;
    };
}
#endif
