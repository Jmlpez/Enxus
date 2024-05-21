#include "pch.h"
#include "Shader.h"
#include "utils.h"

namespace Enxus
{

    Shader::Shader(const std::string &vertexFilepath, const std::string &fragmentFilePath)
        : m_RendererId(0), m_VertexFilePath(vertexFilepath), m_FragmentFilePath(fragmentFilePath)
    {
        std::string vertexShaderSource = ParseShader(vertexFilepath);
        std::string fragmentShaderSource = ParseShader(fragmentFilePath);

        m_RendererId = CreateShader(vertexShaderSource, fragmentShaderSource);

        // bind shader program first
        GLCall(glUseProgram(m_RendererId));
    }

    Shader::~Shader()
    {
        GLCall(glDeleteProgram(m_RendererId));
    }

    void Shader::Bind() const
    {
        GLCall(glUseProgram(m_RendererId));
    }
    void Shader::Unbind() const
    {
        GLCall(glUseProgram(0));
    }

    void Shader::SetBool(const std::string &uName, bool value)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniform1i(location, (int)value));
    }
    void Shader::SetInt(const std::string &uName, int value)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniform1i(location, value));
    }

    void Shader::SetFloat(const std::string &uName, float v0)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniform1f(location, v0));
    }
    void Shader::SetFloat2(const std::string &uName, float v0, float v1)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniform2f(location, v0, v1));
    }
    void Shader::SetFloat3(const std::string &uName, float v0, float v1, float v2)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniform3f(location, v0, v1, v2));
    }
    void Shader::SetFloat4(const std::string &uName, float v0, float v1, float v2, float v3)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniform4f(location, v0, v1, v2, v3));
    }

    void Shader::SetVec2(const std::string &uName, const glm::vec2 &vec)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniform2fv(location, 1, glm::value_ptr(vec)));
    }
    void Shader::SetVec3(const std::string &uName, const glm::vec3 &vec)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniform3fv(location, 1, glm::value_ptr(vec)));
    }
    void Shader::SetMat3(const std::string &uName, const glm::mat3 &mat)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat)))
    }

    void Shader::SetMat4(const std::string &uName, const glm::mat4 &mat)
    {
        int location = GetUniformLocation(uName);
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
    }

    int Shader::GetUniformLocation(const std::string &uName)
    {
        // if it is already stored then retrieved directly
        if (m_UniformLocationCache.find(uName) != m_UniformLocationCache.end())
            return m_UniformLocationCache[uName];

        GLCall(int location = glGetUniformLocation(m_RendererId, uName.c_str()));
        if (location == -1)
        {
            std::cout << "WARNING : uniform " << uName << " does not exist" << std::endl;
        }

        m_UniformLocationCache[uName] = location;
        return location;
    }

    std::string Shader::ParseShader(const std::string &filePath)
    {

        // file handler
        std::ifstream shaderFile(filePath);
        if (!shaderFile.is_open())
        {
            std::cout << "ERROR: Cannot open " << filePath << " shader file\n";
            exit(-1);
        }

        std::stringstream shaderCodeStream;
        shaderCodeStream << shaderFile.rdbuf();

        shaderFile.close();

        std::string shaderCode = shaderCodeStream.str();
        return shaderCode;
    }

    uint32_t Shader::CompileShader(uint32_t type, const std::string &source)
    {
        uint32_t shaderId;
        GLCall(shaderId = glCreateShader(type));
        const char *src = source.c_str();
        GLCall(glShaderSource(shaderId, 1, &src, nullptr));
        GLCall(glCompileShader(shaderId));

        // error handling
        int success;
        char message[512];
        GLCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            GLCall(glGetShaderInfoLog(shaderId, 512, nullptr, message));
            std::cout << "Failed to compile shader: " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment")
                      << std::endl;
            std::cout << message << std::endl;
            GLCall(glDeleteShader(shaderId));
            return 0;
        }

        return shaderId;
    }

    uint32_t Shader::CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
    {
        uint32_t shaderProgram = glCreateProgram();

        uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
        uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        GLCall(glAttachShader(shaderProgram, vs));
        GLCall(glAttachShader(shaderProgram, fs));
        GLCall(glLinkProgram(shaderProgram));
        GLCall(glValidateProgram(shaderProgram));

        GLCall(glDeleteShader(vs));
        GLCall(glDeleteShader(fs));

        return shaderProgram;
    }
}