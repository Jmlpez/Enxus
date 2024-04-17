#include "pch/pch.h"
#include "Application.h"
#include "utils.h"

namespace Enxus
{

// Para hacer el binding mas facil se puede hacer un macro
// La desventaja de esto es cuando la funcion tiene mas parametros, asi que habria que usar otro
// metodo o hacerlo manual
#define BIND_EVENT_FN(func) std::bind(&func, this, std::placeholders::_1)

    Application *Application::s_Instance = nullptr;
    Application::Application()
    {
        if (s_Instance != nullptr)
        {
            std::cout << "[Application Error]: another instance already exist" << std::endl;
            ASSERT(false);
        }
        s_Instance = this;
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
            auto func = std::bind(&Application::OnEvent, this, std::placeholders::_1);
        */
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event &event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

        // From LayerOverlays to normal layers
        // check for someone to handle the event and break

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            it--;
            (*it)->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer *layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }
    bool Application::OnWindowClose(WindowCloseEvent &event)
    {
        m_Running = false;
        return true;
    }
    void Application::PushLayer(Layer *layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer *layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
}