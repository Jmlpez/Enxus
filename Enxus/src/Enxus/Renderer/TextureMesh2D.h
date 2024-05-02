#ifndef TEXTURE_MESH_2D_H
#define TEXTURE_MESH_2D_H

#include "Base.h"

namespace Enxus
{

    enum TextureType
    {
        DIFFUSE,
        SPECULAR,
        NORMAL,
        // EMISSION,
        // Albedo, Metallic, Roughness etc
    };

    struct TextureMesh2DData
    {
        std::string path;
        TextureType type;
        TextureMesh2DData(const std::string &_path, TextureType _type) : path(_path), type(_type) {}
        TextureMesh2DData() : path(""), type(TextureType::DIFFUSE) {}
    };

    class TextureMesh2D
    {

    public:
        TextureMesh2D(const TextureMesh2DData &textureData);
        TextureMesh2D(const std::string &texturePath, TextureType type = TextureType::DIFFUSE);
        ~TextureMesh2D();

        unsigned int GetRendererId() const { return m_RendererId; }

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        const std::string &GetPath() const { return m_TextureData.path; }
        unsigned int GetType() const { return m_TextureData.type; }

        void SetType(TextureType type) { m_TextureData.type = type; }

    private:
        void CreateTexture();
        void LoadImage();

    private:
        unsigned int m_RendererId;
        int m_Width, m_Height, m_NrChannels;
        // unsigned char *m_LocalBuffer; // texture data

        TextureMesh2DData m_TextureData;
    };
}

#endif