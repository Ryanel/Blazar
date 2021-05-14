#pragma once

namespace Blazar {

class GraphicsContext {
   public:
    virtual void Init() = 0;
    virtual void Present() = 0;
};

};  // namespace Blazar
