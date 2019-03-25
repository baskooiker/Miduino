#include <MIDI.h>

#include "application_data.h"
#include "callbacks.h"

MIDI_CREATE_DEFAULT_INSTANCE();

ApplicationData data;

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

    //MIDI.turnThruOff();
    MIDI.begin(MIDI_CHANNEL_OMNI);

    srand(analogRead(A0));

    data.randomize_all();
}

void loop() {
    MIDI.read();
    data.process_events();
}

//////////////////////////////////////////////////
// MIDI IO ///////////////////////////////////////
//////////////////////////////////////////////////

void MidiIOsend_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
{
    MIDI.sendNoteOn(pitch, velocity, channel);
}

void MidiIO::send_note_off(const uint8_t pitch, const uint8_t channel)
{
    MIDI.sendNoteOff(pitch, 0, channel);
}

void MidiIO::send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    MIDI.sendControlChange(cc, value, channel);
}
