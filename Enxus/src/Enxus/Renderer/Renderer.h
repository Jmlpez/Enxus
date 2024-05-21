#ifndef RENDERER_H
#define RENDERER_H

#include "Base.h"
#include "Model.h"
#include "Shader.h"

namespace Enxus
{
    enum class PolygonMode
    {
        POINT, // Draw vertex positions points
        LINE,  // Draw the line joining the vertices
        FILL,  // Fill the entire shape
    };

    class Renderer
    {
    public:
        Renderer(){};
        ~Renderer(){};
        static void Init();
        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        static void SetPolygonMode(PolygonMode mode);
        static void Draw(const Ref<VertexArray> &vao, const Ref<IndexBuffer> &ibo, const Ref<Shader> &shader);
        static void DrawMesh(const Ref<Mesh> &mesh, const Ref<Shader> &shader);
        static void DrawModel(const Ref<Model> &model, const Ref<Shader> &shader);
        static void ClearColor(float red, float green, float blue, float alpha);
        static void Clear();

    private:
        static void DrawVertices(uint32_t numOfVertices);
        static void DrawIndices(uint32_t numOfIndices);
    };
}
#endif
