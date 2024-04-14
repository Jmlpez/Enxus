#include "Renderer.h"
#include <iostream>

void Renderer::ClearColor(float red, float green, float blue, float alpha)
{
    GLCall(glClearColor(red, green, blue, alpha));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(Ref<Mesh> mesh, Shader &shader)
{

    mesh->GetVAO()->Bind();
    shader.Bind();

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
        shader.SetInt(("material." + name).c_str(), i);
        texture->Bind(i); // active the current texture slot
    }

    mesh->GetVAO()->Bind();
    if (mesh->HasIndices())
    {
        GLCall(glDrawElements(GL_TRIANGLES, mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr));
    }
    else
    {
        GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertices().size()));
    }

    mesh->GetVAO()->Unbind();
    mesh->GetIBO()->Unbind();
}

void Renderer::DrawModel(Ref<Model> model, Shader &shader)
{
    // Draw(*model.GetMeshes()[1], shader);
    auto meshes = model->GetMeshes();
    for (auto mesh : meshes)
    {
        Draw(mesh, shader);
    }
}