#pragma once

namespace Vleerhond {
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

enum NoteRange { RangeRoot, RangeChord, RangeScale };

enum class BassStyle { BassArpInterval, BassEuclid };

enum class BassDubStyle { DubUnison, DubOctave, DubOctProbability };

enum class NoteInterval {
    IntervalRoot = 0,
    IntervalThird = 2,
    IntervalFifth = 4
};

enum class ArpType {
    UP,
    DOWN,
    UPDOWN,
    PICKING_IN,
    CLOSEST,
    CLOSEST_EXC,
    RANDOM
};

enum RangeType { Range, Count };

enum LeadStyle { LeadSlow, LeadWhole };

enum PolyType { PolyLow, PolyHigh };

enum HatClosedStyle { HatClosedInterval, HatClosedRegular };

enum class MonoStyle { MonoSixteenths, MonoPolyRhythm };

enum class HarmonyType {
    Const,
    TonicLow,
    TonicHigh,
    DominantLow,
    DominantHigh,
};

enum class NoteType { Normal, Tie };

enum class PlayState { Playing, Stopped };

enum FuguePlayerType { FugueForward, FugueBackward, FugueBackAndForth };

enum class ScaleType { IONIAN, DORIAN, AEOLIAN };
}  // namespace Vleerhond