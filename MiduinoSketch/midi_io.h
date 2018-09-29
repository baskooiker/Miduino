#ifndef MIDI_IO_H
#define MIDI_IO_H
void note_on(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t* storage);
void note_off(uint8_t note, uint8_t channel, uint8_t* storage);
void send_cc(uint8_t cc, uint8_t value, uint8_t channel);
void all_notes_off(uint8_t* storage, uint8_t channel);

class MidiIO {

};

#endif // MIDI_IO_H
