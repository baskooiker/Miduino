#pragma once

enum Root {
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
    Eight = 12,
    DottedEight = 18,
    Quarter = 24,
    DottedQuarter = 36,
    Half = 48,
    Whole = 96,
};

enum NoteRange {
    RangeRoot,
    RangeChord,
    RangeScale
};

enum RocketStyle {
    //RocketWhole,
    RocketLow,
    RocketSixteenths,
    //RocketProb,
    RocketArpInterval,
    RocketEuclid
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

enum HatStyle {
    HatOffBeat,
    HatFull
};

enum MonoStyle {
    Sixteenths,
    PolyRhythm,
    LeadPattern
};

enum HarmonyType {
    Const,
    Low,
    High
};

enum NoteType {
    Normal,
    Tie,
    Slide
};
