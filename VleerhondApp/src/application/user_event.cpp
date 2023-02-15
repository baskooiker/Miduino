#include "application/user_event.h"

namespace Vleerhond {
bool UserEvent::isHandled() { return this->handled; }

ChangeHarmonyEvent::ChangeHarmonyEvent(
    HarmonyType type, HarmonyStruct* harmony, TimeStruct* time)
    : type(type), harmony(harmony), time(time) {}

void ChangeHarmonyEvent::handle() {
    // ofLogNotice("", "handle: base class");
    if ((this->time->tick + 1) %
            (uint32_t)harmony->getPatternLength(this->type) ==
        0) {
        harmony->setType(type);
        this->handled = true;
    }
}
}  // namespace Vleerhond