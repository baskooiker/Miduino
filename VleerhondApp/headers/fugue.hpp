#pragma once

#include "defs.hpp"
#include "harmony_struct.hpp"
#include "midi_channel.hpp"

namespace Vleerhond
{
    class Fugue 
    {
    public:
        CvPattern16 pattern;
        //std::vector<FuguePlayer*> players;

        void randomize_fugue()
        {
            // Randomize Fugue pattern
            static uint8_t pitch_opts[] = { 0, 4, 2, 3, 6 };
            this->pattern.length = 0;
            uint8_t length = Rand::randui8(64) < 32 ? 4 : 8;
            while (this->pattern.length < length)
            {
                Utils::randomize_order(pitch_opts, 5);
                for (int i = 0; i < 5 && this->pattern.length < length; i++)
                {
                    this->pattern.pattern.set(this->pattern.length++, pitch_opts[i]);
                }
            }

            //for (auto player: players)
            //{
            //    player->randomize();
            //}
        }

        //void reset()
        //{
        //    for (FuguePlayer* player: players)
        //    {
        //        player->counter = 0;
        //    }
        //}

    };
}