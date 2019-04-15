#pragma once

#include "defs.h"
#include "instrument_base.h"
#include "modulators.h"
#include "gate_patterns.h"
#include "tanzbar_settings.h"

const RandomParam sd_params[] = {
    {TB_SD_TUNE      ,  0, 127},
    {TB_SD_DTUNE     ,  0, 127},
    {TB_SD_SNAPPY    , 96, 127},
    {TB_SD_SN_DECAY  , 64, 127},
    {TB_SD_TONE      ,  0, 127},
    {TB_SD_TONE_DECAY,  0,  32},
    {TB_SD_PITCH     ,  0, 127},

    {TB_RS_TUNE      ,  0, 127},
};
const uint8_t nr_sd_params = sizeof(sd_params) / sizeof(*sd_params);

const RandomParam rs_params[] = {
    {TB_RS_TUNE      ,  0, 127},
};
const uint8_t nr_rs_params = sizeof(rs_params) / sizeof(*rs_params);

class TanzbarMid : public InstrumentBase
{
protected:
    GatePatternAB sd_pattern;
    GatePatternAB rs_pattern;

    MicroTimingStruct sd_timing;
    MicroTimingStruct rs_timing;

    ModulationReceiver rs_tune;

public:
    uint8_t snare_roll;

    TanzbarMid(
        Modulators& modulators_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref, true),
        rs_tune(modulators_ref)
    {
        storage.set_channel(MIDI_CHANNEL_TANZBAR);
    }

    void randomize()
    {
        last_randomized_time = millis();

        Tanzbar::randomize_parameters(sd_params, nr_sd_params);
        Tanzbar::randomize_parameters(rs_params, nr_rs_params);

        this->sd_pattern.set_coef_snare_pattern();
        this->rs_pattern.randomize();

        // Modulators
        uint8_t range = Rand::randui8(128);
        this->rs_tune.randomize(range, 127 - range, .3);

        // Timing
        this->sd_timing.randomize();
        this->rs_timing.randomize();
    }

    void play_roll(const TimeStruct& time)
    {
        static TimeDivision division = TimeDivision::Sixteenth;
        if (Utils::interval_hit(TimeDivision::Sixteenth, time))
        {
            uint8_t r = Rand::randui8(16);
            if (r < 3)
            {
                division = TimeDivision::Thirtysecond;
            }
            else
            {
                division = TimeDivision::Sixteenth;
            }
        }

        if (Utils::interval_hit(division, time))
        {
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_SD, this->snare_roll),
                time.get_shuffle_delay()
            );
        }
    }

    void play()
    {
        if (this->snare_roll > 0)
        {
            this->play_roll(time);
        }

        // Play snare
        if (this->sd_pattern.gate(time) && !this->kill)
        {
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_SD, 64),
                time.get_shuffle_delay(this->sd_timing)
            );
        }

        // Play rimshot
        if (this->rs_pattern.gate(time) && !this->kill)
        {
            uint8_t value = 0;
            if (this->rs_tune.value(time, value))
            {
                MidiIO::send_cc(TB_RS_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_RS, 64),
                time.get_shuffle_delay(this->rs_timing)
            );
        }
    }
};
