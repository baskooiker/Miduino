#include <MIDI.h>

#include "callbacks.h"

MIDI_CREATE_DEFAULT_INSTANCE();

static ApplicationData data = { 0 };

void handleNoteOnCallback(byte channel, byte pitch, byte velocity)
{
    handleNoteOn(data, channel, pitch, velocity);
}

void handleNoteOffCallback(byte channel, byte pitch, byte velocity)
{
    handleNoteOff(data, channel, pitch, velocity);
}

void handleClockCallback()
{
    handleClock(data);
}

void handleControlChangeCallback(byte channel, byte number, byte value)
{
    handleControlChange(data, channel, number, value);
}

void handleStopCallback()
{
    handleStop(data);
}

void setup() {
    MIDI.setHandleNoteOn(handleNoteOnCallback);
    MIDI.setHandleNoteOff(handleNoteOffCallback);
    MIDI.setHandleClock(handleClockCallback);
    MIDI.setHandleControlChange(handleControlChangeCallback);
    MIDI.setHandleStop(handleStopCallback);

    MIDI.begin(MIDI_CHANNEL_OMNI);

    srand(analogRead(A0));

    data = init_application_data();

    // Initialize patterns
    randomize_all(data);
}

void loop() {
    MIDI.read();
}

//////////////////////////////////////////////////
// MIDI IO ///////////////////////////////////////
//////////////////////////////////////////////////

void note_on(const NoteStruct note, PitchStorage& storage)
{
    untie_notes(storage);
    NoteStruct stored = pop_from_storage(storage, note.pitch);
    if (note.pitch == stored.pitch)
    {
        note_off(note.pitch, storage);
    }
    MIDI.sendNoteOn(note.pitch, note.velocity, storage.channel);
    add_to_storage(note, storage);
}

void note_on(const NoteStruct* notes, const uint8_t length, PitchStorage& storage)
{
    untie_notes(storage);
    for (int i = 0; i < length; i++)
    {
        NoteStruct stored = pop_from_storage(storage, notes[i].pitch);
        if (notes[i].pitch == stored.pitch)
        {
            note_off(notes[i].pitch, storage);
        }
        MIDI.sendNoteOn(notes[i].pitch, notes[i].velocity, storage.channel);
    }
    for (int i = 0; i < length; i++)
    {
        add_to_storage(notes[i], storage);
    }
}

void note_off(uint8_t note, PitchStorage& storage)
{
    MIDI.sendNoteOff(note, 0, storage.channel);
    NoteStruct stored = pop_from_storage(storage, note);
}

void send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    MIDI.sendControlChange(cc, value, channel);
}
