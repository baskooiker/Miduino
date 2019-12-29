#pragma once

#include "defs.h"
#include "instruments/instrument_base.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
#include "instruments/parameters.h"
#include "instruments/drums/snare.h"

namespace Vleerhond
{
    const RandomParam sd_params[] = {
        {TB_SD_TUNE      ,  0,  64},
        {TB_SD_DTUNE     ,  0,  64},
        {TB_SD_SNAPPY    , 64, 127},
        {TB_SD_SN_DECAY  , 64, 127},
        {TB_SD_TONE      ,  0,  64},
        {TB_SD_TONE_DECAY,  0,  48},
        {TB_SD_PITCH     ,  0,  64},

        {TB_RS_TUNE      ,  0,  64},
    };
    const uint8_t nr_sd_params = sizeof(sd_params) / sizeof(*sd_params);

    const RandomParam rs_params[] = {
        {TB_RS_TUNE      ,  0, 127},
    };
    const uint8_t nr_rs_params = sizeof(rs_params) / sizeof(*rs_params);

    class TanzbarMid : public Snare
    {
    protected:
        GatePatternAB rs_pattern;
        MicroTimingStruct rs_timing;
        ModulationReceiver rs_tune;

    public:
        TanzbarMid(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Snare(modulators_ref, time_ref, MIDI_CHANNEL_TANZBAR),
            rs_tune(modulators_ref)
        {
            pitch = NOTE_TANZBAR_SD;
            settings.p_coef = 0;
            settings.p_off = 0;
        }

        void randomize()
        {
            Snare::randomize();
            Parameters::randomize_parameters(sd_params, nr_sd_params, MIDI_CC_CHANNEL_TANZBAR);
            Parameters::randomize_parameters(rs_params, nr_rs_params, MIDI_CC_CHANNEL_TANZBAR);

            this->rs_pattern.randomize();

            // Modulators
            uint8_t range = Rand::randui8(128);
            this->rs_tune.randomize(range, 127 - range, .3);

            // Timing
            this->rs_timing.randomize();
        }

        bool play()
        {
            bool played = false;
            // Play rimshot
            if (this->rs_pattern.gate(time) && !this->kill)
            {
                uint8_t value = 0;
                if (this->rs_tune.value(time, value))
                {
                    MidiIO::send_cc(TB_RS_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
                }
                this->midi_channel.note_on(
                    NoteStruct(NOTE_TANZBAR_RS, 64),
                    time.get_shuffle_delay(this->rs_timing)
                );
                played = true;
            }

            return Snare::play() || played;
        }
    };
}