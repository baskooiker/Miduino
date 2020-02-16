#include "instruments/mfb_522/mfb_522.h"

#include <algorithm>

#include "instrument_group.h"
#include "patterns/modulators.h"

#include "kick.h"
#include "snare.h"
#include "hats.h"
#include "toms.h"
#include "percussion.h"
#include "cymbal.h"

namespace Vleerhond
{
    Mfb522Kick::Mfb522Kick(Modulators& modulators, TimeStruct& time) 
        : Kick(modulators, time, MIDI_CHANNEL_522)
    {
        // TODO: long & short
        pitch = NOTE_522_BD_LONG;
    }

    Mfb522Snare::Mfb522Snare(Modulators& modulators, TimeStruct& time) 
        : Snare(modulators, time, MIDI_CHANNEL_522)
    {
        pitch = NOTE_522_SN;
        //settings.p_coef = 0;
        //settings.p_off = 0;
        settings.p_rand = 0;
    }

    Mfb522Rimshot::Mfb522Rimshot(Modulators& modulators, TimeStruct& time) :
            Percussion(modulators, time, MIDI_CHANNEL_522)
        {
            pitch = NOTE_522_RS;
        }
    Mfb522Hats::Mfb522Hats(Modulators& modulators, TimeStruct& time) :
            Hats(modulators, time, MIDI_CHANNEL_522)
        {
            pitch_open = NOTE_522_OH;
            pitch_closed = NOTE_522_HH;
        }

    Mfb522Clap::Mfb522Clap(Modulators& modulators, TimeStruct& time) :
            Snare(modulators, time, MIDI_CHANNEL_522)
        {
            // TODO: long & short
            pitch = NOTE_522_CP_LONG;
            settings.p_coef = 0;
            settings.p_off = 0;
        }

    Mfb522Toms::Mfb522Toms(Modulators& modulators, TimeStruct& time) :
            Toms(modulators, time, MIDI_CHANNEL_522)
        {
            pitches = {
                NOTE_522_LO_TOM,
                NOTE_522_MI_TOM,
                NOTE_522_HI_TOM,
                NOTE_522_HI2_TOM
            };
        }

        void Mfb522Toms::randomize()
        {
            pitches = {
                NOTE_522_LO_TOM,
                NOTE_522_MI_TOM,
                NOTE_522_HI_TOM,
                NOTE_522_HI2_TOM
            };
            std::random_shuffle(pitches.begin(), pitches.end());
            const int length = Rand::randui8(2, 5);
            while (pitches.size() > length)
            {
                pitches.pop_back();
            }

            Toms::randomize();
        }

        Mfb522Clave::Mfb522Clave(Modulators& modulators, TimeStruct& time) :
            Percussion(modulators, time, MIDI_CHANNEL_522)
        {
            pitch = NOTE_522_CLAVE;
        }

        Mfb522Cowbell::Mfb522Cowbell(Modulators& modulators, TimeStruct& time) :
            Percussion(modulators, time, MIDI_CHANNEL_522)
        {
            pitch = NOTE_522_CB;
        }

        Mfb522Cymbal::Mfb522Cymbal(Modulators& modulators, TimeStruct& time) :
            Cymbal(modulators, time, MIDI_CHANNEL_522)
        {
            pitch = NOTE_522_CYMBAL;
        }

        Mfb522::Mfb522(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time) :
            InstrumentGroup(harmony, time),
            kick(modulators, time),
            snare(modulators, time),
            rimshot(modulators, time),
            clap(modulators, time),
            hats(modulators, time),
            toms(modulators, time),
            clave(modulators, time),
            cowbell(modulators, time),
            cymbal(modulators, time)
        {
            instruments.push_back(&kick);
            instruments.push_back(&snare);
            instruments.push_back(&clap);
            instruments.push_back(&hats);
            instruments.push_back(&toms);
            instruments.push_back(&clave);
            instruments.push_back(&cowbell);
            instruments.push_back(&cymbal);
        }

        std::vector<InstrumentBase*> Mfb522::getLow()
        {
            return {
                &kick
            };
        }

        std::vector<InstrumentBase*> Mfb522::getMid()
        {
            return {
                &snare,
                &rimshot,
                &clap,
                &toms,
                &clave,
                &cowbell
            };
        }

        std::vector<InstrumentBase*> Mfb522::getHigh()
        {
            return {
                &hats,
                &cymbal
            };
        }

        void Mfb522::killLow(const bool kill)
        {
            for (auto p : getLow())
            {
                p->kill(kill);
            }
        }

        void Mfb522::killMid(const bool kill)
        {
            for (auto p : getMid())
            {
                p->kill(kill);
            }
        }

        void Mfb522::killHigh(const bool kill)
        {
            for (auto p : getHigh())
            {
                p->kill(kill);
            }
        }
}
