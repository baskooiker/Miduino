#pragma once

class InstrumentBase
{
protected:
    TimeStruct& time;

public:
    PitchStorage storage;
    bool kill;

    InstrumentBase(TimeStruct& time_ref) :
        time(time_ref)
    {
        kill = false;
    }

    virtual void play() = 0;

    void process_events()
    {
        storage.process_events();
    }

    void stop_notes()
    {
        storage.all_notes_off();
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
