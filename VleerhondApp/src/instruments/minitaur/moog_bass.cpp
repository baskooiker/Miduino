#include "instruments/minitaur/moog_bass.h"

namespace Vleerhond
{
    MoogBass::MoogBass(
        Modulators& modulators,
        HarmonyStruct& harmony,
        TimeStruct& time) :
        Bass(harmony, time),
        vel_mod(modulators)
    {
        params.push_back({ MINITAUR_CC_LFO_MIDI_SYNC  ,   0, 127 });
        params.push_back({ MINITAUR_CC_LFO_KEY_TRIGGER,   0, 127 });
        params.push_back({ MINITAUR_CC_MOD_SOURCE     ,  16,  16 }); // Only sine
        params.push_back({ MINITAUR_CC_VCO1_WAVE      ,   0, 127 });
        params.push_back({ MINITAUR_CC_VCO2_WAVE      ,   0, 127 });
        params.push_back({ MINITAUR_CC_VCO2_HARD_SYNC ,   0,   1 });
        params.push_back({ MINITAUR_CC_VCO2_MODULATION,  64, 127 }); // Pitch mod only on VCO2
        params.push_back({ MINITAUR_CC_GLIDE_TYPE     ,   0, 127 });
        params.push_back({ MINITAUR_CC_LEGATO_GLIDE   ,  64, 127 }); // Only on legato notes
        params.push_back({ MINITAUR_CC_FILTER_VELOCITY,  64,  64 });
        params.push_back({ MINITAUR_CC_VOLUME_VELOCITY,  64,  64 });
        
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

        settings.default_note_length = 12;
    }

    void MoogBass::randomize()
    {
        Bass::randomize();
        uint8_t range = Rand::randui8(64);
        vel_mod.randomize(range, 64);
    }

    void MoogBass::totalRandomize()
    {
        Bass::totalRandomize();
    }

    uint8_t MoogBass::getVelocity()
    {
        return this->accents.gate(this->time) ? 127 : 64;
    }
    void MoogBass::setVco2Square(const bool square)
    {
        getChannel()->sendCC(MINITAUR_CC_VCO2_WAVE, square ? 127 : 0);
    }
}
