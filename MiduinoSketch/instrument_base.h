#pragma once

class InstrumentBase
{
public:
    bool kill;

    InstrumentBase()
    {
        kill = false;
    }

    virtual void play() = 0;
};
