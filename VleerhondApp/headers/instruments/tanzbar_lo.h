#pragma once

namespace Vleerhond
{
    const RandomParam tanzbar_low_params[] = {
        {TB_BD1_ATTACK   ,  0, 127},
        {TB_BD1_DECAY    , 32,  96},
        {TB_BD1_PITCH    , 80, 112},
        {TB_BD1_TUNE     , 32,  96},
        {TB_BD1_NOISE    ,  0,  64},
        {TB_BD1_FILTER   ,  0,  64},
        {TB_BD1_DIST     ,  0, 127},
        {TB_BD1_TRIGGER  ,  0, 127},

        {TB_BD2_DECAY    , 32,  96},
        {TB_BD2_TUNE     , 32,  96},
        {TB_BD2_TONE     , 32,  96}
    };
    const uint8_t nr_tanzbar_low_params = sizeof(tanzbar_low_params) / sizeof(RandomParam);

    class TanzbarLo : public InstrumentBase
    {
    protected:
        MicroTimingStruct bd_timing;

    public:
        GatePatternAB bd_pattern;

        TanzbarLo(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true)
        {
            storage.set_channel(MIDI_CHANNEL_TANZBAR);
        }

        void randomize()
        {
            ofLogNotice("tanzbar_lo", "randomize()");
            last_randomized_time = millis();

            Tanzbar::randomize_parameters(tanzbar_low_params, nr_tanzbar_low_params);

            this->bd_pattern.set_coef_kick_pattern();

            if (Rand::distribution(32, 12))
            {
                this->randomize_tanzbar_kick();
            }

            this->bd_timing.randomize();
        }

        void play()
        {
            bool quarter_hit = Utils::interval_hit(TimeDivision::Quarter, time);
            uint8_t velocity = quarter_hit ? 127 : 63;
            if (this->bd_pattern.gate(time) && !this->kill)
            {
                uint8_t pitch = NOTE_TANZBAR_BD1;
                this->storage.note_on(
                    NoteStruct(pitch, velocity),
                    time.get_shuffle_delay(this->bd_timing)
                );
            }

            if (quarter_hit && !this->kill)
            {
                this->storage.note_on(
                    NoteStruct(NOTE_TANZBAR_BD2, velocity),
                    time.get_shuffle_delay(this->bd_timing)
                );
            }
        }

    protected:
        void randomize_tanzbar_kick()
        {
            // Fill in first or second half of bar
            uint8_t half = Rand::distribution(64, 64);

            uint8_t bar = this->bd_pattern.abPattern.value(Rand::randui8(4));
            this->bd_pattern.patterns[bar].set_kick_fill(half * 8);
        }

    };
}