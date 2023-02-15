#pragma once

#include <sstream>

#include "harmony/harmony_struct.h"
#include "instruments/instrument_base.h"
#include "patterns/cv_patterns.h"
#include "patterns/fugue.h"
#include "patterns/gate_patterns.h"
#include "patterns/interval_pattern.h"
#include "patterns/sample_and_hold.h"

namespace Vleerhond {
class Bass : public TonalInstrumentBase {
   protected:
    SampleAndHold octave_sh;

   public:
    struct {
        struct {
            uint8_t p_5 = 16;
            uint8_t p_6 = 16;
            uint8_t p_7 = 16;
            uint8_t p_9 = 16;
            uint8_t p_11 = 16;
        } euclid_16;
        uint8_t p_euclid_16 = 16;

        struct {
            uint8_t p_3 = 40;
            uint8_t p_5 = 20;
            uint8_t p_7 = 20;
        } euclid_8;
        uint8_t p_euclid_8 = 8;

        uint8_t p_interval = 16;

        struct {
            double p_min = .5;
            double p_max = .75;
        } diddles;
        uint8_t p_diddles = 32;
        uint8_t p_octave_sh = 32;
        uint8_t default_note_length = 6;
        uint8_t min_pitch = 24;
    } settings;

   public:
    GatePatternAB accents;
    CvPatternAB pitches;
    CvPatternAB octaves;
    CvPatternAB variable_octaves;
    GatePatternAB slides;

    CvPatternAB probs;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;

    BassStyle style;
    CvPatternAB note_range_prob;
    uint8_t note_range_value;
    bool follow_harmony = true;

    Bass(HarmonyStruct& harmony_ref, TimeStruct& time_ref);

    virtual void randomizeOctaves();
    virtual void randomizePitches();
    virtual void randomizeDrop();
    virtual void randomizeGates();
    virtual void randomizeAccents();
    virtual void randomize();
    virtual void totalRandomize();
    virtual uint8_t getLength();
    virtual bool getHit(const uint8_t density, const TimeStruct& time);
    virtual uint8_t getPitch();
    virtual bool play();
    virtual std::string toString();
    virtual bool getPedal() override;
};
}  // namespace Vleerhond
