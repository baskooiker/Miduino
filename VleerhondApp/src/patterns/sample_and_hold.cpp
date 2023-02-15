#include "patterns/sample_and_hold.h"

#include "utils/rand.h"
#include "utils/utils.h"

namespace Vleerhond {
SampleAndHold::SampleAndHold(const TimeDivision div) { interval = div; }

uint8_t SampleAndHold::value(const TimeStruct& time) {
    randomize(time);
    return sampled_value;
}

bool SampleAndHold::gate(const TimeStruct& time) {
    randomize(time);
    return value(time) < prob;
}

void SampleAndHold::randomize(const TimeStruct time) {
    if (!Utils::intervalHit(interval, time)) {
        return;
    }
    if (last_randomized >= time.tick) {
        return;
    }
    last_randomized = time.tick;

    this->sampled_value = Rand::randui8();
}

NoteRepeat::NoteRepeat(const TimeDivision div) : SampleAndHold(div) {
    interval_value = TimeDivision::Sixteenth;
}

TimeDivision NoteRepeat::getInterval(const TimeStruct& time) {
    if (Utils::intervalHit(interval, time)) {
        randomize(time);
    }
    return interval_value;
}

NoteStruct NoteRepeat::repeatNote(const TimeStruct time) {
    randomize(time);
    if (gate(time)) {
        return repeated_note;
    } else {
        if (repeated_note.pitch > 0) {
            repeated_note = NoteStruct();
        }
        return repeated_note;
    }
}

void NoteRepeat::setRepeatNote(const NoteStruct note) {
    if (repeated_note.pitch == 0) {
        repeated_note = note;
    }
}

void NoteRepeat::randomize(const TimeStruct& time) {
    if (!Utils::intervalHit(interval, time)) {
        return;
    }
    if (last_randomized >= time.tick) {
        return;
    }

    SampleAndHold::randomize(time);

    switch (Rand::distribution(16, 0)) {
        case 0:
            this->interval_value = TimeDivision::Thirtysecond;
            break;
        case 1:
            this->interval_value = TimeDivision::Sixteenth;
            break;
    }

    repeated_note = NoteStruct();
}
}  // namespace Vleerhond
