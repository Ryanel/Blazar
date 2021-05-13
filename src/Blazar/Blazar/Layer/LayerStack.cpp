#include <bzpch.h>

#include <Blazar/Layer/Layer.h>
#include <Blazar/Layer/LayerStack.h>

namespace Blazar {

LayerStack::LayerStack() { m_LayerInsert = m_Layers.begin(); }

LayerStack::~LayerStack() {
    for (Layer* layer : m_Layers) { delete layer; }
}

void LayerStack::PushLayer(Layer* layer) {
    m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
    layer->OnAttach();
}

void LayerStack::PushOverlay(Layer* overlay) {
    overlay->OnAttach();
    m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer) {
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
        layer->OnDetached();
        m_Layers.erase(it);
        m_LayerInsert--;
    }
}

void LayerStack::PopOverlay(Layer* overlay) {
    auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
    if (it != m_Layers.end()) {
        overlay->OnDetached();
        m_Layers.erase(it);
    }
}

}  // namespace Blazar
