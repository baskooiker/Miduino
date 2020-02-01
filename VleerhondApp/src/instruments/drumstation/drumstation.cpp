#include "instruments/drumstation/drumstation.h"

#include "midi/midi_io.h"

#define NOTE_DS909_BD 24
#define NOTE_DS909_RS 25
#define NOTE_DS909_SD 26
#define NOTE_DS909_CP 27
#define NOTE_DS909_LT 28
#define NOTE_DS909_MT 29
#define NOTE_DS909_HH 30
#define NOTE_DS909_HT 31
#define NOTE_DS909_CR 32
#define NOTE_DS909_CY 33
#define NOTE_DS909_OH 34
#define NOTE_DS808_BD 36
#define NOTE_DS808_RS 37
#define NOTE_DS808_SD 38
#define NOTE_DS808_CP 39
#define NOTE_DS808_SD 40
#define NOTE_DS808_LT 41
#define NOTE_DS808_HH 42
#define NOTE_DS808_LT 43
#define NOTE_DS808_HH 44
#define NOTE_DS808_MT 45
#define NOTE_DS808_OH 46
#define NOTE_DS808_MT 47
#define NOTE_DS808_HT 48
#define NOTE_DS808_CR 49
#define NOTE_DS808_HT 50
#define NOTE_DS909_CY 51
#define NOTE_DS808_CB 56
#define NOTE_DS808_HC 62
#define NOTE_DS808_MC 63
#define NOTE_DS808_LC 64
#define NOTE_DS808_MA 70
#define NOTE_DS808_CL 75

namespace Vleerhond
{
    DrumStationKick::DrumStationKick(Modulators& modulators, TimeStruct& time)
        : Kick(modulators, time, MIDI_CHANNEL_DS)
    {
        // TODO: long & short
        pitch = NOTE_DS808_BD;
    }

    DrumStationSnare::DrumStationSnare(Modulators& modulators, TimeStruct& time)
        : Snare(modulators, time, MIDI_CHANNEL_DS)
    {
        pitch = NOTE_DS808_SD;
        //settings.p_coef = 0;
        //settings.p_off = 0;
        settings.p_rand = 0;
    }

    DrumStationRimshot::DrumStationRimshot(Modulators& modulators, TimeStruct& time) :
        Percussion(modulators, time, MIDI_CHANNEL_DS)
    {
        pitch = NOTE_DS808_RS;
    }
    DrumStationHats::DrumStationHats(Modulators& modulators, TimeStruct& time) :
        Hats(modulators, time, MIDI_CHANNEL_DS)
    {
        pitch_open = NOTE_DS808_OH;
        pitch_closed = NOTE_DS808_HH;
        settings.velocity_range = 32;
        settings.velocity_offset = 96;
    }

    DrumStationClap::DrumStationClap(Modulators& modulators, TimeStruct& time) :
        Snare(modulators, time, MIDI_CHANNEL_DS)
    {
        // TODO: long & short
        pitch = NOTE_DS808_CP;
        settings.p_coef = 0;
        settings.p_off = 0;
    }

    DrumStationToms::DrumStationToms(Modulators& modulators, TimeStruct& time) :
        Toms(modulators, time, MIDI_CHANNEL_DS)
    {
        pitches = {
            NOTE_DS808_LT,
            NOTE_DS808_MT,
            NOTE_DS808_HT,
        };
    }

    void DrumStationToms::randomize()
    {
        // TODO: Add Conga's
        pitches = {
            NOTE_DS808_LT,
            NOTE_DS808_MT,
            NOTE_DS808_HT,
        };
        std::random_shuffle(pitches.begin(), pitches.end());
        const int length = Rand::randui8(2, 5);
        while (pitches.size() > length)
        {
            pitches.pop_back();
        }

        Toms::randomize();
    }

    DrumStationClave::DrumStationClave(Modulators& modulators, TimeStruct& time) :
        Percussion(modulators, time, MIDI_CHANNEL_DS)
    {
        pitch = NOTE_DS808_CL;
    }

    DrumStationCowbell::DrumStationCowbell(Modulators& modulators, TimeStruct& time) :
        Percussion(modulators, time, MIDI_CHANNEL_DS)
    {
        pitch = NOTE_DS808_CB;
    }

    DrumStationCymbal::DrumStationCymbal(Modulators& modulators, TimeStruct& time) :
        Cymbal(modulators, time, MIDI_CHANNEL_DS)
    {
        pitch = NOTE_DS909_CY;
    }

    DrumStation::DrumStation(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time) :
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
        //instruments.push_back(&toms);
        //instruments.push_back(&clave);
        //instruments.push_back(&cowbell);
        instruments.push_back(&cymbal);
    }

    std::vector<InstrumentBase*> DrumStation::getLow()
    {
        return {
            &kick
        };
    }

    std::vector<InstrumentBase*> DrumStation::getMid()
    {
        return {
            &snare,
            //&rimshot,
            &clap,
            &toms,
            //&clave,
            //&cowbell
        };
    }

    std::vector<InstrumentBase*> DrumStation::getHigh()
    {
        return {
            &hats,
            &cymbal
        };
    }

    void DrumStation::killLow(const bool kill)
    {
        for (auto p : getLow())
        {
            p->kill = kill;
        }
    }

    void DrumStation::killMid(const bool kill)
    {
        for (auto p : getMid())
        {
            p->kill = kill;
        }
    }

    void DrumStation::killHigh(const bool kill)
    {
        for (auto p : getHigh())
        {
            p->kill = kill;
        }
    }
    void DrumStation::reset()
    {
        std::vector<int> pan_params = {
            DS_909_BD_PAN,
            DS_909_SD_PAN,
            DS_909_RS_PAN,
            DS_909_CP_PAN,
            DS_909_HH_PAN,
            DS_909_OH_PAN,
            DS_909_CY_PAN,
            DS_909_CRASH_PAN,
            DS_909_LT_PAN,
            DS_909_HT_PAN,
            DS_909_MT_PAN,
        };

        MidiIO::sendProgramChange(MIDI_CHANNEL_DS, 8);
        for (const int p: pan_params)
        {
            MidiIO::send_cc(p, 64, MIDI_CHANNEL_DS);
        }

    }
}
