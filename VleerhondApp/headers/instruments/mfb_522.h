#pragma once

#include "defs.h"
#include "intervals.h"
#include "midi_io.h"

#include "gate_patterns.h"
#include "interval_pattern.h"
#include "instrument_base.h"

namespace Vleerhond
{
    class Mfb522Settings : public InstrumentBase
    {
    public:
        GatePattern16 ac_522_pattern;
        GatePatternAB bd_522_pattern;
        GatePattern16 lo_tom_522_pattern;
        GatePattern16 mi_tom_522_pattern;
        GatePattern16 rs_522_pattern;
        GatePatternAB clave_522_pattern;
        GatePatternAB clap_522_pattern;
        GatePatternAB hh_522_pattern;
        GatePatternAB oh_522_pattern;
        GatePatternAB cy_522_pattern;
        GatePatternAB sd_522_pattern;

        bool use_hh_int;
        IntervalPattern hh_int_pattern;

        Mfb522Settings(TimeStruct& time_ref) :
            InstrumentBase(time_ref, true)
        {
            use_hh_int = false;
        }

        void randomize_522_seq()
        {
            this->ac_522_pattern.randomize(.25f);

            this->bd_522_pattern.set_coef_kick_pattern();
            this->cy_522_pattern.set_coef_kick_pattern();

            this->lo_tom_522_pattern.randomize(.25f);
            this->mi_tom_522_pattern.randomize(.25f);
            this->rs_522_pattern.randomize(.25f);
            this->clave_522_pattern.randomize(.25);
            this->sd_522_pattern.randomize(.50);

            this->clap_522_pattern.set_coef_snare_pattern();

            float r = Rand::randf();
            if (r < .25)
            {
                this->hh_522_pattern.randomize(.25f);
                this->oh_522_pattern.randomize(.25f);

                this->use_hh_int = false;
            }
            else if (r < .5)
            {
                uint8_t steps[8] = { 6, 7, 8, 10, 12, 14, 15, 16 };
                Utils::randomize_order(steps, 8);
                uint8_t gates = Rand::randui8(MAX((uint8_t)(steps[0] * .3), 2), (uint8_t)(steps[0] * .8));
                this->hh_522_pattern.patterns[0].set_euclid(steps[0], gates);
                this->hh_522_pattern.length = steps[0];

                gates = Rand::randui8(MAX((uint8_t)(steps[1] * .3), 2), (uint8_t)(steps[1] * .8));
                this->hh_522_pattern.patterns[0].set_euclid(steps[1], gates);
                this->hh_522_pattern.length = steps[1];
                this->use_hh_int = false;
            }
            else
            {
                this->hh_int_pattern.randomize_interval_hat();
                this->use_hh_int = true;
            }

        }

        void play()
        {
            uint8_t velocity = 63;

            if (this->ac_522_pattern.gate(time))
            {
                velocity = 127;
            }

            if (this->bd_522_pattern.gate(time))
            {
                this->midi_channel.note_on(NoteStruct(NOTE_522_BD_LONG, velocity));
            }
            if (this->lo_tom_522_pattern.gate(time))
            {
                this->midi_channel.note_on(NoteStruct(NOTE_522_LO_TOM, velocity));
            }
            if (this->mi_tom_522_pattern.gate(time))
            {
                this->midi_channel.note_on(NoteStruct(NOTE_522_HI_TOM, velocity));
            }
            if (this->rs_522_pattern.gate(time))
            {
                this->midi_channel.note_on(NoteStruct(NOTE_522_RS, velocity));
            }
            if (this->clave_522_pattern.gate(time))
            {
                this->midi_channel.note_on(NoteStruct(NOTE_522_CLAVE, velocity));
            }
            if (this->clap_522_pattern.gate(time))
            {
                this->midi_channel.note_on(NoteStruct(NOTE_522_CP_LONG, velocity));
            }
            if (this->oh_522_pattern.gate(time))
            {
                this->midi_channel.note_on(NoteStruct(NOTE_522_OH, velocity));
            }
            if (this->cy_522_pattern.gate(time))
            {
                this->midi_channel.note_on(NoteStruct(NOTE_522_CYMBAL, velocity));
            }
            if (this->sd_522_pattern.gate(time))
            {
                this->midi_channel.note_on(NoteStruct(NOTE_522_SN, velocity));
            }
            if (!this->use_hh_int)
            {
                if (this->hh_522_pattern.gate(time))
                {
                    this->midi_channel.note_on(NoteStruct(NOTE_522_HH, velocity));
                }
            }
            else
            {
                if (this->hh_int_pattern.hit(time))
                {
                    this->midi_channel.note_on(NoteStruct(NOTE_522_HH, velocity));
                }
            }
        }

    };
}