#pragma once

#include "core/timing_structs.h"
#include "harmony/harmony_struct.h"
#include "midi/midi_channel.h"
#include "ofMain.h"

namespace Vleerhond {
class InstrumentBase {
   protected:
    TimeStruct& time;
    MicroTimingStruct timing;

    uint32_t last_randomized_time;

    std::vector<CcParam> params;

    bool _kill = false;
    uint8_t _variable_density = 0;
    uint8_t _variable_pitch = 0;
    uint8_t _variable_pitch_offset = 0;
    uint8_t _variable_octave = 0;

    std::shared_ptr<MidiChannel> midi_channel;
    std::string name = "";

   public:
    InstrumentBase(TimeStruct& time_ref);

    virtual bool play() = 0;
    virtual void randomize();
    virtual void randomizeParameters();
    virtual void processNoteEvents();
    virtual void stopNotes();
    virtual uint32_t randomizedTime();
    virtual uint8_t getVelocity();
    virtual std::vector<InstrumentBase*> getPtrs();
    virtual void checkAutoRandomize();
    virtual void autoRandomize();
    virtual void kill(const bool kill);
    virtual bool isKilled() const;
    virtual void setChannel(std::shared_ptr<MidiChannel> channel);
    virtual std::shared_ptr<MidiChannel> getChannel();
    virtual bool getPedal();
    virtual void updatePedalState();

    virtual void setVariableDensity(const uint8_t variable_density);
    virtual uint8_t getVariableDensity() const;
    virtual void setVariablePitch(const uint8_t variable_pitch);
    virtual uint8_t getVariablePitch() const;
    virtual void setVariablePitchOffset(const uint8_t variable_pitch_offset);
    virtual uint8_t getVariablePitchOffset() const;
    virtual void setVariableOctave(const uint8_t variable_octave);
    virtual uint8_t getVariableOctave() const;
    std::string getName();
};

class TonalInstrumentBase : public InstrumentBase {
   protected:
    HarmonyStruct& harmony;

   public:
    TonalInstrumentBase(HarmonyStruct& harmony, TimeStruct& time);
};
}  // namespace Vleerhond