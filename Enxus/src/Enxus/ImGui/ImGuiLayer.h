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
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
    private:
        float m_Time = 0.0f;
    };

}
#endif