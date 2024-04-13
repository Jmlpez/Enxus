#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

// utility functions
#include "utils.h"

// Imgui includes
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Test
#include "tests/TestMenu.h"
#include "tests/TestClearColor.h"
#include "tests/TestMesh.h"
#include "tests/TestModel.h"
#include "tests/TestDepthBuffer.h"
// #include "tests/TestMultipleLightSources.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

unsigned int viewportWidth = 1366, viewportHeight = 768;
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

    // //----------------- IMGUI -------------------//
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Test::Test *currentTest = nullptr;
    Test::TestMenu *testMenu = new Test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<Test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<Test::TestMesh>("Mesh");
    testMenu->RegisterTest<Test::TestModel>("Model Loading");
    testMenu->RegisterTest<Test::TestDepthBuffer>("Depth Testing");

    currentTest = new Test::TestDepthBuffer;

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process user input
        processInput(window);
        mainCamera.OnResize(viewportWidth, viewportHeight);

        // clear screen color
        GLCall(glClearColor(0.11f, 0.11f, 0.11f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        //----------------- IMGUI START NEW FRAME -------------------//
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest)
        {
            currentTest->OnUpdate(deltaTime, &cameraController);
            currentTest->OnRender();
            ImGui::Begin("Test");

            if (currentTest != testMenu && ImGui::Button("<--"))
            {
                delete currentTest;
                currentTest = testMenu;
                cameraController.ResetCameraLocation();
            }
            ImGui::Separator();
            currentTest->OnImGuiRender();

            ImGui::Separator();
            const glm::vec3 cameraPos = cameraController.GetCameraPos();
            const glm::vec3 cameraFront = cameraController.GetCameraFront();
            ImGui::Text("Position x: %.1f  y: %.1f  z: %.1f", cameraPos.x, cameraPos.y, cameraPos.z);
            ImGui::Text("Direction x: %.1f  y: %.1f  z: %.1f", cameraFront.x, cameraFront.y, cameraFront.z);
            ImGui::Text("Yaw %.1fº", cameraController.GetYaw());
            ImGui::Text("Pitch %.1fº", cameraController.GetPitch());

            if (ImGui::Button("Reset Camera Location"))
            {
                cameraController.ResetCameraLocation();
            }

            ImGui::Text("App average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); // swap buffers

        glfwPollEvents(); // I/O events, mouse events etc
    }
    delete currentTest;

    if (currentTest != testMenu)
        delete testMenu;

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

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        cameraController.ProcessKeyboardInput(CAMERA_MOVEMENT::DOWN, deltaTime);

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
