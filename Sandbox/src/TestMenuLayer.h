#ifndef TEST_MENU_LAYER_H
#define TEST_MENU_LAYER_H

#include "Enxus.h"
#include "Test.h"

class TestMenuLayer : public Enxus::Layer
{
public:
    TestMenuLayer();
    ~TestMenuLayer();

    void OnUpdate(Enxus::Timestep ts) override;
    void OnEvent(Enxus::Event &event) override;
    void OnAttach() override;
    void OnImGuiRender() override;

private:
    template <typename T>
    void RegisterTest(const std::string &name)
    {
        std::cout << "Registering Test: " << name << std::endl;
        m_Tests.push_back(
            {name,
             []()
             {
                 return new T();
             }});
    }
    void HandleViewportResize();

private:
    // viewport variables
    glm::vec2 m_ViewportSize;
    bool m_IsViewportFocused = false;
    bool m_IsViewportHovered = false;

    OpenGLTest::Test *m_CurrentTest;
    std::vector<std::pair<std::string, std::function<OpenGLTest::Test *()>>> m_Tests;

    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
    Enxus::Scope<Enxus::Framebuffer> m_Framebuffer;
};
#endif