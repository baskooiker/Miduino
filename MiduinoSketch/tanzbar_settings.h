#pragma once

#include "defs.h"
#include "modulators.h"
#include "mfb_503.h"
#include "timing_structs.h"
#include "interval_pattern.h"
#include "instrument_base.h"

#define MODULATOR_PROB (.3f)

const RandomParam tanzbar_low_params[] = {
    {TB_BD1_ATTACK   ,  0, 127},
    {TB_BD1_DECAY    , 32,  96},
    {TB_BD1_PITCH    , 64,  96},
    {TB_BD1_TUNE     , 32,  96},
    {TB_BD1_NOISE    ,  0, 127},
    {TB_BD1_FILTER   ,  0, 127},
    {TB_BD1_DIST     ,  0, 127},
    {TB_BD1_TRIGGER  ,  0, 127},

    {TB_BD2_DECAY    , 32,  96},
    {TB_BD2_TUNE     , 32,  96},
    {TB_BD2_TONE     , 32,  96}
};

const RandomParam tanzbar_mid_params[] = {
    {TB_SD_TUNE      ,  0, 127},
    {TB_SD_DTUNE     ,  0, 127},
    {TB_SD_SNAPPY    , 96, 127},
    {TB_SD_SN_DECAY  , 64, 127},
    {TB_SD_TONE      ,  0, 127},
    {TB_SD_TONE_DECAY,  0,  64},
    {TB_SD_PITCH     ,  0, 127},

    {TB_RS_TUNE      ,  0, 127},
    {TB_CP_DECAY     , 32,  96},
    {TB_CP_FILTER    , 32,  96},
    {TB_CP_ATTACK    , 32,  96},
    {TB_CP_TRIGGER   , 32,  96}
};

const RandomParam tanzbar_hi_params[] = {
    {TB_OH_DECAY,  0,  96},
    {TB_HH_TUNE ,  0, 127},
    {TB_HH_DECAY,  0, 127},

    {TB_CY_DECAY,  64, 127},
    {TB_CY_TONE ,   0, 127},
    {TB_CY_TUNE ,   0, 127}
};

const RandomParam tanzbar_perc_params[] = {

    {TB_CL_TUNE           , 32,  96},
    {TB_CL_DECAY          , 32,  96},
    {TB_HTC_TUNE          , 32,  96},
    {TB_HTC_DECAY         , 32,  96},
    {TB_HTC_NOISE_ON_OFF  , 32,  96},
    {TB_HTC_TOM_CONGA     , 32,  96},
    {TB_MTC_TUNE          , 32,  96},
    {TB_MTC_DECAY         , 32,  96},
    {TB_MTC_NOISE_ON_OFF  , 32,  96},
    {TB_MTC_TOM_CONGA     , 32,  96},
    {TB_LTC_TUNE          , 32,  96},
    {TB_LTC_DECAY         , 32,  96},
    {TB_LTC_NOISE_ON_OFF  , 32,  96},
    {TB_LTC_TOM_CONGA     , 32,  96},
    {TB_TOM_NOISE         , 32,  96},
    {TB_CB_Tune           , 32,  96},
    {TB_CB_Decay          , 32,  96},
    {TB_MA_Decay          , 32,  96},
};
const uint8_t nr_tanzbar_low_params = sizeof(tanzbar_low_params) / sizeof(RandomParam);
const uint8_t nr_tanzbar_mid_params = sizeof(tanzbar_mid_params) / sizeof(RandomParam);
const uint8_t nr_tanzbar_perc_params = sizeof(tanzbar_perc_params) / sizeof(RandomParam);
const uint8_t nr_tanzbar_hi_params = sizeof(tanzbar_hi_params) / sizeof(RandomParam);

class TanzbarModulators
{
public:
    ModulationReceiver cl_pitch;
    ModulationReceiver ma_dec;
    ModulationReceiver rs_tune;
    ModulationReceiver cb_tune;
    ModulationReceiver cp_trig;
    ModulationReceiver cy_tune;
    ModulationReceiver cy_vel;
    ModulationReceiver hats_vel;
    ModulationReceiver tom_vel;

    TanzbarModulators(Modulators& modulators) :
        cl_pitch(modulators),
        ma_dec(modulators),
        rs_tune(modulators),
        cb_tune(modulators),
        cp_trig(modulators),
        cy_tune(modulators),
        cy_vel(modulators),
        hats_vel(modulators),
        tom_vel(modulators)
    {

    }

    void randomize()
    {
        uint8_t range = Rand::randui8(128, 64);
        this->cl_pitch.randomize(range, 127 - range, MODULATOR_PROB);

        this->ma_dec.randomize(Rand::randui8(128, 64), 0);

        range = Rand::randui8(128);
        this->rs_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = Rand::randui8(128);
        this->cb_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = Rand::randui8(128);
        this->cp_trig.randomize(range, 127 - range, MODULATOR_PROB);

        range = Rand::randui8(128);
        this->cy_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = Rand::randui8(16, 64);
        this->hats_vel.randomize(range, 127 - range);

        range = Rand::randui8(16, 64);
        this->cy_vel.randomize(range, 127 - range);

        range = Rand::randui8(16, 64);
        this->tom_vel.randomize(range, 127 - range);
    }
};

class Tanzbar : public InstrumentBase
{
    Modulators& modulators;

public:
    GatePatternAB bd_pattern;
    GatePatternAB sd_pattern;
    GatePatternAB rs_pattern;
    GatePatternAB cp_pattern;
    GatePatternAB cl_pattern;
    GatePatternAB cb_pattern;
    CvPatternAB tom_pattern;
    
    PercussionType percussion_type;
    GatePatternAB tom_mask;

    bool kill_low;
    bool kill_mid;
    bool kill_perc;
    bool drum_fill;
    uint8_t snare_roll;

    TanzbarModulators mod_receivers;
    TanzbarTimeSettings time_settings;

    Tanzbar(Modulators& modulators_ref, HarmonyStruct& harmony_ref, TimeStruct& time_ref) :
        InstrumentBase(time_ref),
        modulators(modulators_ref),
        mod_receivers(modulators)
    {
        percussion_type = PercussionType::PercussionToms;
    }

    void randomize_tanzbar_kick()
    {
        // Fill in first or second half of bar
        uint8_t half = Rand::distribution(64, 64);

        uint8_t bar = this->bd_pattern.abPattern.value(Rand::randui8(4));
        this->bd_pattern.patterns[bar].set_kick_fill(half * 8);
    }

    void randomize_timing()
    {
        this->time_settings.bd.randomize();
        this->time_settings.cb.randomize();
        this->time_settings.cl.randomize();
        this->time_settings.cp.randomize();
        this->time_settings.cy.randomize();
        this->time_settings.hh.randomize();
        this->time_settings.ma.randomize();
        this->time_settings.sd.randomize();
        this->time_settings.tc.randomize();
    }

    void randomize_low_seq()
    {
        this->bd_pattern.set_coef_kick_pattern();

        if (Rand::distribution(32, 12))
        {
            this->randomize_tanzbar_kick();
        }
    }

    void randomize_mid_seq()
    {
        this->sd_pattern.set_coef_snare_pattern();
        this->cp_pattern.set_coef_snare_pattern();

        this->rs_pattern.randomize();

    }

    void randomize_perc_seq()
    {
        // Randomize toms
        this->tom_pattern.randomize();
        switch (Rand::distribution(32, 32))
        {
        case 0: this->percussion_type = PercussionType::PercussionToms; break;
        case 1: this->percussion_type = PercussionType::PercussionCongas; break;
        }
        this->tom_mask.randomize_mask_pattern();

        // Randomize other percussion
        this->cl_pattern.randomize();
        this->cb_pattern.randomize();
    }

    void randomize()
    {
        randomize_low_seq();
        randomize_mid_seq();
        randomize_perc_seq();

        // Randomize micro-timing
        this->randomize_timing();

        // Randomize modulators
        this->mod_receivers.randomize();
    }

    void play_fill(const TimeStruct time)
    {
        if (!Utils::interval_hit(TimeDivision::Sixteenth, time))
            return;

        uint8_t p = 0;
        switch (Rand::randui8(7))
        {
        case 0: p = NOTE_TANZBAR_BD1; break;
        case 1: p = NOTE_TANZBAR_SD; break;
        case 2: p = NOTE_TANZBAR_LT; break;
        case 3: p = NOTE_TANZBAR_MT; break;
        case 4: p = NOTE_TANZBAR_HT; break;
        case 5: p = NOTE_TANZBAR_OH; break;
        case 6: p = NOTE_TANZBAR_HH; break;
        }
        this->storage.note_on(NoteStruct(p, 127), time.get_shuffle_delay());
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

    void play_bd(const TimeStruct& time)
    {
        bool quarter_hit = Utils::interval_hit(TimeDivision::Quarter, time);
        uint8_t velocity = quarter_hit ? 127 : 63;
        if (this->bd_pattern.gate(time) && !this->kill_low)
        {
            uint8_t pitch = NOTE_TANZBAR_BD1;
            this->storage.note_on(
                NoteStruct(pitch, velocity),
                time.get_shuffle_delay(this->time_settings.bd)
            );
        }

        if (quarter_hit && !this->kill_low)
        {
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_BD2, velocity),
                time.get_shuffle_delay(this->time_settings.bd)
            );
        }
    }

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

    void play()
    {
        if (this->drum_fill)
        {
            return this->play_fill(time);
        }

        if (this->snare_roll > 0)
        {
            this->play_roll(time);
        }

        uint8_t velocity = 63;

        // Play kick
        this->play_bd(time);

        // Play snare
        if (this->sd_pattern.gate(time) && !this->kill_mid)
        {
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_SD, velocity),
                time.get_shuffle_delay(this->time_settings.sd)
            );
        }

        // Play rimshot
        if (this->rs_pattern.gate(time) && !this->kill_mid)
        {
            uint8_t value = 0;
            if (this->mod_receivers.rs_tune.value(time, value))
            {
                MidiIO::send_cc(TB_RS_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_RS, velocity),
                time.get_shuffle_delay(this->time_settings.sd)
            );
        }

        // Play clap
        if (this->cp_pattern.gate(time) && !this->kill_mid)
        {
            uint8_t value = 0;
            if (this->mod_receivers.cp_trig.value(time, value))
            {
                MidiIO::send_cc(TB_CP_TRIGGER, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_CP, velocity),
                time.get_shuffle_delay(this->time_settings.cp)
            );
        }

        // Play clave
        if (this->cl_pattern.gate(time))
        {
            uint8_t value = 0;
            if (this->mod_receivers.cl_pitch.value(time, value))
            {
                MidiIO::send_cc(TB_CL_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_CL, velocity),
                time.get_shuffle_delay(this->time_settings.cl)
            );
        }

        // Play cowbell
        if (this->cb_pattern.gate(time) && !kill_perc)
        {
            uint8_t value = 0;
            if (this->mod_receivers.cb_tune.value(time, value))
            {
                MidiIO::send_cc(TB_CB_Tune, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_CB, velocity),
                time.get_shuffle_delay(this->time_settings.cb)
            );
        }

        // Play toms
        uint8_t tom_prob = this->tom_pattern.value(time);
        if (Utils::interval_hit(TimeDivision::Sixteenth, time)
            && tom_prob < 100
            && this->tom_mask.gate(time)
            && !kill_perc)
        {
            uint8_t tom_id = tom_prob % 3;
            uint8_t velocity = 100;
            this->mod_receivers.tom_vel.value(time, velocity);

            this->storage.note_on(
                NoteStruct(get_tc_pitch(tom_id, this->percussion_type), velocity),
                time.get_shuffle_delay(this->time_settings.tc)
            );
        }

    }

    static void randomize_parameters(const RandomParam* list, const uint8_t length)
    {
        for (int i = 0; i < length; i++)
        {
            MidiIO::send_cc(list[i].note,
                Rand::randui8(list[i].min, list[i].max),
                MIDI_CC_CHANNEL_TANZBAR);
        }
    }

    void randomize_tanzbar_sound()
    {
        randomize_parameters(tanzbar_low_params, nr_tanzbar_low_params);
        randomize_parameters(tanzbar_mid_params, nr_tanzbar_mid_params);
        randomize_parameters(tanzbar_perc_params, nr_tanzbar_perc_params);
        randomize_parameters(tanzbar_hi_params, nr_tanzbar_hi_params);
    }

    void randomize_low()
    {
        randomize_parameters(tanzbar_low_params, nr_tanzbar_low_params);
        randomize_low_seq();
    }

    void randomize_mid()
    {
        randomize_parameters(tanzbar_mid_params, nr_tanzbar_mid_params);
        randomize_mid_seq();
    }

    void randomize_perc()
    {
        randomize_parameters(tanzbar_perc_params, nr_tanzbar_perc_params);
        randomize_perc_seq();
    }

};

class TanzbarHi : public InstrumentBase
{
protected:
    TanzbarModulators& tanzbar_modulators;
    TanzbarTimeSettings& tanzbar_time;

public:
    GatePatternAB hh_pattern;
    GatePatternAB oh_pattern;
    GatePatternAB cy_pattern;
    CvPatternAB ma_pattern;

    IntervalPattern hat_int_pattern;
    CvPatternAB hat_velocity;

    HatClosedStyle hat_closed_style;

    TanzbarHi(
        TanzbarModulators& tanzbar_modulators_ref, 
        TanzbarTimeSettings& tanzbar_time_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref),
        tanzbar_modulators(tanzbar_modulators_ref),
        tanzbar_time(tanzbar_time_ref)
    {
        hat_closed_style = HatClosedStyle::HatClosedRegular;
    }

    void randomize()
    {
        Tanzbar::randomize_parameters(tanzbar_hi_params, nr_tanzbar_hi_params);
        randomize_hi_seq();
    }

    void randomize_hi_seq()
    {
        // Randomize hats
        this->oh_pattern.set_coef_hat_pattern();
        uint8_t four_pat = 0;
        switch (Rand::distribution(32, 10, 10, 10, 10))
        {
        case 0: four_pat = BXXXX; break;
        case 1: four_pat = BXXX0; break;
        case 2: four_pat = BXX0X; break;
        case 3: four_pat = BX0XX; break;
        case 4: four_pat = B0XXX; break;
        }
        for (int i = 0; i < 3; i++)
        {
            for (int step = 0; step < 4; step++)
            {
                this->hh_pattern.patterns[i].set_gate(step, Utils::gate(four_pat, step));
            }
            this->hh_pattern.length = 4;
            this->hh_pattern.abPattern.set_ab_pattern();
        }
        switch (Rand::distribution(32, 32))
        {
        case 0: this->hat_closed_style = HatClosedStyle::HatClosedRegular; break;
        case 1: this->hat_closed_style = HatClosedStyle::HatClosedInterval; break;
        }

        this->hat_int_pattern.randomize_interval_hat();
        this->hat_velocity.randomize();

        // Randomize Maracas
        this->ma_pattern.randomize();
        this->ma_pattern.time_division = TimeDivision::Sixteenth;
        switch (Rand::distribution(32, 32, 32))
        {
        case 0: this->ma_pattern.length = 2; break;
        case 1: this->ma_pattern.length = 4; break;
        case 2: this->ma_pattern.length = 8; break;
        }

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

    bool play_maracas()
    {
        if (kill)
            return false;

        if (Utils::interval_hit(this->ma_pattern.time_division, time))
        {
            uint8_t value = 64;
            this->tanzbar_modulators.ma_dec.value(time, value);
            MidiIO::send_cc(
                TB_MA_Decay,
                Utils::quad(value) / 2,
                MIDI_CC_CHANNEL_TANZBAR
            );

            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_MA, Utils::rerange(this->ma_pattern.value(time), 96, 16)),
                time.get_shuffle_delay(this->tanzbar_time.ma)
            );
            return true;
        }
        return false;
    }

    void play_hats_closed()
    {
        uint8_t velocity = 63;

        switch (this->hat_closed_style)
        {
        case HatClosedStyle::HatClosedInterval:
            this->tanzbar_modulators.hats_vel.value(time, velocity);

            if (this->hat_int_pattern.hit(time))
            {
                this->storage.note_on(
                    NoteStruct(NOTE_TANZBAR_HH, velocity),
                    time.get_shuffle_delay(this->tanzbar_time.hh)
                );
            }
            break;
        case HatClosedStyle::HatClosedRegular:
            if (this->hh_pattern.gate(time))
            {
                velocity = Utils::rerange(this->hat_velocity.value(time), 50, 32);
                this->storage.note_on(
                    NoteStruct(NOTE_TANZBAR_HH, velocity),
                    time.get_shuffle_delay(this->tanzbar_time.hh)
                );
            }
            break;
        }
    }

    bool play_hats_open()
    {
        if (this->kill)
            return false;

        uint8_t velocity = 63;
        if ((time.tick / TICKS_PER_STEP) % 4 == 2)
            velocity = 127;

        if (this->oh_pattern.gate(time))
        {
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_OH, velocity),
                time.get_shuffle_delay(this->tanzbar_time.hh)
            );
            return true;
        }
        return false;
    }

    void play_hats()
    {
        if (this->kill)
            return;

        if (!play_hats_open())
        {
            play_hats_closed();
        }
    }

    void play()
    {
        play_hats();
        play_maracas();

        // Play Cymbal
        if (this->cy_pattern.gate(time) && !kill)
        {
            uint8_t value = 0;
            if (this->tanzbar_modulators.cy_tune.value(time, value))
            {
                MidiIO::send_cc(TB_CY_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            uint8_t velocity = 100;
            this->tanzbar_modulators.cy_vel.value(time, velocity);
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_CY, 64),
                time.get_shuffle_delay(this->tanzbar_time.cy)
            );
        }
    }
};