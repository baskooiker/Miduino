#pragma once

#include "bass.h"
#include "patterns/modulators.h"

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
    class MoogBass : public Bass
    {
    protected:
        ModulationReceiver vel_mod;

    public:
        MoogBass(
            Modulators& modulators,
            HarmonyStruct& harmony,
            TimeStruct& time
        );
        virtual void randomize();
        virtual void total_randomize();
        virtual uint8_t getVelocity();

        void setVco2Square(const bool square);
    };
}
