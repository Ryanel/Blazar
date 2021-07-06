#include "bzpch.h"

#include "Blazar/Layer/Layer.h"
#include "Blazar/Layer/LayerStack.h"

namespace Blazar {

LayerStack::LayerStack() {}

LayerStack::~LayerStack() {
    for (Layer* layer : m_Layers) { 
        layer->OnDetached();
        delete layer; 
    }
}

void LayerStack::PushLayer(Layer* layer) {
    layer->OnAttach();
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* overlay) {
    overlay->OnAttach();
    m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer) {
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
    if (it != m_Layers.begin() + m_LayerInsertIndex) {
        layer->OnDetached();
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStack::PopOverlay(Layer* overlay) {
    auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
    if (it != m_Layers.end()) {
        overlay->OnDetached();
        m_Layers.erase(it);
    }
}

}  // namespace Blazar
