#pragma once

#include "core/defs.h"
#include "core/note_struct.h"

#define STORAGE_SIZE 32

namespace Vleerhond
{
    class ChannelStruct
    {
    public:
        uint8_t channel;

        ChannelStruct();
        ChannelStruct(const uint8_t channel);
    };

    class MidiChannel
    {
    protected:
        NoteStruct data[STORAGE_SIZE];
        uint8_t size = 0;

        NoteEvent events[STORAGE_SIZE];
        uint8_t nr_of_events = 0;
        bool pedal = false;

        ChannelStruct channel;
        std::string port_name = "";

        void _send_note_on(const uint8_t pitch, const uint8_t velocity);
        void _send_note_off(const uint8_t pitch);

    public:
        MidiChannel(const uint8_t channel, const std::string& port_name="");
        void processNoteEvents();
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
