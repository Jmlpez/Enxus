#ifndef LAYER_H
#define LAYER_H

#include "base.h"
#include "Event.h"

namespace Enxus
{

    class Layer
    {
    public:
        Layer(const std::string name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event &event)
        {
            (void)event; // to remove warning
        }

        inline const std::string &GetName() const { return m_DebugName; }

    private:
        std::string m_DebugName;
    };

}
#endif