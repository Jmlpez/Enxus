#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H
#include "Event.h"

namespace Enxus
{
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float xpos, float ypos)
            : m_MouseX(xpos), m_MouseY(ypos) {}

        inline float GetMouseX() const { return m_MouseX; }
        inline float GetMouseY() const { return m_MouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: "
               << "(" << m_MouseX << "," << m_MouseY << ")";
            return ss.str();
        }
        static EventType GetStaticType() { return EventType::MouseMoved; }
        EventType GetEventType() const override { return GetStaticType(); }
        const char *GetName() const override { return "MouseMove"; }

        int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

    private:
        float m_MouseX, m_MouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

        inline float GetXOffset() const { return m_XOffset; }
        inline float GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        static EventType GetStaticType() { return EventType::MouseScrolled; }
        EventType GetEventType() const override { return GetStaticType(); }
        const char *GetName() const override { return "MouseScrolled"; }
        int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

    private:
        float m_XOffset, m_YOffset;
    };

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

#endif