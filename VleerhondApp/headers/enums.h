#pragma once

namespace Vleerhond
{
    enum class Root {
        ROOT_C = 0,
        ROOT_C_SHARP,
        ROOT_D,
        ROOT_D_SHARP,
        ROOT_E,
        ROOT_F,
        ROOT_F_SHARP,
        ROOT_G,
        ROOT_G_SHARP,
        ROOT_A,
        ROOT_A_SHARP,
        ROOT_B
    };

    class Strings 
    {
    public:

        static std::string get_string(const ScaleType scale)
        {
            switch (scale)
            {
            case ScaleType::AEOLIAN:
                return "aeolian";
            case ScaleType::DORIAN:
                return "dorian";
            case ScaleType::IONIAN:
                return "ionian";
            default:
                return "unkown scale";
            }
        }

        static std::string get_string(const Root root)
        {
            switch (root)
            {
            case Root::ROOT_C:
                return "C";
            case Root::ROOT_C_SHARP:
                return "C#";
            case Root::ROOT_D:
                return "D";
            case Root::ROOT_D_SHARP:
                return "D#";
            case Root::ROOT_E:
                return "E";
            case Root::ROOT_F:
                return "F";
            case Root::ROOT_F_SHARP:
                return "F#";
            case Root::ROOT_G:
                return "G";
            case Root::ROOT_G_SHARP:
                return "G#";
            case Root::ROOT_A:
                return "A";
            case Root::ROOT_A_SHARP:
                return "A#";
            case Root::ROOT_B:
                return "B";
            default:
                return "unkown root";
            }
        }
    };

    enum TimeDivision {
        Thirtysecond = 3,
        Sixteenth = 6,
        TripletEight = 8,
        Eighth = 12,
        DottedEight = 18,
        Quarter = 24,
        DottedQuarter = 36,
        Half = 48,
        Whole = 96,
        Two = 192,
        Four = 384,
        Eight = 768
    };

    enum NoteRange {
        RangeRoot,
        RangeChord,
        RangeScale
    };

    enum BassStyle {
        BassLow,
        BassArpInterval,
        BassEuclid,
        BassFugue
    };

    enum BassDubStyle {
        DubUnison,
        DubOctave,
        DubOctProbability,
        DubFugue
    };

    enum NoteInterval {
        IntervalRoot = 0,
        IntervalThird = 2,
        IntervalFifth = 4
    };

    enum ArpType {
        UP,
        DOWN,
        UPDOWN,
        PICKING_IN,
        CLOSEST,
        CLOSEST_EXC,
        RANDOM
    };

    enum RangeType {
        Range,
        Count
    };

    enum LeadStyle {
        //LeadSixteenths,
        //LeadLongPattern,
        //LeadIntPattern,
        LeadSlow,
        LeadWhole
    };

    enum PolyType {
        PolyLow,
        PolyHigh
    };

    enum HatClosedStyle {
        HatClosedInterval,
        HatClosedRegular
    };

    enum MonoStyle {
        MonoSixteenths,
        MonoPolyRhythm,
        MonoLeadPattern,
        MonoFugue
    };

    enum MonoDubStyle {
        MonoDubLead,
        MonoDubUnison
    };

    enum HarmonyType {
        HarmonyConst,
        HarmonyHigh
    };

    enum class NoteType {
        Normal,
        Tie,
        Slide
    };

    enum class PlayState {
        Playing,
        Stopped
    };

    enum FuguePlayerType {
        FugueForward,
        FugueBackward,
        FugueBackAndForth
    };

    enum ScaleType {
        IONIAN,
        DORIAN,
        AEOLIAN
    };
}