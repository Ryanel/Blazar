#include "bzpch.h"

#include "Blazar/Layer/Layer.h"

namespace Blazar {
Layer::Layer(const std::string& debugName) : m_DebugName(debugName) {}
Layer::~Layer() {}
}  // namespace Blazar
