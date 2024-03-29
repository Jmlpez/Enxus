#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <string>

class Texture2D
{
private:
    unsigned int m_RendererId;
    int m_Width, m_Height, m_NrChannels;
    unsigned char *m_LocalBuffer; // texture data
    // debugging purposes
    std::string m_TexturePath;

public:
    Texture2D(const std::string &texturePath);
    ~Texture2D();

    void Bind(unsigned int slot = 0) const;
    void UnBind() const;

private:
    void LoadImage();
};

#endif