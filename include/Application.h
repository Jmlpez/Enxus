#include "Window.h"

namespace Enxus
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();

    private:
        Scope<Window> m_Window;
        bool m_Running;
    };

}