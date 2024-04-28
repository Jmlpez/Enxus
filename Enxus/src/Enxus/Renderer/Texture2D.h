#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

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

    struct TextureData2D
    {
        std::string path;
        TextureType type;
        TextureData2D(const std::string &_path, TextureType _type) : path(_path), type(_type) {}
        TextureData2D() : path(""), type(TextureType::DIFFUSE) {}
    };

    class Texture2D
    {

    public:
        Texture2D(const TextureData2D &textureData);
        Texture2D(const std::string &texturePath, TextureType type = TextureType::DIFFUSE);
        ~Texture2D();

        unsigned int GetRendererId() const { return m_RendererId; }

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        const std::string &GetPath() const { return m_TextureData.path; }
        unsigned int GetType() const { return m_TextureData.type; }

        void SetType(TextureType type) { m_TextureData.type = type; }

    private:
        void CreateTexture2D();
        void LoadImage();

    private:
        unsigned int m_RendererId;
        int m_Width, m_Height, m_NrChannels;
        // unsigned char *m_LocalBuffer; // texture data

        TextureData2D m_TextureData;
    };
}

#endif