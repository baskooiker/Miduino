#include "rocket_bass.h"

namespace Vleerhond
{
    RocketBass::RocketBass(
        Modulators& modulators,
        HarmonyStruct& harmony,
        TimeStruct& time) :
        Bass(harmony, time),
        vel_mod(modulators)
    {
        midi_channel.set_channel(MIDI_CHANNEL_MINITAUR);

        params.push_back({ MINITAUR_CC_LFO_MIDI_SYNC  ,   0,  63, MIDI_CHANNEL_MINITAUR }); // MidiSync on
        params.push_back({ MINITAUR_CC_LFO_KEY_TRIGGER,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_MOD_SOURCE     ,   0,  64, MIDI_CHANNEL_MINITAUR }); // No ADSR or SnH
        params.push_back({ MINITAUR_CC_VCO1_WAVE      ,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_VCO2_WAVE      ,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_VCO2_HARD_SYNC ,  64, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_VCO2_MODULATION,  64, 127, MIDI_CHANNEL_MINITAUR }); // Pitch mod only on VCO2
        params.push_back({ MINITAUR_CC_GLIDE_TYPE     ,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_LEGATO_GLIDE   ,  64, 127, MIDI_CHANNEL_MINITAUR }); // Only on legato notes
        params.push_back({ MINITAUR_CC_FILTER_VELOCITY,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_VOLUME_VELOCITY,   0, 127, MIDI_CHANNEL_MINITAUR });
        

        settings.p_euclid_16 = 16;
        settings.p_euclid_8 = 8;
        settings.p_interval = 8;
        settings.p_diddles = 0;

        settings.euclid_16.p_5 = 16;
        settings.euclid_16.p_6 = 16;
        settings.euclid_16.p_7 = 8;
        settings.euclid_16.p_9 = 0;
        settings.euclid_16.p_11 = 0;

        settings.euclid_8.p_3 = 16;
        settings.euclid_8.p_4 = 16;

        settings.diddles.p_min = .6;
        settings.diddles.p_max = .9;

        settings.p_octave_sh = 0;
    }

    void RocketBass::randomize()
    {
        Bass::randomize();
        uint8_t range = Rand::randui8(64);
        vel_mod.randomize(range, 64);
    }

    void RocketBass::total_randomize()
    {
        randomize_parameters();
        Bass::total_randomize();
    }

    uint8_t RocketBass::get_velocity()
    {
        uint8_t value = 100;
        //vel_mod.value(time, value);

        value = this->accents.gate(this->time) ? 127 : 64;
        return value;
    }
}
