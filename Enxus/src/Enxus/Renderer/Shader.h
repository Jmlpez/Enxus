#ifndef SHADER_H
#define SHADER_H

#include "Enxus/Core/Base.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Enxus
{

    class Shader
    {

    public:
        ~Shader();
        Shader(const std::string &vertexFilePath, const std::string &fragmentFilePath);

        void Bind() const;
        void Unbind() const;

        void SetBool(const std::string &uName, bool value);
        void SetInt(const std::string &uName, int value);
        void SetFloat(const std::string &uName, float v0);
        void SetFloat2(const std::string &uName, float v0, float v1);
        void SetFloat3(const std::string &uName, float v0, float v1, float v3);
        void SetFloat4(const std::string &uName, float v0, float v1, float v2, float v3);
        // missing
        void SetVec2(const std::string &uName, const glm::vec2 &vec);
        void SetVec3(const std::string &uName, const glm::vec3 &vec);
        void SetMat3(const std::string &uName, const glm::mat3 &mat);
        void SetMat4(const std::string &uName, const glm::mat4 &mat);

    private:
        // get the location index of the uniforms in the shader program
        int GetUniformLocation(const std::string &uName);

        // Parse the shader code from a file
        std::string ParseShader(const std::string &filePath);

        // Compile shader source code
        uint32_t CompileShader(uint32_t type, const std::string &source);

        // Create a shader program ready to be use with vertex and fragment shader attached
        uint32_t CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
        // consider add geometry shader with same template
        // uint32_t CreateShader(const std::string &vertexShaderSource, const std::string &geometryShaderSource,const std::string &fragmentShaderSource);
    private:
        uint32_t m_RendererId;
        // to store the location of the uniforms
        std::unordered_map<std::string, int> m_UniformLocationCache;
        // debugging purposes
        std::string m_VertexFilePath, m_FragmentFilePath;
    };

}
#endif