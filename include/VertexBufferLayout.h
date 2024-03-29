#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include "utils.h"
#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    VertexBufferElement(unsigned int _type, unsigned int _count, unsigned char _normalized)
    {
        type = _type;
        count = _count;
        normalized = _normalized;
    }
    static unsigned int GetSizeOfType(unsigned int dataType)
    {
        switch (dataType)
        {
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_UNSIGNED_INT:
            return sizeof(unsigned int);
        case GL_INT:
            return sizeof(GLint);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLchar);
        }
        ASSERT(false); // break the program
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

private:
    bool CheckType(unsigned int type)
    {
        /*
        type checking (GL_FLOAT, GL_UNSIGNED_BYTE, GL_INT, GL_UNSIGNED_INT)
        */
        const unsigned int VALID_TYPES[4] = {GL_FLOAT, GL_UNSIGNED_BYTE, GL_INT, GL_UNSIGNED_INT};

        for (int i = 0; i < 4; i++)
            if (type == VALID_TYPES[i])
                return true;
        return false;
    }

public:
    VertexBufferLayout() : m_Stride(0) {}

    void Push(unsigned int count, unsigned int type)
    {
        ASSERT(CheckType(type))
        m_Elements.push_back({type, count, GL_FALSE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(type);
    }
    inline const std::vector<VertexBufferElement> &GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};

#endif
