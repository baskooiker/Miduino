#pragma once

#include "instruments/instrument_group.h"
#include "instruments/drums/kick.h"
#include "instruments/drums/snare.h"
#include "instruments/drums/hats.h"
#include "instruments/drums/toms.h"
#include "instruments/drums/percussion.h"
#include "instruments/drums/cymbal.h"


// TR808 Kit
// Bass Drum
#define DS_808_BD_FRONT CUT 20
#define DS_808_BD_PAN 21
#define DS_808_BD_DISTORTION 22
#define DS_808_BD_TUNE 23
#define DS_808_BD_TONE 24
#define DS_808_BD_DECAY 25

// SNARE DRUM
#define DS_808_SD_FRONT CUT 26
#define DS_808_SD_PAN 27
#define DS_808_SD_DISTORTION 28
#define DS_808_SD_TUNE 29
#define DS_808_SD_TONE 30
#define DS_808_SD_SNAPPY 31

// LOW TOM(1)
#define DS_808_LT_FRONT CUT 32
#define DS_808_LT_PAN 33
#define DS_808_LT_DISTORTION 34
#define DS_808_LT_TUNE 35
#define DS_808_LT_DECAY 36

// MID TOM(2)
#define DS_808_MT_FRONT CUT 37
#define DS_808_MT_PAN 38
#define DS_808_MT_DISTORTION 39
#define DS_808_MT_TUNE 40
#define DS_808_MT_DECAY 41

// HIGH TOM(3)
#define DS_808_HT_FRONT CUT 42
#define DS_808_HT_PAN 43
#define DS_808_HT_DISTORTION 44
#define DS_808_HT_TUNE 45
#define DS_808_HT_DECAY 46
#define DS_808_HT_RIM SHOT 
//#define DS_808_HT_PAN 47
//#define DS_808_HT_TUNE 48

// HAND CLAP
#define DS_808_CP_PAN 49
#define DS_808_CP_TUNE 50

// COWBELL
#define DS_808_CB_PAN 51
#define DS_808_CB_DISTORTION 52
#define DS_808_CB_TUNE 53

// CLOSED HIHAT
#define DS_808_HH_PAN 54
#define DS_808_HH_TUNE 55
#define DS_808_HH_DECAY 56

// OPEN HIHAT
#define DS_808_OH_PAN 57
#define DS_808_OH_TUNE 58
#define DS_808_OH_DECAY 59

// CRASH CYMBAL
#define DS_808_CRASH_PAN 60
#define DS_808_CRASH_TONE 61
#define DS_808_CRASH_DECAY 62

// LOW CONGA(1)
#define DS_808_LC_PAN 63
#define DS_808_LC_DISTORTION 64
#define DS_808_LC_TUNE 65

// MID CONGA(2)
#define DS_808_MC_PAN 66
#define DS_808_MC_DISTORTION 67
#define DS_808_MC_TUNE 68

// HIGH CONGA(3)
#define DS_808_HC_PAN 69
#define DS_808_HC_DISTORTION 70
#define DS_808_HC_TUNE 71

// MARACAS
#define DS_808_MA_PAN 72
#define DS_808_MA_TUNE 73

// CLAVES
#define DS_808_CL_PAN 74
#define DS_808_CL_TUNE 75

// TR909 KIT
// BASS DRUM
#define DS_909_BD_FRONT CUT 105
#define DS_909_BD_PAN 106
#define DS_909_BD_DISTORTION 107
#define DS_909_BD_TUNE 76
#define DS_909_BD_ATTACK 77
#define DS_909_BD_DECAY 78

// SNARE DRUM
#define DS_909_SD_FRONT CUT 108
#define DS_909_SD_PAN 109
#define DS_909_SD_DISTORTION 110
#define DS_909_SD_TUNE 79
#define DS_909_SD_TONE 80
#define DS_909_SD_SNAPPY 81

// LOW TOM
#define DS_909_LT_FRONT CUT 82
#define DS_909_LT_PAN 83
#define DS_909_LT_DISTORTION 84
#define DS_909_LT_TUNE 85
#define DS_909_LT_DECAY 86

// MID TOM
#define DS_909_MT_FRONT CUT 87
#define DS_909_MT_PAN 88
#define DS_909_MT_DISTORTION 89
#define DS_909_MT_TUNE 90
#define DS_909_MT_DECAY 91

// HIGH TOM
#define DS_909_HT_FRONT CUT 92
#define DS_909_HT_PAN 93
#define DS_909_HT_DISTORTION 94
#define DS_909_HT_TUNE 95
#define DS_909_HT_DECAY 96

// RIM SHOT
#define DS_909_RS_PAN 97
#define DS_909_RS_TUNE 98

// HAND CLAP
#define DS_909_CP_PAN 99
#define DS_909_CP_TUNE 100

// CLOSED HIHAT
#define DS_909_HH_PAN 111
#define DS_909_HH_DISTORTION 101
#define DS_909_HH_TUNE 102
#define DS_909_HH_DECAY 103

// OPEN HIHAT
#define DS_909_OH_PAN 112
#define DS_909_OH_TUNE 104
#define DS_909_OH_DECAY 113

// CRASH CYMBAL
#define DS_909_CRASH_PAN 114
#define DS_909_CRASH_TUNE 115
#define DS_909_CRASH_DECAY 116

// RIDE CYMBAL
#define DS_909_CY_PAN 117
#define DS_909_CY_TUNE 118
#define DS_909_CY_DECAY 119


namespace Vleerhond
{
class DrumStationKick : public Kick
{
public:
    DrumStationKick(Modulators& modulators, TimeStruct& time);
};

class DrumStationSnare : public Snare
{
public:
    DrumStationSnare(Modulators& modulators, TimeStruct& time);
};

class DrumStationRimshot : public Percussion
{
public:
    DrumStationRimshot(Modulators& modulators, TimeStruct& time);
};

class DrumStationHats : public Hats
{
public:
    DrumStationHats(Modulators& modulators, TimeStruct& time);
};

class DrumStationMaracas : public Hats
{
public:
    DrumStationMaracas(Modulators& modulators, TimeStruct& time);
};

class DrumStationClap : public Snare
{
public:
    DrumStationClap(Modulators& modulators, TimeStruct& time);
};

class DrumStationToms : public Toms
{
public:
    DrumStationToms(Modulators& modulators, TimeStruct& time);

    virtual void randomize();

};

class DrumStationClave : public Percussion
{
public:
    DrumStationClave(Modulators& modulators, TimeStruct& time);
};

class DrumStationCowbell : public Percussion
{
public:
    DrumStationCowbell(Modulators& modulators, TimeStruct& time);
};

class DrumStationCymbal : public Cymbal
{
public:
    DrumStationCymbal(Modulators& modulators, TimeStruct& time);
};

class DrumStation : public InstrumentGroup
{
public:
    DrumStationSnare snare;
    DrumStationRimshot rimshot;
    DrumStationClap clap;
    DrumStationHats hats;
    DrumStationToms toms;
    DrumStationClave clave;
    DrumStationCowbell cowbell;
    DrumStationCymbal cymbal;
    DrumStationMaracas maracas;

    DrumStation(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);

    std::vector<InstrumentBase*> getLow();
    std::vector<InstrumentBase*> getMid();
    std::vector<InstrumentBase*> getHigh();
    void killLow(const bool kill);
    void killMid(const bool kill);
    void killHigh(const bool kill);

    void reset();
};
}
