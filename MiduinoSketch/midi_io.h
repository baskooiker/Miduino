#ifndef MIDI_IO_H
#define MIDI_IO_H
void note_on(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t* storage);
void note_off(uint8_t note, uint8_t channel, uint8_t* storage);

#endif // MIDI_IO_H
