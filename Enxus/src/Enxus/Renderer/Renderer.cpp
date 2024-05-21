#include "pch.h"
#include "Renderer.h"
#include "utils.h"

namespace Enxus
{

    void Renderer::Init()
    {
        //----------------- OpenGL State Initialization -------------------//

        // Blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Depth testing
        GLCall(glEnable(GL_DEPTH_TEST));

        Renderer::SetPolygonMode(PolygonMode::FILL);
    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        GLCall(glViewport(x, y, width, height));
    }

    void Renderer::ClearColor(float red, float green, float blue, float alpha)
    {
        GLCall(glClearColor(red, green, blue, alpha));
    }
    void Renderer::Clear()
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void Renderer::SetPolygonMode(PolygonMode mode)
    {
        switch (mode)
        {
        case PolygonMode::POINT:
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_POINT));
            return;
        case PolygonMode::LINE:
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
            return;
        case PolygonMode::FILL:
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
            return;
        default:
            break;
        }
        std::cout << "[OpenGL Error] Invalid polygon mode" << std::endl;
        ASSERT(false);
    }

    void Renderer::Draw(const Ref<VertexArray> &vao, const Ref<IndexBuffer> &ibo, const Ref<Shader> &shader)
    {
        vao->Bind();
        // ibo->Bind();
        shader->Bind();

        Renderer::DrawIndices(vao->GetIndexBuffer()->GetCount());

        vao->Unbind();
        // ibo->Unbind();
        shader->Unbind();
    }

    void Renderer::DrawMesh(const Ref<Mesh> &mesh, const Ref<Shader> &shader)
    {

        mesh->GetVertexArray()->Bind();
        shader->Bind();

        /*
            Currently supported textures types
            diffuse map
            specular map
        */

        unsigned diffuseNr = 1;
        unsigned specularNr = 1;

        auto textures = mesh->GetTextures();

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            auto &texture = textures[i];
            std::string number, name;
            if (texture->GetType() == TextureType::DIFFUSE)
            {
                name = "texture_diffuse" + std::to_string(diffuseNr++);
            }
            else if (texture->GetType() == TextureType::SPECULAR)
            {
                name = "texture_specular" + std::to_string(specularNr++);
            }
            shader->SetInt(("material." + name).c_str(), i);
            texture->Bind(i); // active the current texture slot
        }

        mesh->GetVertexArray()->Bind();
        if (mesh->HasIndices())
        {
            Renderer::DrawIndices(mesh->GetVertexArray()->GetIndexBuffer()->GetCount());
        }
        else
        {
            Renderer::DrawVertices(mesh->GetVertices().size());
        }

        mesh->GetVertexArray()->Unbind();
        mesh->GetIndexBuffer()->Unbind();
    }

    void Renderer::DrawModel(const Ref<Model> &model, const Ref<Shader> &shader)
    {
        auto meshes = model->GetMeshes();
        for (auto mesh : meshes)
        {
            DrawMesh(mesh, shader);
        }
    }
    inline void Renderer::DrawVertices(uint32_t numOfVertices)
    {
        GLCall(glDrawArrays(GL_TRIANGLES, 0, numOfVertices));
    }
    inline void Renderer::DrawIndices(uint32_t numOfIndices)
    {
        GLCall(glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, nullptr));
    }
}