#pragma once

#include "Blazar/Events/Events.h"

namespace Blazar {

class BLAZAR_API MouseButtonPressedEvent : public Event {
    int m_Button;

   public:
    MouseButtonPressedEvent(int button) : m_Button(button) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Mouse Press: " << m_Button;
        return ss.str();
    }

    int getButton() const { return m_Button; }

    EVENT_CLASS_TYPE(MouseButtonPressed);
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouseButton | EventCategoryMouse);
};

class BLAZAR_API MouseButtonReleasedEvent : public Event {
    int m_Button;

   public:
    MouseButtonReleasedEvent(int button) : m_Button(button) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Mouse Release: " << m_Button;
        return ss.str();
    }

    int getButton() const { return m_Button; }

    EVENT_CLASS_TYPE(MouseButtonReleased);
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouseButton | EventCategoryMouse);
};

class BLAZAR_API MouseScrolledEvent : public Event {
    double m_XOffset, m_YOffset;

   public:
    MouseScrolledEvent(double xOffset, double yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Mouse Scroll: x:" << m_XOffset << ", y:" << m_YOffset;
        return ss.str();
    }

    double getX() const { return m_XOffset; }
    double getY() const { return m_YOffset; }

    EVENT_CLASS_TYPE(MouseScrolled);
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);
};

class BLAZAR_API MouseMovedEvent : public Event {
    double m_xPos, m_yPos;

   public:
    MouseMovedEvent(double xPos, double yPos) : m_xPos(xPos), m_yPos(yPos) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Mouse Move: x:" << m_xPos << ", y:" << m_yPos;
        return ss.str();
    }

    double getX() const { return m_xPos; }
    double getY() const { return m_yPos; }

    EVENT_CLASS_TYPE(MouseMoved);
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);
};
};  // namespace Blazar
