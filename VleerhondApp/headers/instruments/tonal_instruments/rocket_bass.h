#pragma once

#include "bass.h"

#define MINITAUR_CC_LFO_MIDI_SYNC   87
#define MINITAUR_CC_LFO_KEY_TRIGGER 82
#define MINITAUR_CC_MOD_SOURCE      85

#define MINITAUR_CC_VCO1_WAVE       70
#define MINITAUR_CC_VCO2_WAVE       71
#define MINITAUR_CC_VCO2_HARD_SYNC  80
#define MINITAUR_CC_VCO2_MODULATION 112
#define MINITAUR_CC_GLIDE_TYPE      92
#define MINITAUR_CC_LEGATO_GLIDE    83

#define MINITAUR_CC_FILTER_VELOCITY 89
#define MINITAUR_CC_VOLUME_VELOCITY 90

namespace Vleerhond
{
    class RocketBass : public Bass
    {
    protected:
        ModulationReceiver vel_mod;

    public:
        RocketBass(
            Modulators& modulators,
            HarmonyStruct& harmony,
            TimeStruct& time) :
            Bass(harmony, time),
            vel_mod(modulators)
        {
            midi_channel.set_channel(MIDI_CHANNEL_MINITAUR);

            params.push_back({ MINITAUR_CC_LFO_MIDI_SYNC  ,   0,  63, MIDI_CHANNEL_MINITAUR }); // MidiSync off
            params.push_back({ MINITAUR_CC_LFO_KEY_TRIGGER,   0, 127, MIDI_CHANNEL_MINITAUR });
            params.push_back({ MINITAUR_CC_MOD_SOURCE     ,   0,  64, MIDI_CHANNEL_MINITAUR }); // No ADSR or SnH
            params.push_back({ MINITAUR_CC_VCO1_WAVE      ,   0, 127, MIDI_CHANNEL_MINITAUR });
            params.push_back({ MINITAUR_CC_VCO2_WAVE      ,   0, 127, MIDI_CHANNEL_MINITAUR });
            params.push_back({ MINITAUR_CC_VCO2_HARD_SYNC ,   0, 127, MIDI_CHANNEL_MINITAUR });
            params.push_back({ MINITAUR_CC_VCO2_MODULATION,  64, 127, MIDI_CHANNEL_MINITAUR }); // Pitch mod only on VCO2
            params.push_back({ MINITAUR_CC_GLIDE_TYPE     ,   0, 127, MIDI_CHANNEL_MINITAUR });
            params.push_back({ MINITAUR_CC_LEGATO_GLIDE   ,  64, 127, MIDI_CHANNEL_MINITAUR }); // Only on legato notes
            params.push_back({ MINITAUR_CC_FILTER_VELOCITY,   0, 127, MIDI_CHANNEL_MINITAUR });
            params.push_back({ MINITAUR_CC_VOLUME_VELOCITY,   0, 127, MIDI_CHANNEL_MINITAUR });
        }

        virtual void randomize()
        {
            Bass::randomize();
            uint8_t range = Rand::randui8(32);
            vel_mod.randomize(range, 64);
        }

        virtual void total_randomize()
        {
            randomize_parameters();
            Bass::total_randomize();
        }

        virtual uint8_t get_velocity()
        {
            uint8_t value = 100;
            //vel_mod.value(time, value);

            value = this->accents.gate(this->time) ? 127 : 64;
            return value;
        }
    };
}
