#define BLAZAR_CREATE_APPLICATION

#include "Blazar/Blazar.h"
#include "Blazar/Entry.h"

#include "Editor/Editor.h"
#include "Sandbox/Sandbox.h"

using namespace Blazar;

class Game : public Blazar::Application {
   public:
    Game() {
        BLAZAR_PROFILE_FUNCTION();
        PushLayer(new Sandbox());

        Editor* editor = new Editor();
        PushLayer(editor);
        editor->Setup();
    }

    ~Game() {}
};

namespace Blazar {

Application* CreateApplication() { return new Game(); }

}  // namespace Blazar
