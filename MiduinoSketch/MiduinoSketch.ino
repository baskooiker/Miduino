#include <MIDI.h>

#include "app_utils.h"
#include "callbacks.h"

MIDI_CREATE_DEFAULT_INSTANCE();

static ApplicationData data;

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

    // Initialize patterns
    randomize_all(data);
}

void loop() {
    MIDI.read();
    process_events(data);
}

//////////////////////////////////////////////////
// MIDI IO ///////////////////////////////////////
//////////////////////////////////////////////////

void send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
{
    MIDI.sendNoteOn(pitch, velocity, channel);
}

void send_note_off(const uint8_t pitch, const uint8_t channel)
{
    MIDI.sendNoteOff(pitch, 0, channel);
}

void send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    MIDI.sendControlChange(cc, value, channel);
}
