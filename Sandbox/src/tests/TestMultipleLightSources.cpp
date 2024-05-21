#include "TestMultipleLightSources.h"

namespace OpenGLTest
{
    TestMultipleLightSources::TestMultipleLightSources()
    {
        float cubeVertexPositions[] =
            {
                // FACE
                -0.5f, -0.5f, -0.5f, /*V*/
                0.5f, -0.5f, -0.5f,  /*V*/
                0.5f, 0.5f, -0.5f,   /*V*/
                0.5f, 0.5f, -0.5f,   /*V*/
                -0.5f, 0.5f, -0.5f,  /*V*/
                -0.5f, -0.5f, -0.5f, /*V*/
                // FACE
                -0.5f, -0.5f, 0.5f, /*V*/
                0.5f, -0.5f, 0.5f,  /*V*/
                0.5f, 0.5f, 0.5f,   /*V*/
                0.5f, 0.5f, 0.5f,   /*V*/
                -0.5f, 0.5f, 0.5f,  /*V*/
                -0.5f, -0.5f, 0.5f, /*V*/
                // FACE
                -0.5f, 0.5f, 0.5f,   /*V*/
                -0.5f, 0.5f, -0.5f,  /*V*/
                -0.5f, -0.5f, -0.5f, /*V*/
                -0.5f, -0.5f, -0.5f, /*V*/
                -0.5f, -0.5f, 0.5f,  /*V*/
                -0.5f, 0.5f, 0.5f,   /*V*/
                // FACE
                0.5f, 0.5f, 0.5f,   /*V*/
                0.5f, 0.5f, -0.5f,  /*V*/
                0.5f, -0.5f, -0.5f, /*V*/
                0.5f, -0.5f, -0.5f, /*V*/
                0.5f, -0.5f, 0.5f,  /*V*/
                0.5f, 0.5f, 0.5f,   /*V*/
                // FACE
                -0.5f, -0.5f, -0.5f, /*V*/
                0.5f, -0.5f, -0.5f,  /*V*/
                0.5f, -0.5f, 0.5f,   /*V*/
                0.5f, -0.5f, 0.5f,   /*V*/
                -0.5f, -0.5f, 0.5f,  /*V*/
                -0.5f, -0.5f, -0.5f, /*V*/
                // FACE
                -0.5f, 0.5f, -0.5f, /*V*/
                0.5f, 0.5f, -0.5f,  /*V*/
                0.5f, 0.5f, 0.5f,   /*V*/
                0.5f, 0.5f, 0.5f,   /*V*/
                -0.5f, 0.5f, 0.5f,  /*V*/
                -0.5f, 0.5f, -0.5f,
                /*V*/
            };
        float cubeTexCoords[] = {
            0.0f, 0.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            1.0f, 1.0f, /*Tex Coords*/
            1.0f, 1.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
            0.0f, 0.0f, /*Tex Coords*/
            0.0f, 0.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            1.0f, 1.0f, /*Tex Coords*/
            1.0f, 1.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
            0.0f, 0.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            1.0f, 1.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
            0.0f, 0.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            1.0f, 1.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
            0.0f, 0.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
            1.0f, 1.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            0.0f, 0.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
            1.0f, 1.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            1.0f, 0.0f, /*Tex Coords*/
            0.0f, 0.0f, /*Tex Coords*/
            0.0f, 1.0f, /*Tex Coords*/
        };
        float cubeNormals[] = {
            0.0f, 0.0f, -1.0f, /* Normal */
            0.0f, 0.0f, -1.0f, /* Normal */
            0.0f, 0.0f, -1.0f, /* Normal */
            0.0f, 0.0f, -1.0f, /* Normal */
            0.0f, 0.0f, -1.0f, /* Normal */
            0.0f, 0.0f, -1.0f, /* Normal */
            0.0f, 0.0f, 1.0f,  /* Normal */
            0.0f, 0.0f, 1.0f,  /* Normal */
            0.0f, 0.0f, 1.0f,  /* Normal */
            0.0f, 0.0f, 1.0f,  /* Normal */
            0.0f, 0.0f, 1.0f,  /* Normal */
            0.0f, 0.0f, 1.0f,  /* Normal */
            -1.0f, 0.0f, 0.0f, /* Normal */
            -1.0f, 0.0f, 0.0f, /* Normal */
            -1.0f, 0.0f, 0.0f, /* Normal */
            -1.0f, 0.0f, 0.0f, /* Normal */
            -1.0f, 0.0f, 0.0f, /* Normal */
            -1.0f, 0.0f, 0.0f, /* Normal */
            1.0f, 0.0f, 0.0f,  /* Normal */
            1.0f, 0.0f, 0.0f,  /* Normal */
            1.0f, 0.0f, 0.0f,  /* Normal */
            1.0f, 0.0f, 0.0f,  /* Normal */
            1.0f, 0.0f, 0.0f,  /* Normal */
            1.0f, 0.0f, 0.0f,  /* Normal */
            0.0f, -1.0f, 0.0f, /* Normal */
            0.0f, -1.0f, 0.0f, /* Normal */
            0.0f, -1.0f, 0.0f, /* Normal */
            0.0f, -1.0f, 0.0f, /* Normal */
            0.0f, -1.0f, 0.0f, /* Normal */
            0.0f, -1.0f, 0.0f, /* Normal */
            0.0f, 1.0f, 0.0f,  /* Normal */
            0.0f, 1.0f, 0.0f,  /* Normal */
            0.0f, 1.0f, 0.0f,  /* Normal */
            0.0f, 1.0f, 0.0f,  /* Normal */
            0.0f, 1.0f, 0.0f,  /* Normal */
            0.0f, 1.0f, 0.0f,  /* Normal */
        };
        objVAO = Enxus::CreateRef<Enxus::VertexArray>();
        lightSourceVAO = Enxus::CreateRef<Enxus::VertexArray>();

        Enxus::Ref<Enxus::VertexBuffer> boxVertexPositionBuffer = Enxus::CreateRef<Enxus::VertexBuffer>(cubeVertexPositions, sizeof(cubeVertexPositions));
        Enxus::Ref<Enxus::VertexBuffer> boxTexCoordsBuffer = Enxus::CreateRef<Enxus::VertexBuffer>(cubeTexCoords, sizeof(cubeTexCoords));
        Enxus::Ref<Enxus::VertexBuffer> boxNormalsBuffer = Enxus::CreateRef<Enxus::VertexBuffer>(cubeNormals, sizeof(cubeNormals));

        Enxus::BufferLayout boxVertexPositionLayout = {
            {Enxus::ShaderDataType::Float3, "aPos"},
        };
        Enxus::BufferLayout boxTexCoordsLayout = {
            {Enxus::ShaderDataType::Float2, "aTexCoord"},
        };
        Enxus::BufferLayout boxNormalsLayout = {
            {Enxus::ShaderDataType::Float3, "aNormal"},
        };
        boxVertexPositionBuffer->SetLayout(boxVertexPositionLayout);
        boxTexCoordsBuffer->SetLayout(boxTexCoordsLayout);
        boxNormalsBuffer->SetLayout(boxNormalsLayout);

        objVAO->AddVertexBuffer(boxVertexPositionBuffer);
        objVAO->AddVertexBuffer(boxTexCoordsBuffer);
        objVAO->AddVertexBuffer(boxNormalsBuffer);

        Enxus::Ref<Enxus::VertexBuffer> ligthSourceVertexBuffer = Enxus::CreateRef<Enxus::VertexBuffer>(cubeVertexPositions, sizeof(cubeVertexPositions));
        Enxus::BufferLayout lightSourceLayout = {
            {Enxus::ShaderDataType::Float3, "aPos"},
        };

        ligthSourceVertexBuffer->SetLayout(lightSourceLayout);
        lightSourceVAO->AddVertexBuffer(ligthSourceVertexBuffer);

        // //----------------- TEXTURES -------------------//

        containerDiffuse = Enxus::CreateRef<Enxus::TextureMesh2D>("Sandbox/res/images/container2.png");
        containerSpecular = Enxus::CreateRef<Enxus::TextureMesh2D>("Sandbox/res/images/container2_specular.png");

        // //----------------- SHADERS -------------------//

        objShader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/basic-lighting/obj.vert", "Sandbox/res/shaders/basic-lighting/obj.frag");
        lightSourceShader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/basic-lighting/light.vert", "Sandbox/res/shaders/basic-lighting/light.frag");

        // // bind shader program first

        // //----------------- Light Source Matrices Transformations -------------------//

        lightSourceShader->Bind();
        // mainCamera->SetViewProjMatrix(*lightSourceShader);
        glm::mat4 lightSourceModel = glm::mat4(1.0f);
        glm::vec3 lightSourcePos = glm::vec3(1.2f, 1.0f, 2.0f);
        //    for directional light
        // glm::vec3 lightSourceDir = glm::vec3(-0.2f, -1.0f, -0.3f);

        lightSourceModel = glm::translate(lightSourceModel, lightSourcePos);
        lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.2f));

        // Basic shader uniforms: MVP matrix
        // send camera uniforms to the light  shader
        lightSourceShader->SetMat4("uModel", lightSourceModel);
        // mainCamera->SetViewProjMatrix(*lightSourceShader);

        //----------------- Object Matrices Transformations -------------------//

        // glm::mat4 objModel = glm::mat4(1.0f);

        objShader->Bind();

        // //----------------- OBJECT MATERIAL -------------------//
        objShader->SetFloat("uObjectMaterial.shininess", 32.0f);
        objShader->SetInt("uObjectMaterial.diffuse", 0);
        objShader->SetInt("uObjectMaterial.specular", 1);

        containerDiffuse->Bind(0);
        containerSpecular->Bind(1);

        // //----------------- LIGHT MATERIAL (Affecting the object, not the material of the light source) -------------------//

        // // DIRECTIONAL LIGHTS
        objShader->SetFloat3("uDirLight.direction", -0.2f, -1.0f, -0.3f);
        objShader->SetFloat3("uDirLight.ambient", 0.1f, 0.1f, 0.1f);
        objShader->SetFloat3("uDirLight.diffuse", 1.0f, 1.0f, 1.0f);
        objShader->SetFloat3("uDirLight.specular", 1.0f, 1.0f, 1.0f);

        // // POINT LIGHTS UNIFORMS

        for (int i = 0; i < 4; i++)
        {
            std::string index = std::to_string(i);
            objShader->SetVec3("uPointLights[" + index + "].position", pointLightPositions[i]);
            objShader->SetFloat3("uPointLights[" + index + "].ambient", 0.1f, 0.1f, 0.1f);
            objShader->SetFloat3("uPointLights[" + index + "].diffuse", 1.0f, 1.0f, 1.0f);
            objShader->SetFloat3("uPointLights[" + index + "].specular", 1.0f, 1.0f, 1.0f);
            // attenuation values
            objShader->SetFloat("uPointLights[" + index + "].constant", 1.0f);
            objShader->SetFloat("uPointLights[" + index + "].linear", 0.09f);
            objShader->SetFloat("uPointLights[" + index + "].quadratic", 0.032f);
        }

        // // SPOT LIGHT UNIFORMS
        // objShader->SetVec3("uSpotLight.position", cameraController->GetCameraPos()); // to spotlight
        // objShader->SetVec3("uSpotLight.direction", cameraController->GetCameraFront());
        objShader->SetFloat("uSpotLight.innerCutOffAngle", glm::cos(glm::radians(12.5f)));
        objShader->SetFloat("uSpotLight.outerCutOffAngle", glm::cos(glm::radians(17.5f)));
        objShader->SetFloat3("uSpotLight.diffuse", 1.0f, 1.0f, 1.0f);
        objShader->SetFloat3("uSpotLight.specular", 1.0f, 1.0f, 1.0f);

        // objShader->SetVec3("uCameraPos", cameraController->GetCameraPos());
        //  send camera uniforms to the object shader
        //  objShader->SetMat4("uModel", objModel);
        // mainCamera->SetViewProjMatrix(*objShader);

        // //----------------- UNBIND EVERYTHING -------------------//
        objShader->Unbind();
        objVAO->Unbind();
        // objVBO->Unbind();
        lightSourceShader->Unbind();
        lightSourceVAO->Unbind();
    }

    TestMultipleLightSources::~TestMultipleLightSources()
    {
    }

    void TestMultipleLightSources::OnUpdate(Enxus::Camera &camera)
    {

        objShader->Bind();
        objShader->SetVec3("uSpotLight.position", camera.GetPos());
        objShader->SetVec3("uSpotLight.direction", camera.GetFront());

        // send camera pos to specular light calculation
        objShader->SetVec3("uCameraPos", camera.GetPos());
        // cameraController->GetCamera()->SetViewProjMatrix(*objShader);
        objShader->SetMat4("uView", camera.GetViewMatrix());
        objShader->SetMat4("uProj", camera.GetProjectionMatrix());

        objShader->Unbind();
        lightSourceShader->Bind();
        lightSourceShader->SetMat4("uView", camera.GetViewMatrix());
        lightSourceShader->SetMat4("uProj", camera.GetProjectionMatrix());

        {
            objShader->Bind();
            objVAO->Bind(); // bind VAO
            for (int i = 0; i < 10; i++)
            {
                glm::mat4 cubeModel = glm::mat4(1.0f);
                cubeModel = glm::translate(cubeModel, cubePositions[i]);
                float angle = 20.0f * i;
                cubeModel = glm::rotate(cubeModel, glm::radians(angle),
                                        glm::vec3(1.0f, 0.3f, 0.5f));
                objShader->SetMat4("uModel", cubeModel);
                GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
            }

            objVAO->Unbind(); // unbind VAO
        }
        //----------------- RENDER LIGHTS SOURCES -------------------//

        {
            lightSourceShader->Bind();
            // mainCamera->SetViewProjMatrix(*lightSourceShader);

            lightSourceVAO->Bind();
            for (int i = 0; i < 4; i++)
            {
                glm::mat4 lightSourceModel = glm::mat4(1.0f);
                lightSourceModel = glm::translate(lightSourceModel, pointLightPositions[i]);
                lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.2f));
                lightSourceShader->SetMat4("uModel", lightSourceModel);

                GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
            }
            lightSourceVAO->Unbind();
        }
    }
    void TestMultipleLightSources::OnImGuiRender()
    {
    }
}