#pragma once

#include "instrument_base.h"
#include "patterns/fugue.h"

namespace Vleerhond {
class FuguePlayer : public TonalInstrumentBase {
    Fugue& fugue;

   public:
    uint8_t pitch_offset;
    uint8_t manual_pitch_offset;
    uint8_t length;
    FuguePlayerType type;
    uint32_t counter;
    uint8_t rhythm;
    uint8_t density;
    NoteInterval note_interval;
    uint8_t note_repeat;

    FuguePlayer(HarmonyStruct& harmony, TimeStruct& time, Fugue& fugue);
    bool play();
    void randomize();
    void reset();
};
}  // namespace Vleerhond