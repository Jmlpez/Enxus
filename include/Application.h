#include "Window.h"
#include "utils.h"

namespace Enxus
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        Scope<Window> m_Window;
        bool m_Running;
    };

}