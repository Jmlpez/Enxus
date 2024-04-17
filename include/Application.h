#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "ApplicationEvent.h"
#include "LayerStack.h"

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

    private:
        bool OnWindowClose(WindowCloseEvent &event);

    private:
        Scope<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
    };

    // To be defined in CLIENT
    Application *CreateApplication();

}
#endif