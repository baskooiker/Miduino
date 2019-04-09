#pragma once

class InstrumentBase
{
protected:
    TimeStruct& time;

    uint32_t last_randomized_time;

public:
    PitchStorage storage;
    bool kill;

    InstrumentBase(TimeStruct& time_ref) :
        time(time_ref)
    {
        kill = false;
    }

    virtual void play() = 0;
    virtual void randomize() = 0;

    void process_events()
    {
        storage.process_events();
    }

    void stop_notes()
    {
        storage.all_notes_off();
    }
    
    uint32_t randomized_time()
    {
        return last_randomized_time;
    }
};

class TonalInstrumentBase : public InstrumentBase
{
protected:
    HarmonyStruct& harmony;

public:
    TonalInstrumentBase(HarmonyStruct& harmony_ref, TimeStruct& time_ref) :
        InstrumentBase(time_ref),
        harmony(harmony_ref)
    {
    }
};
