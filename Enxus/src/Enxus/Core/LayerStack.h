#ifndef LAYER_STACK_H
#define LAYER_STACK_H

#include "Event.h"
#include "Layer.h"

namespace Enxus
{

    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer *layer);
        void PushOverlay(Layer *overlay);
        void PopLayer(Layer *layer);
        void PopOverlay(Layer *overlay);

        std::vector<Layer *>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer *>::iterator end() { return m_Layers.end(); }
        std::vector<Layer *>::const_iterator cbegin() { return m_Layers.cbegin(); }
        std::vector<Layer *>::const_iterator cend() { return m_Layers.cend(); }
        std::vector<Layer *>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
        std::vector<Layer *>::reverse_iterator rend() { return m_Layers.rend(); }

    private:
        std::vector<Layer *> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };

}
#endif