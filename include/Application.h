#ifndef APPLICATION_H
#define APPLICATION_H

#include "ApplicationEvent.h"
#include "LayerStack.h"
#include "ImGuiLayer.h"
#include "Window.h"

namespace Enxus
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void PushLayer(Layer *layer);
        void PushOverlay(Layer *layer);
        void OnEvent(Event &event);

        void Close();

        inline Window &GetWindow() { return *m_Window; }
        static Application &Get() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent &event);

    private:
        Scope<Window> m_Window;
        ImGuiLayer *m_ImGuiLayer;

        bool m_Running = true;
        float m_LastFrameTime = 0.0f;

        LayerStack m_LayerStack;
        static Application *s_Instance;
    };

    // To be defined in CLIENT
    Application *CreateApplication();

}
#endif