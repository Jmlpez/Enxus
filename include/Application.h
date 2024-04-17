#ifndef APPLICATION_H
#define APPLICATION_H

#include "ApplicationEvent.h"
#include "LayerStack.h"
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

        static Application &Get() { return *s_Instance; }

        Scope<Window> &GetWindow() { return m_Window; }

    private:
        bool OnWindowClose(WindowCloseEvent &event);

    private:
        Scope<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        static Application *s_Instance;
    };

    // To be defined in CLIENT
    Application *CreateApplication();

}
#endif