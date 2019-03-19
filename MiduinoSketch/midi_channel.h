#pragma once

#include "defs.h"
#include "midi_io.h"

#define STORAGE_SIZE 8

class ChannelStruct
{
public:
    uint8_t channel;
    int8_t pitch_offset;

    ChannelStruct()
    {
        this->channel = 0;
        this->pitch_offset = 0;
    }

    ChannelStruct(const uint8_t channel, const int8_t pitch_offset = 0)
    {
        this->channel = channel;
        this->pitch_offset = pitch_offset;
    }
};

class PitchStorage
{
public:
    NoteStruct data[STORAGE_SIZE];
    uint8_t size;

    NoteEvent events[STORAGE_SIZE];
    uint8_t nr_of_events;

    ChannelStruct channel;

    PitchStorage()
    {
        size = 0;
        nr_of_events = 0;
    }

    void note_on(const uint8_t pitch, const uint8_t velocity)
    {
        send_note_on(pitch + this->channel.pitch_offset, velocity, this->channel.channel);
    }

    void note_off(const uint8_t pitch)
    {
        send_note_off(pitch + this->channel.pitch_offset, this->channel.channel);
    }
};
