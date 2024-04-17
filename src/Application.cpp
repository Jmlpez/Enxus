#include "Application.h"
#include <functional>

namespace Enxus
{
    Application::Application()

    {
        // m_Window = CreateScope<Window>(Window::Create());
        m_Window = Window::Create();

        /*
            hacer una referencia a la funcion OnEvent
            para que sea dicha funcion la que se llame cuando se dispare
            un evento en la clase window, que es la que precisamente los dispara.
            Como OnEvent es una funcion miembro, lo que se hace es pasar la direccion
            de memoria de dicha funcion, y los parametros que recibe.

            Como toda funcion miembro de una clase recibe implicitamente el parametro
            this, entonces OnEvent(Event&) se vuelve OnEvent(Application *instance, Event&), y con
            std::placeholders::_1 lo que se dice es  que cuando se llame a
            la funcion func declarada se le pase como primer argumento el puntero this
            y como 2do argumento el evento que la clase Window determine.

            Basicamente es un alias
        */
        auto func = std::bind(&Application::OnEvent, this, std::placeholders::_1);
        m_Window->SetEventCallback(func);
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event &event)
    {

        std::cout << event;
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>(
            [this](WindowCloseEvent &ev)
            {
                std::cout << ev;
                this->m_Running = false;
                return true;
            });
    }

    bool Application::CloseWindow(WindowCloseEvent &event)
    {
        // std::cout << event;
        // m_Running = false;
        // return true;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_Window->OnUpdate();
        }
    }
}