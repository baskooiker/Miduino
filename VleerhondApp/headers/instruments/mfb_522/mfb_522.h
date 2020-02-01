#pragma once

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
    class Mfb522Kick : public Kick 
    {
    public:
        Mfb522Kick(Modulators& modulators, TimeStruct& time);
    };

    class Mfb522Snare : public Snare
    {
    public:
        Mfb522Snare(Modulators& modulators, TimeStruct& time);
    };

    class Mfb522Rimshot : public Percussion
    {
    public:
        Mfb522Rimshot(Modulators& modulators, TimeStruct& time);
    };

    class Mfb522Hats : public Hats 
    {
    public:
        Mfb522Hats(Modulators& modulators, TimeStruct& time);
    };
    
    class Mfb522Clap : public Snare 
    {
    public:
        Mfb522Clap(Modulators& modulators, TimeStruct& time);
    };
    
    class Mfb522Toms : public Toms 
    {
    public:
        Mfb522Toms(Modulators& modulators, TimeStruct& time);

        virtual void randomize();

    };

    class Mfb522Clave : public Percussion
    {
    public:
        Mfb522Clave(Modulators& modulators, TimeStruct& time);
    };

    class Mfb522Cowbell : public Percussion
    {
    public:
        Mfb522Cowbell(Modulators& modulators, TimeStruct& time);
    };

    class Mfb522Cymbal: public Cymbal
    {
    public:
        Mfb522Cymbal(Modulators& modulators, TimeStruct& time);
    };

class Mfb522 : public InstrumentGroup
{
public:
    Mfb522Kick kick;
    Mfb522Snare snare;
    Mfb522Rimshot rimshot;
    Mfb522Clap clap;
    Mfb522Hats hats;
    Mfb522Toms toms;
    Mfb522Clave clave;
    Mfb522Cowbell cowbell;
    Mfb522Cymbal cymbal;

    Mfb522(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);

    std::vector<InstrumentBase*> getLow();
    std::vector<InstrumentBase*> getMid();
    std::vector<InstrumentBase*> getHigh();
    void killLow(const bool kill);
    void killMid(const bool kill);
    void killHigh(const bool kill);
};
}
