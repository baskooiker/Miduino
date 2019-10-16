#pragma once

#include "instrument_group.h"
#include "modulators.h"

#include "kick.h"
#include "snare.h"
#include "hats.h"
#include "toms.h"
#include "percussion.h"
#include "cymbal.h"

namespace Vleerhond
{
    class Mfb522Kick : public Kick 
    {
    public:
        Mfb522Kick(Modulators& modulators, TimeStruct& time) :
            Kick(modulators, time)
        {
            // TODO: long & short
            pitch = NOTE_522_BD_LONG;
        }
    };
    
    class Mfb522Snare : public Snare 
    {
    public:
        Mfb522Snare(Modulators& modulators, TimeStruct& time) :
            Snare(modulators, time)
        {
            pitch = NOTE_522_SN;
        }
    };
    
    class Mfb522Hats : public Hats 
    {
    public:
        Mfb522Hats(Modulators& modulators, TimeStruct& time) :
            Hats(modulators, time)
        {
            pitch_open = NOTE_522_OH;
            pitch_closed = NOTE_522_HH;
        }
    };
    
    class Mfb522Clap : public Snare 
    {
    public:
        Mfb522Clap(Modulators& modulators, TimeStruct& time) :
            Snare(modulators, time)
        {
            // TODO: long & short
            pitch = NOTE_522_CP_LONG;
        }
    };
    
    class Mfb522Toms : public Toms 
    {
    public:
        Mfb522Toms(Modulators& modulators, TimeStruct& time) :
            Toms(modulators, time)
        {

            pitches = {
                NOTE_522_LO_TOM,
                NOTE_522_MI_TOM,
                NOTE_522_HI_TOM,
                NOTE_522_HI2_TOM
            };
        }
    };

    class Mfb522Clave : public Percussion
    {
    public:
        Mfb522Clave(Modulators& modulators, TimeStruct& time) :
            Percussion(modulators, time)
        {
            pitch = NOTE_522_CLAVE;
        }
    };

    class Mfb522Cowbell : public Percussion
    {
    public:
        Mfb522Cowbell(Modulators& modulators, TimeStruct& time) :
            Percussion(modulators, time)
        {
            pitch = NOTE_522_CB;
        }
    };

    class Mfb522Cymbal: public Cymbal
    {
    public:
        Mfb522Cymbal(Modulators& modulators, TimeStruct& time) :
            Cymbal(modulators, time)
        {
            pitch = NOTE_522_CYMBAL;
        }
    };

class Mfb522 : public InstrumentGroup
{
public:
    Mfb522Kick kick;
    Mfb522Snare snare;
    Mfb522Clap clap;
    Mfb522Hats hats;
    Mfb522Toms toms;
    Mfb522Clave clave;
    Mfb522Cowbell cowbell;
    Mfb522Cymbal cymbal;

    Mfb522(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time) :
        InstrumentGroup(harmony, time),
        kick(modulators, time),
        snare(modulators, time),
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
};
}
