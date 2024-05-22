#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "Base.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace Enxus
{

    class SkyBox
    {
    public:
        SkyBox();
        ~SkyBox();

        void Bind() const;
        void Unbind() const;

        /*
        The path must be absolute (relative to the project root)
        Order and Orientation:
        Right (+X), Left (-X)
        Up (+Y), Down (-Y)
        Front (+Z), Back (-Z)
        */
        void SetCubeMapFaces(const std::array<std::string, 6> &faces);

        inline Ref<VertexArray> GetVertexArray() const { return m_SkyBoxVAO; }

    private:
        void LoadCubeMap();

        uint32_t m_RendererID;

        Ref<VertexArray> m_SkyBoxVAO;
        Ref<VertexBuffer> m_SkyBoxVBO;

        std::array<std::string, 6> m_CubeMapFaces;
    };
}

#endif