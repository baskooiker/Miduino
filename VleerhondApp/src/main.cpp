#include "VleerhondApp.h"
// #include "ofAppNoWindow.h"

namespace Vleerhond {
void test_things() {
    HarmonyStruct harmony;
    TimeStruct time;
    Vermona vermona(harmony, time);

    harmony.scale.setRoot(Root::ROOT_G);
    harmony.setType(HarmonyType::DominantHigh);

    for (int i = 0; i < 1000; i++) {
        vermona.randomize();
        harmony.randomize();
        for (int t = 0; t < 64; t++) {
            vermona.play();
            time.tick += 1;
        }
    }
}
}  // namespace Vleerhond

//========================================================================
int main() {
    Vleerhond::test_things();

    return 0;
    // ofAppNoWindow headless;
    // ofSetupOpenGL(&headless, 640, 420, OF_WINDOW);

    // ofRunApp(new Vleerhond::VleerhondApp());

    auto app = new Vleerhond::VleerhondApp();
    app->setup();
    while (true) {
        app->update();
    }
}
