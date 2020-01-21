#pragma once

#include "core/defs.h"
#include "core/note_struct.h"

#define STORAGE_SIZE 8

namespace Vleerhond
{
    class ChannelStruct
    {
    public:
        uint8_t channel;
        int8_t pitch_offset;

        ChannelStruct();
        ChannelStruct(const uint8_t channel, const int8_t pitch_offset = 0);
    };

    class MidiChannel
    {
    protected:
        NoteStruct data[STORAGE_SIZE];
        uint8_t size;

        NoteEvent events[STORAGE_SIZE];
        uint8_t nr_of_events;
        bool pedal = false;

        ChannelStruct channel;
        void _send_note_on(const uint8_t pitch, const uint8_t velocity);
        void _send_note_off(const uint8_t pitch);

    public:
        MidiChannel(const uint8_t channel, const int8_t offset = 0);
        void process_events();
        void note_off(uint8_t pitch);
        void note_on(const NoteStruct& note);
        void note_on(const NoteStruct& note, const uint32_t delay);
        void note_on(const NoteStruct* notes, const uint8_t length);
        void add_to_storage(const NoteStruct& note);
        NoteStruct pop_from_storage(uint8_t pitch);
        void process_active_notes();
        void untie_notes();
        void all_notes_off();
        ChannelStruct& get_channel();
        void print_storage();
        void set_pedal(const bool value);

    };
}
