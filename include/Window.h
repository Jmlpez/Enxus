#ifndef WINDOW_H
#define WINDOW_H

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "utils.h"
#include "Event.h"

namespace Enxus
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string &title = "Enxus Engine",
                    unsigned int width = 1366,
                    unsigned int height = 680)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    class Window
    {

    public:
        using EventCallbackFn = std::function<void(Event &)>;

        Window(const WindowProps &props);
        ~Window();

        void OnUpdate();

        inline unsigned int GetWidth() { return m_Data.Width; }
        inline unsigned int GetHeight() { return m_Data.Height; }
        inline std::string GetTitle() { return m_Data.Title; }

        void SetVSync(bool enabled);
        inline bool IsVSync() { return m_Data.VSync; }

        static Scope<Window> Create(const WindowProps &props = WindowProps());

    private:
        void Init(const WindowProps &props);
        void ShutDown();

    private:
        GLFWwindow *m_Window;

        struct WindowData
        {
            unsigned int Width;
            unsigned int Height;
            std::string Title;
            bool VSync;

            EventCallbackFn EventCallback;
        };
        WindowData m_Data;
    };
}
#endif