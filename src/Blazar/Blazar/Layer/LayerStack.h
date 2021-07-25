#pragma once

#include <vector>

#include "Blazar/Layer/Layer.h"

namespace Blazar {

/// A ordered list of layers
class LayerStack {
   public:
    /// Create a layer stack
    LayerStack();

    /// Destory a layer stack
    ~LayerStack();

    /// Pushes a layer tothe back of the stack
    void PushLayer(Layer* layer);
    /// Pushes a layer onto the stack
    void PushOverlay(Layer* overlay);
    /// Pops a layer off the back of the stack
    void PopLayer(Layer* layer);

    /// Pops a layer off of the stack
    void PopOverlay(Layer* overlay);

    /// Iterator exposing m_Layers iterator
    std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
    /// Iterator exposing m_Layers iterator
    std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    /// Iterator exposing m_Layers iterator
    std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
    /// Iterator exposing m_Layers iterator
    std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

    /// Iterator exposing m_Layers iterator
    std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
    /// Iterator exposing m_Layers iterator
    std::vector<Layer*>::const_iterator end() const { return m_Layers.end(); }
    /// Iterator exposing m_Layers iterator
    std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
    /// Iterator exposing m_Layers iterator
    std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

    /// The list of layers
    std::vector<Layer*> m_Layers;

   private:
    unsigned int m_LayerInsertIndex = 0;
};
}  // namespace Blazar
