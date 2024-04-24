#include "pch.h"
#include "Renderer.h"
#include "utils.h"

namespace Enxus
{

    void Renderer::Init()
    {
        //----------------- OpenGL State Initialization -------------------//

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        GLCall(glEnable(GL_DEPTH_TEST));
    }

    void Renderer::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
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

    void Renderer::DrawMesh(const Ref<Mesh> &mesh, const Ref<Shader> &shader)
    {

        mesh->GetVAO()->Bind();
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

        mesh->GetVAO()->Bind();
        if (mesh->HasIndices())
        {
            Renderer::DrawIndices(mesh->GetIBO()->GetCount());
        }
        else
        {
            Renderer::DrawVertices(mesh->GetVertices().size());
        }

        mesh->GetVAO()->Unbind();
        mesh->GetIBO()->Unbind();
    }

    void Renderer::DrawModel(const Ref<Model> &model, const Ref<Shader> &shader)
    {
        auto meshes = model->GetMeshes();
        for (auto mesh : meshes)
        {
            DrawMesh(mesh, shader);
        }
    }
    inline void Renderer::DrawVertices(unsigned int numOfVertices)
    {
        GLCall(glDrawArrays(GL_TRIANGLES, 0, numOfVertices));
    }
    inline void Renderer::DrawIndices(unsigned int numOfIndices)
    {
        GLCall(glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, nullptr));
    }
}