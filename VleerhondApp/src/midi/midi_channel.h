#pragma once

#include <vector>

#include "core/defs.h"
#include "core/note_struct.h"

#define MIDI_STORAGE_SIZE 32

namespace Vleerhond {
class MidiChannel {
   protected:
    NoteStruct data[MIDI_STORAGE_SIZE];
    uint8_t size = 0;

    NoteEvent events[MIDI_STORAGE_SIZE];
    uint8_t nr_of_events = 0;
    bool pedal = false;

    uint8_t channel = -1;
    uint8_t cc_channel = -1;
    std::string port_name = "";

    virtual void _sendNoteOn(const uint8_t pitch, const uint8_t velocity);
    virtual void _sendNoteOff(const uint8_t pitch);
    virtual void _noteOn(const std::vector<NoteStruct>& notes);

    virtual void addToStorage(const NoteStruct& note);
    virtual NoteStruct popFromStorage(uint8_t pitch);
    virtual void untieNotes();

   public:
    MidiChannel(const uint8_t channel, const std::string& port_name);
    MidiChannel(
        const uint8_t channel, const uint8_t cc_channel,
        const std::string& port_name);

    virtual void processNoteEvents();
    virtual void processActiveNotes();

    virtual void noteOn(const NoteStruct& note, const uint32_t delay);
    virtual void noteOn(
        const std::vector<NoteStruct>& notes, const uint8_t delay);
    virtual void noteOff(uint8_t pitch);
    virtual void sendCC(const uint8_t cc, const uint8_t value);
    virtual void sendBytes(std::vector<uint8_t>& bytes);
    void sendProgramChange(const uint8_t program_change);

    virtual void allNotesOff();
    virtual void printStorage();

    virtual void setPedal(const bool value);
    virtual bool getPedal();

    // TODO: Remove when obsolete
    virtual std::string getPortName();

    virtual uint8_t getChannel() { return this->channel; }
};
}  // namespace Vleerhond
