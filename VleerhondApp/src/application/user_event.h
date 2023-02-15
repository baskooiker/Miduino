#pragma once

#include "core/time_struct.h"
#include "harmony/harmony_struct.h"

namespace Vleerhond {
class UserEvent {
   protected:
    bool handled = false;

   public:
    virtual void handle() = 0;

    bool isHandled();
};

class ChangeHarmonyEvent : public UserEvent {
   private:
    const HarmonyType type;
    HarmonyStruct* harmony;
    TimeStruct* time;

   public:
    ChangeHarmonyEvent(
        HarmonyType type, HarmonyStruct* harmony, TimeStruct* time);
    virtual void handle();
};
}  // namespace Vleerhond
