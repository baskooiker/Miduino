#pragma once

#include "instruments/instrument_base.h"
#include "patterns/arp.h"
#include "patterns/cv_patterns.h"
#include "patterns/gate_patterns.h"
#include "patterns/interval_pattern.h"
#include "patterns/modulators.h"
#include "patterns/sample_and_hold.h"

namespace Vleerhond {
enum class MonoPitchMode { ARP, SEQUENCE };

class NoteInfo {
   public:
    bool hit;
    uint64_t tick;
    NoteStruct note;
};

class Mono : public TonalInstrumentBase {
   protected:
    ArpData arp_data;
    TimeDivision arp_reset_interval;

    GatePatternAB gate_pattern;
    IntervalPattern lead_pattern;
    CvPatternAB pitch_pattern;
    CvPatternAB octave_pattern;
    uint8_t octave_range;
    GatePatternAB slide_pattern;
    GatePatternAB accent_pattern;

    MonoStyle style;
    MonoPitchMode pitch_mode = MonoPitchMode::ARP;

    NoteInfo note_event;

   public:
    struct {
        uint8_t p_arp = 16;
        uint8_t p_euclid = 16;
        uint8_t min_velocity = 64;
        uint8_t max_velocity = 100;
        uint8_t min_pitch = 0;
    } settings;

    Mono(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);

    void randomizeArp();
    void randomizeRhythm();
    void randomizeChaos();
    virtual void randomize();
    void totalRandomize();
    bool getHit() const;
    uint8_t getSequencePitch() const;
    uint8_t getNextMonoPitch();
    uint8_t getMonoPitch() const;
    NoteInfo getNoteEvent();
    bool play();
    void checkArpReset();
    void setArpType(ArpType arp_type);
    void setStyle(MonoStyle mono_style);
    void setPitchMode(const MonoPitchMode pitch_mode);
    void setArpRange(const int range);
    void setConstSequence();
    void setSlowRhythm();
    virtual uint8_t getVelocity();
    void disableSlides();
    bool getPedal();
};
}  // namespace Vleerhond