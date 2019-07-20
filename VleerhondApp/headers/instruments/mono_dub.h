#pragma once

#include "instrument_base.h"

namespace Vleerhond
{
    class MonoDub : public TonalInstrumentBase
    {
    public:
        Mono& lead;
        uint8_t variable_pitch_offset;

        MonoDub(
            Mono& lead_settings,
            HarmonyStruct& harmony,
            TimeStruct& time) :
            TonalInstrumentBase(harmony, time, true),
            lead(lead_settings)
        {
        }

        void total_randomize()
        {
        }

        void randomize()
        {
        }

        bool play()
        {
            if (this->kill)
                return false;

            NoteInfo note_info = lead.get_note_event();

            if (note_info.hit)
            {
                //pitch = this->harmony.scale.get_ascending(pitch);
                note_info.note.pitch += (Utils::rerange(this->variable_pitch_offset, 3) - 1) * 12;

                this->midi_channel.note_on(
                    note_info.note,
                    time.get_shuffle_delay()
                );
                return true;
            }
            return false;
        }
    };
}