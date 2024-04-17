#include "Window.h"
#include "ApplicationEvent.h"

namespace Enxus
{
    class Application
    {
    public:
        Application();
        ~Application();

        void OnEvent(Event &event);
        void Run();

        bool OnWindowClose(WindowCloseEvent &event);

    private:
        Scope<Window> m_Window;
        bool m_Running = true;
    };

}