#include "Event.h"

namespace Enxus
{

    class MouseButtonEvent : public Event
    {
    public:
        inline int GetButton() const { return m_Button; }
        virtual int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

    protected:
        MouseButtonEvent(int button) : m_Button(button) {}

        int m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button){};

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        static EventType GetStaticType() { return EventType::MouseButtonPressed; }
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char *GetName() const override { return "MouseButtonPressed"; }
    };
    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button){};

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        static EventType GetStaticType() { return EventType::MouseButtonReleased; }
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char *GetName() const override { return "MouseButtonReleased"; }
    };
}
