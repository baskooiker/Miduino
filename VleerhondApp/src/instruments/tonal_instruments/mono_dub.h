#pragma once

#include "instrument_base.h"

namespace Vleerhond {
class MonoDub : public TonalInstrumentBase {
   public:
    Mono& lead;
    uint8_t variable_pitch_offset;

    MonoDub(
        Mono& lead_settings, HarmonyStruct& harmony, TimeStruct& time,
        const uint8_t midi_channel, const uint8_t offset = 0)
        : TonalInstrumentBase(harmony, time, true, midi_channel, offset),
          lead(lead_settings) {}

    void totalRandomize() {}

    void randomize() {}

    bool play() {
        if (this->kill) return false;

        NoteInfo note_info = lead.getNoteEvent();

        if (note_info.hit) {
            int8_t pitch = note_info.note.pitch;
            switch (Utils::rerange(this->variable_pitch_offset, 6)) {
                case 0:
                    pitch -= 12;
                    break;
                case 1:
                    pitch -= 5;
                    break;
                case 2:
                    pitch += 0;
                    break;
                case 3:
                    pitch = this->harmony.scale.getAscending(
                        note_info.note.pitch, 2);
                    break;
                case 4:
                    pitch += 7;
                    break;
                case 5:
                    pitch += 12;
                    break;
            }
            note_info.note.pitch = pitch;

            this->midi_channel->noteOn(note_info.note, time.getShuffleDelay());
            return true;
        }
        return false;
    }
};
}  // namespace Vleerhond