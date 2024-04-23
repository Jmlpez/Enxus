#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Layer.h"
#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace Enxus
{

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event &event) override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }

    private:
        bool m_BlockEvents = false;
        float m_Time = 0.0f;
    };

}
#endif