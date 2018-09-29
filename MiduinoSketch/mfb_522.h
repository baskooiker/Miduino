#ifndef MFB_522_H
#define MFB_522_H

#include "defs.h"
#include "midi_io.h"
#include "rhythms.h"

void randomize_522_seq(ApplicationData& data)
{
    data.ac_522_pattern = init_percussive_pattern(.25);

    set_kick_pattern(data.bd_522_pattern);
    data.lo_tom_522_pattern = init_percussive_pattern(.25);
    data.mi_tom_522_pattern = init_percussive_pattern(.25);
    data.rs_522_pattern = init_percussive_pattern(.25);
    data.clave_522_pattern = init_percussive_pattern_64(.25);
    data.hh_522_pattern = init_percussive_pattern_64(.75);
    data.oh_522_pattern = init_percussive_pattern_64(.25);
    data.sd_522_pattern = init_percussive_pattern_64(.50);
    
    uint8_t clap_pat[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0};
    data.clap_522_pattern = init_pattern(clap_pat, 16);
}

void play_522(ApplicationData& data)
{
    uint8_t velocity = 63;
    if (gate(data.ac_522_pattern, data.step))
    {
        velocity = 127;
    }

    if (gate(data.bd_522_pattern, data.step))
    {
        note_on(NOTE_522_BD_LONG, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
    if (gate(data.lo_tom_522_pattern, data.step))
    {
        note_on(NOTE_522_LO_TOM, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
    if (gate(data.lo_tom_522_pattern, data.step))
    {
        note_on(NOTE_522_LO_TOM, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
    if (gate(data.mi_tom_522_pattern, data.step))
    {
        note_on(NOTE_522_HI_TOM, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
    if (gate(data.rs_522_pattern, data.step))
    {
        note_on(NOTE_522_RS, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
    if (gate(data.clave_522_pattern, data.step))
    {
        note_on(NOTE_522_CLAVE, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
    if (gate(data.clap_522_pattern, data.step))
    {
        note_on(NOTE_522_CP_LONG, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
    if (gate(data.hh_522_pattern, data.step))
    {
        note_on(NOTE_522_HH, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
    if (gate(data.oh_522_pattern, data.step))
    {
        note_on(NOTE_522_OH, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
    if (gate(data.sd_522_pattern, data.step))
    {
        note_on(NOTE_522_SN, velocity, MIDI_CHANNEL_522, data.storage_522);
    }
}


#endif // MFB_522_H
