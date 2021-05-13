#pragma once

#include <Blazar/Events/Events.h>

namespace BLAZAR_API Blazar {

class KeyDownEvent : public Event {
    int m_Keycode;
    bool m_Repeated;

   public:
    KeyDownEvent(int keycode, bool repeating) : m_Keycode(keycode), m_Repeated(repeating) {}

    std::string ToString() const override {
        std::stringstream ss;

        if (m_Repeated) {
            ss << "Key repeat: " << m_Keycode;
        } else {
            ss << "Key down: " << m_Keycode;
        }

        return ss.str();
    }

    int getKeycode() const { return m_Keycode; }
    bool repeating() const { return m_Repeated; }

    EVENT_CLASS_TYPE(KeyDown);
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);
};

class KeyUpEvent : public Event {
    int m_Keycode;

   public:
    KeyUpEvent(int keycode) : m_Keycode(keycode) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Key up: " << m_Keycode;
        return ss.str();
    }

    int getKeycode() const { return m_Keycode; }

    EVENT_CLASS_TYPE(KeyDown);
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);
};

};  // namespace Blazar
