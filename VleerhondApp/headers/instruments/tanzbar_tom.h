#pragma once

#include "defs.h"
#include "instrument_base.h"
#include "modulators.h"
#include "gate_patterns.h"
#include "tanzbar_settings.h"

const RandomParam tom_params[] = {

    {TB_HTC_TUNE          ,  0, 127},
    {TB_HTC_DECAY         , 64,  96},
    {TB_HTC_NOISE_ON_OFF  ,  0, 127},
    {TB_HTC_TOM_CONGA     ,  0, 127},
    {TB_MTC_TUNE          ,  0, 127},
    {TB_MTC_DECAY         , 64,  96},
    {TB_MTC_NOISE_ON_OFF  ,  0, 127},
    {TB_MTC_TOM_CONGA     ,  0, 127},
    {TB_LTC_TUNE          ,  0, 127},
    {TB_LTC_DECAY         , 64,  96},
    {TB_LTC_NOISE_ON_OFF  ,  0, 127},
    {TB_LTC_TOM_CONGA     ,  0, 127},
    {TB_TOM_NOISE         ,  0, 127},
};
const uint8_t nr_tom_params = sizeof(tom_params) / sizeof(*tom_params);

class TanzbarTom : public InstrumentBase
{
protected:
    MicroTimingStruct tc_timing;

    ModulationReceiver tom_vel;

    CvPatternAB tom_pattern;

    PercussionType percussion_type;
    GatePatternAB tom_mask;

public:

    TanzbarTom(
        Modulators& modulators_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref, true),
        tom_vel(modulators_ref)
    {
        storage.set_channel(MIDI_CHANNEL_TANZBAR);
        percussion_type = PercussionType::PercussionToms;
    }

    void randomize()
    {
        ofLogNotice("tanzbar_perc", "randomize()");
        last_randomized_time = millis();

        Tanzbar::randomize_parameters(tom_params, nr_tom_params);

        // Randomize toms
        this->tom_pattern.randomize();
        switch (Rand::distribution(32, 32))
        {
        case 0: this->percussion_type = PercussionType::PercussionToms; break;
        case 1: this->percussion_type = PercussionType::PercussionCongas; break;
        }
        this->tom_mask.randomize_mask_pattern();

        // Modulators
        uint8_t range = Rand::randui8(16, 64);
        this->tom_vel.randomize(range, 127 - range);

        this->tc_timing.randomize();
    }

    void play()
    {
        // Play toms
        uint8_t tom_prob = this->tom_pattern.value(time);
        if (Utils::interval_hit(TimeDivision::Sixteenth, time)
            && tom_prob < 100
            && this->tom_mask.gate(time)
            && !kill)
        {
            uint8_t tom_id = tom_prob % 3;
            uint8_t velocity = 100;
            this->tom_vel.value(time, velocity);

            this->storage.note_on(
                NoteStruct(get_tc_pitch(tom_id, this->percussion_type), velocity),
                time.get_shuffle_delay(this->tc_timing)
            );
        }

    }

protected:
    uint8_t get_tc_pitch(const uint8_t id, const PercussionType type)
    {
        static const uint8_t t_pitches[3] = { NOTE_TANZBAR_LT, NOTE_TANZBAR_MT, NOTE_TANZBAR_HT };
        static const uint8_t c_pitches[3] = { NOTE_TANZBAR_LC, NOTE_TANZBAR_MC, NOTE_TANZBAR_HC };
        switch (type)
        {
        case PercussionType::PercussionToms:
            return t_pitches[id % 3];
        case PercussionType::PercussionCongas:
        default:
            return c_pitches[id % 3];
        }
    }

};
