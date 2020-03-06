#pragma once

#include "instruments/drums/percussion.h"
#include "patterns/modulators.h"

namespace Vleerhond
{
    class TanzbarCl : public Percussion
    {
    protected:
        ModulationReceiver cl_pitch;

    public:

        TanzbarCl(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Percussion(modulators_ref, time_ref),
            cl_pitch(modulators_ref)
        {
            pitch = NOTE_TANZBAR_CL;

            this->params.push_back(CcParam(TB_CL_TUNE, 0, 127));
            this->params.push_back(CcParam(TB_CL_DECAY, 80, 127));
        }

        void randomize()
        {
            ofLogNotice("tanzbar_perc", "randomize()");
            Percussion::randomize();

            // Modulators
            uint8_t range = Rand::randui8(128, 64);
            this->cl_pitch.randomize(range, 127 - range, .3);
        }

        bool play()
        {
            uint8_t value = 0;
            if (this->cl_pitch.value(time, value))
            {
                getChannel()->sendCC(TB_CL_TUNE, value);
            }
            return Percussion::play();
        }
    };
}