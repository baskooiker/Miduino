#pragma once

class InstrumentBase
{
protected:
    HarmonyStruct& harmony;
    TimeStruct& time;

public:
    PitchStorage storage;
    bool kill;

    InstrumentBase(HarmonyStruct& harmony_ref, TimeStruct& time_ref) :
        harmony(harmony_ref),
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
