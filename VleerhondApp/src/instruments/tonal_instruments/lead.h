#pragma once

#include "midi_channel.h"
#include "midi_io.h"
#include "patterns/arp.h"
#include "patterns/cv_patterns.h"
#include "patterns/gate_patterns.h"

namespace Vleerhond {
class LeadSettings : public TonalInstrumentBase {
   public:
    ArpData arp_data;
    CvPatternAB min_pitch_pattern;
    GatePatternAB pattern_slow;
    LeadStyle style;

    LeadSettings(
        HarmonyStruct& harmony_ref, TimeStruct& time_ref,
        const uint8_t midi_channel)
        : TonalInstrumentBase(harmony_ref, time_ref, true, midi_channel) {
        style = LeadStyle::LeadSlow;
    }

    void randomize() {
        ofLogNotice("lead", "randomize()");
        last_randomized_time = Utils::millis();

        this->min_pitch_pattern.randomize(60, 78);
        switch (Rand::randui8(3)) {
            case 0:
                this->min_pitch_pattern.time_division = TimeDivision::Quarter;
                break;
            case 1:
                this->min_pitch_pattern.time_division = TimeDivision::Half;
                break;
            case 2:
                this->min_pitch_pattern.time_division = TimeDivision::Whole;
                break;
        }

        switch (Rand::distribution(10, 30)) {
            case 0:
                this->arp_data.type = ArpType::CLOSEST;
                break;
            case 1:
                this->arp_data.type = ArpType::CLOSEST_EXC;
                break;
        }
        this->arp_data.range = Rand::randui8(12, 24);

        this->pattern_slow.setCoefSlowPattern();

        switch (Rand::distribution(16, 16)) {
            case 0:
                this->style = LeadStyle::LeadSlow;
                break;
            case 1:
                this->style = LeadStyle::LeadWhole;
                break;
        }
    }

    bool play() {
        bool hit = false;
        uint8_t length = 6;

        switch (this->style) {
            case LeadStyle::LeadWhole:
                hit = Utils::intervalHit(TimeDivision::Whole, time);
                length = TICKS_IN_BAR;
                break;
            case LeadStyle::LeadSlow:
                hit = this->pattern_slow.gate(time);
                length = time.ticksLeftInBar();
                break;
        }

        if (hit) {
            this->arp_data.min = this->min_pitch_pattern.value(time);

            uint8_t chord = harmony.getChordStep(time).root;
            uint8_t pitch =
                this->arp_data.getNextArpPitch(harmony.scale, chord);
            this->midi_channel->noteOn(NoteStruct(pitch, 64, length));
            return true;
        }
        return false;
    }
};
}  // namespace Vleerhond