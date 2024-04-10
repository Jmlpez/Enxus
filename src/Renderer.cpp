#include "Renderer.h"

void Renderer::Draw(Mesh &mesh, Shader &shader)
{

    mesh.GetVAO()->Bind();
    shader.Bind();

    /*
        Currently supported textures types
        diffuse map
        specular map
    */
    unsigned diffuseNr = 1;
    unsigned specularNr = 1;

    auto textures = mesh.GetTextures();

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        auto &texture = textures[i];
        std::string number, name;
        if (texture->GetType() == TEXTURE_TYPE::DIFFUSE)
        {
            name = "texture_diffuse" + std::to_string(diffuseNr++);
        }
        else if (texture->GetType() == TEXTURE_TYPE::SPECULAR)
        {
            name = "texture_specular" + std::to_string(specularNr++);
        }
        shader.SetInt(("material." + name).c_str(), i);
        texture->Bind(i); // active the current texture slot
    }

    mesh.GetVAO()->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr));

    mesh.GetVAO()->Unbind();
    mesh.GetIBO()->Unbind();
}
