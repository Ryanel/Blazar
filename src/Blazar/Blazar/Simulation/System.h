#pragma once

namespace Blazar {

class SimSystem {
   public:
    /// Update path
    enum class UpdatePath { Update, Render };
    virtual void tick(float deltatime);
    UpdatePath   path;
};
}  // namespace Blazar
