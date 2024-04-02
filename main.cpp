#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "utils.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "cube.h"

// Imgui includes
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

unsigned int viewportWidth = 800, viewportHeight = 600;
float deltaTime = 0.0f;

Camera mainCamera(viewportWidth, viewportHeight, CAMERA_PROJECTION::PERSPECTIVE);
FreeCameraController cameraController(&mainCamera);

int main()
{

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(viewportWidth, viewportHeight, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR: failed to initialize glew :(\n";
        return -1;
    }

    GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
    GLCall(glEnable(GL_DEPTH_TEST));

    // set VSync to true (study later...)
    glfwSwapInterval(1);
    // for resize window event
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    VertexArray VAO;
    // cube vertices is being imported from cube.h
    VertexBuffer VBO(cubeVertices, sizeof(cubeVertices));

    VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT); // position
    layout.Push(2, GL_FLOAT); // texture coordinates
    VAO.AddBuffer(VBO, layout);

    // IndexBuffer IBO(indices, sizeof(indices) / sizeof(unsigned int));

    Texture2D textureWood("res/images/container.jpg"), textureFace("res/images/awesomeface.png");
    textureWood.Bind(0);
    textureFace.Bind(1);

    //----------------- SHADERS -------------------//
    Shader shader("res/shaders/basic.vert", "res/shaders/basic.frag");

    // bind shader program first
    shader.Bind();
    shader.SetInt("u_TextureWood", 0);
    shader.SetInt("u_TextureFace", 1);

    //----------------- Matrices Transformations -------------------//
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f));

    shader.SetMat4("u_Model", model);
    mainCamera.SetViewProjMatrix(shader);

    //----------------- UNBIND EVERYTHING -------------------//
    shader.UnBind();
    VAO.UnBind();
    VBO.Unbind();
    // IBO.Unbind();

    //----------------- IMGUI -------------------//
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process user input
        processInput(window);
        mainCamera.SetViewport(viewportWidth, viewportHeight);

        // clear screen color
        GLCall(glClearColor(0.11f, 0.11f, 0.11f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        //----------------- IMGUI START NEW FRAME -------------------//
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //----------------- BIND EVERYTHING BEFORE A DRAW CALL -------------------//

        shader.Bind();       // bind shaders
        textureWood.Bind(0); // activate and bind texture unit (0)
        textureFace.Bind(1); // activate and bind texture unit (1)
        shader.SetMat4("u_Model", model);
        mainCamera.SetViewProjMatrix(shader);
        VAO.Bind(); // bind VAO

        // GLCall(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr));

        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

        VAO.UnBind(); // unbind VAO

        {
            ImGui::Begin("Camera");
            const glm::vec3 cameraPos = cameraController.GetCameraPos();
            const glm::vec3 cameraFront = cameraController.GetCameraFront();
            ImGui::Text("Position x: %.1f  y: %.1f  z: %.1f", cameraPos.x, cameraPos.y, cameraPos.z);
            ImGui::Text("Direction x: %.1f  y: %.1f  z: %.1f", cameraFront.x, cameraFront.y, cameraFront.z);
            ImGui::Text("Yaw %.1fº", cameraController.GetYaw());
            ImGui::Text("Pitch %.1fº", cameraController.GetPitch());
            ImGui::End();

            ImGui::Begin("FPS");
            ImGui::Text("App average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        //----------------- IMGUI RENDER -------------------//

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); // swap buffers

        glfwPollEvents(); // I/O events, mouse events etc
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    // ImGui::DestroyContext();
    ImGui::DestroyContext(nullptr);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // first parameter is not used, to avoid warning set (void)
    (void)window;
    viewportWidth = width;
    viewportHeight = height;
    // camera.SetVpWidth(viewportWidth);
    // camera.SetVpHeight(viewportHeight);
    GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //----------------- CAMERA MOVE (TRANSLATION) -------------------//

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::RIGHT, deltaTime);

    //----------------- CAMERA MOVE (ROTATION) -------------------//
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::UP_ROTATION, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::DOWN_ROTATION, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::LEFT_ROTATION, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::RIGHT_ROTATION, deltaTime);
}
