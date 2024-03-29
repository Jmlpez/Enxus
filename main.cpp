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

// #include "glm/glm.hpp"
// #include "glm/gtc/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void printVec3(glm::vec3 &vec)
{
    std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

unsigned int viewportWidth = 800, viewportHeight = 600;

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

    Camera mainCamera(viewportWidth, viewportHeight, CAMERA_PROJECTION::PERSPECTIVE);
    FreeCameraController cameraController(&mainCamera);

    shader.SetMat4("u_Model", model);
    mainCamera.SetViewProjMatrix(shader);

    //----------------- UNBIND EVERYTHING -------------------//
    shader.UnBind();
    VAO.UnBind();
    VBO.Unbind();
    // IBO.Unbind();

    float alpha = 0.2;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process user input
        processInput(window);
        cameraController.ProcessInput(window, deltaTime);

        mainCamera.SetViewport(viewportWidth, viewportHeight);

        // clear screen color
        // GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GLCall(glClearColor(0.11f, 0.11f, 0.11f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        //----------------- BIND EVERYTHING BEFORE A DRAW CALL -------------------//

        shader.Bind();                   // bind shaders
        shader.SetFloat("alpha", alpha); // set alpha uniform

        textureWood.Bind(0); // activate and bind texture unit (0)
        textureFace.Bind(1); // activate and bind texture unit (1)

        // First Object -- on the left (perspective)
        // model = glm::rotate(model, 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
        // model and view matrix remains unchanged but generally that would not be the case
        shader.SetMat4("u_Model", model);
        mainCamera.SetViewProjMatrix(shader);

        VAO.Bind(); // bind VAO

        // GLCall(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

        // unbind VAO
        VAO.UnBind();

        // swap buffers
        glfwSwapBuffers(window);

        // I/O events, mouse events etc
        glfwPollEvents();
    }

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
}