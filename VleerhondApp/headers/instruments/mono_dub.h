#pragma once

#include "instrument_base.h"

namespace Vleerhond
{
    class MonoDub : public TonalInstrumentBase
    {
    public:
        const Mono& lead;
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

            if (lead.get_hit())
            {
                uint8_t pitch = 0;

                pitch = lead.get_mono_pitch();
                //pitch = this->harmony.scale.get_ascending(pitch);
                pitch += (Utils::rerange(this->variable_pitch_offset, 3) - 1) * 12;

                this->midi_channel.note_on(
                    NoteStruct(pitch, 64, 6, NoteType::Tie),
                    time.get_shuffle_delay()
                );
                return true;
            }
            return false;
        }
    };
}