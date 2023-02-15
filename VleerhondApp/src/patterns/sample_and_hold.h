#pragma once

#include "core/defs.h"
#include "core/note_struct.h"
#include "core/time_struct.h"

namespace Vleerhond {
class SampleAndHold {
   protected:
    uint8_t sampled_value;
    TimeDivision interval;
    uint64_t last_randomized;

   public:
    uint8_t prob;

    SampleAndHold(const TimeDivision div = TimeDivision::Sixteenth);

    uint8_t value(const TimeStruct& time);

    bool gate(const TimeStruct& time);

   protected:
    void randomize(const TimeStruct time);
};

class NoteRepeat : public SampleAndHold {
   protected:
    TimeDivision interval_value;
    NoteStruct repeated_note;

   public:
    NoteRepeat(const TimeDivision div);

    TimeDivision getInterval(const TimeStruct& time);

    NoteStruct repeatNote(const TimeStruct time);

    void setRepeatNote(const NoteStruct note);

   protected:
    void randomize(const TimeStruct& time);
};

}  // namespace Vleerhond
