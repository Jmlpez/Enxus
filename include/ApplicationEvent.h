#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H

#include "Event.h"

namespace Enxus
{

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(int width, int height)
            : m_Width(width), m_Height(height){};

        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        static EventType GetStaticType() { return EventType::WindowResize; }
        EventType GetEventType() const override { return GetStaticType(); }
        const char *GetName() const override { return "WindowResize"; }

        int GetCategoryFlags() const override { return EventCategory::EventCategoryApplication; }

    private:
        int m_Width, m_Height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowCloseEvent";
            return ss.str();
        }

        static EventType GetStaticType() { return EventType::WindowClose; }
        EventType GetEventType() const override { return GetStaticType(); }
        const char *GetName() const override { return "WindowClose"; }

        int GetCategoryFlags() const override { return EventCategoryApplication; }
    };
}

#endif