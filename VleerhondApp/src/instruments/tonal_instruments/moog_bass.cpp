#include "moog_bass.h"

namespace Vleerhond
{
    MoogBass::MoogBass(
        Modulators& modulators,
        HarmonyStruct& harmony,
        TimeStruct& time) :
        Bass(harmony, time, MIDI_CHANNEL_MINITAUR),
        vel_mod(modulators)
    {
        params.push_back({ MINITAUR_CC_LFO_MIDI_SYNC  ,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_LFO_KEY_TRIGGER,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_MOD_SOURCE     ,  16,  16, MIDI_CHANNEL_MINITAUR }); // Only sine
        params.push_back({ MINITAUR_CC_VCO1_WAVE      ,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_VCO2_WAVE      ,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_VCO2_HARD_SYNC ,   0,   1, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_VCO2_MODULATION,  64, 127, MIDI_CHANNEL_MINITAUR }); // Pitch mod only on VCO2
        params.push_back({ MINITAUR_CC_GLIDE_TYPE     ,   0, 127, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_LEGATO_GLIDE   ,  64, 127, MIDI_CHANNEL_MINITAUR }); // Only on legato notes
        params.push_back({ MINITAUR_CC_FILTER_VELOCITY,  64,  64, MIDI_CHANNEL_MINITAUR });
        params.push_back({ MINITAUR_CC_VOLUME_VELOCITY,  64,  64, MIDI_CHANNEL_MINITAUR });
        
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
        settings.euclid_8.p_5 = 8;
        settings.euclid_8.p_7 = 0;

        settings.diddles.p_min = .5;
        settings.diddles.p_max = .6;

        settings.p_octave_sh = 0;
    }

    void MoogBass::randomize()
    {
        Bass::randomize();
        uint8_t range = Rand::randui8(64);
        vel_mod.randomize(range, 64);
    }

    void MoogBass::total_randomize()
    {
        randomize_parameters();
        Bass::total_randomize();
    }

    uint8_t MoogBass::get_velocity()
    {
        return this->accents.gate(this->time) ? 127 : 64;
    }
}
