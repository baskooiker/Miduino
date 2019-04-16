#pragma once

#include "instrument_base.h"
#include "timing_structs.h"
#include "time_struct.h"
#include "modulators.h"
#include "gate_patterns.h"

namespace Vleerhond
{
    class TanzbarCy : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        ModulationReceiver cy_tune;
        ModulationReceiver cy_vel;

    public:
        GatePatternAB cy_pattern;

        TanzbarCy(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true),
            cy_tune(modulators_ref),
            cy_vel(modulators_ref)
        {
            storage.set_channel(MIDI_CHANNEL_TANZBAR);
        }

        void randomize();

        void randomize_hi_seq()
        {
            // Randomize Cymbal
            switch (Rand::distribution(16, 16, 16))
            {
            case 0:
                this->cy_pattern.set_coef_kick_pattern();
                this->cy_pattern.length = 16;
                break;
            case 1:
                this->cy_pattern.set_euclid(8, 3);
                this->cy_pattern.length = 8;
                break;
            case 2:
                this->cy_pattern.set_coef_hat_pattern();
                this->cy_pattern.length = 16;
                break;
            }
        }

        void play()
        {
            // Play Cymbal
            if (this->cy_pattern.gate(time) && !kill)
            {
                uint8_t value = 0;
                if (this->cy_tune.value(time, value))
                {
                    MidiIO::send_cc(TB_CY_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
                }
                uint8_t velocity = 100;
                this->cy_vel.value(time, velocity);
                this->storage.note_on(
                    NoteStruct(NOTE_TANZBAR_CY, 64),
                    time.get_shuffle_delay(this->timing)
                );
            }
        }
    };
}