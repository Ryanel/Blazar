#pragma once

#include <Blazar/Events/Events.h>
#include <sstream>

namespace BLAZAR_API Blazar {

class WindowCloseEvent : public Event {
   public:
    WindowCloseEvent() {}

    EVENT_CLASS_TYPE(WindowClose);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class WindowFocusEvent : public Event {
   public:
    WindowFocusEvent() {}

    EVENT_CLASS_TYPE(WindowFocus);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class WindowLostFocusEvent : public Event {
   public:
    WindowLostFocusEvent() {}

    EVENT_CLASS_TYPE(WindowLostFocus);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class WindowResizeEvent : public Event {
   private:
    int m_Width;
    int m_Height;

   public:
    WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}

    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }
    virtual std::string ToString() const {
        std::stringstream ss;
        ss << "Window Resize (w: " << m_Width << ", h: " << m_Height << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class WindowMovedEvent : public Event {
   private:
    int m_X;
    int m_Y;

   public:
    WindowMovedEvent(int x, int y) : m_X(x), m_Y(y) {}

    inline int getX() const { return m_X; }
    inline int getY() const { return m_Y; }
    virtual std::string ToString() const {
        std::stringstream ss;
        ss << "Window Move (x: " << m_X << ", y: " << m_Y << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowMoved);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

};  // namespace Blazar
