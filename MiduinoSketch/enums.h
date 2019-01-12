enum ControlMode
{
    CONTROL_MODE_NORMAL,
    CONTROL_MODE_ROOT
};

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
    TIME_DIVISION_THIRTYTWO = 3,
    TIME_DIVISION_SIXTEENTH = 6,
    TIME_DIVISION_EIGHT = 12,
    TIME_DIVISION_FOURTH = 24,
    TIME_DIVISION_HALF = 48,
    TIME_DIVISION_WHOLE = 96,
    TIME_DIVISION_TRIPLE_EIGHT = 8
};

enum NoteRange {
    RangeRoot,
    RangeChord,
    RangeScale
};

enum RocketStyle {
    RocketWhole,
    RocketSixteenths,
    RocketProb,
    RocketArpInterval,
    RocketEuclid
};

enum ArpType {
    UP,
    DOWN,
    UPDOWN,
    PICKING_IN,
    CLOSEST,
    RANDOM
};

enum LeadStyle {
    LeadSixteenths,
    LeadIntPattern
};

enum PolyType {
    PolyOff,
    PolyLow,
    PolyHigh
};