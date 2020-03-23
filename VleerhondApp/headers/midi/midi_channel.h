#pragma once

#include <vector>

#include "core/defs.h"
#include "core/note_struct.h"

#define STORAGE_SIZE 32

namespace Vleerhond
{
    class MidiChannel
    {
    protected:
        NoteStruct data[STORAGE_SIZE];
        uint8_t size = 0;

        NoteEvent events[STORAGE_SIZE];
        uint8_t nr_of_events = 0;
        bool pedal = false;

        std::string port_name = "";
        uint8_t channel = -1;
        uint8_t cc_channel = -1;

        virtual void _sendNoteOn(const uint8_t pitch, const uint8_t velocity);
        virtual void _sendNoteOff(const uint8_t pitch);

    public:
        MidiChannel(const uint8_t channel, const std::string& port_name);
        MidiChannel(const uint8_t channel, const uint8_t cc_channel, const std::string& port_name);
        virtual void processNoteEvents();
        virtual void noteOff(uint8_t pitch);
        virtual void _noteOn(const std::vector<NoteStruct>& notes);
        virtual void noteOn(const NoteStruct& note, const uint32_t delay);
        virtual void noteOn(const std::vector<NoteStruct>& notes);
        virtual void addToStorage(const NoteStruct& note);
        virtual NoteStruct popFromStorage(uint8_t pitch);
        virtual void processActiveNotes();
        virtual void untieNotes();
        virtual void allNotesOff();
        virtual void printStorage();
        virtual void setPedal(const bool value);
        virtual bool getPedal();

        // TODO: Remove when obsolete
        virtual std::string getPortName();

        virtual void sendCC(const uint8_t cc, const uint8_t value);
        virtual void sendBytes(std::vector<uint8_t>& bytes);

        void sendProgramChange(const uint8_t program_change);

    };
}
