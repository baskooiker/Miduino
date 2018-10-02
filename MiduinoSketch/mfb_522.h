#ifndef MFB_522_H
#define MFB_522_H

#include "defs.h"
#include "midi_io.h"
#include "rhythms.h"

void randomize_522_seq(ApplicationData& data)
{
    data.settings_522.ac_522_pattern = init_percussive_pattern(.25);

    set_kick_pattern(data.settings_522.bd_522_pattern);
    set_kick_pattern(data.settings_522.cy_522_pattern);

    data.settings_522.lo_tom_522_pattern = init_percussive_pattern(.25);
    data.settings_522.mi_tom_522_pattern = init_percussive_pattern(.25);
    data.settings_522.rs_522_pattern = init_percussive_pattern(.25);
    data.settings_522.clave_522_pattern = init_percussive_pattern_64(.25);
    data.settings_522.hh_522_pattern = init_percussive_pattern_64(.75);
    data.settings_522.oh_522_pattern = init_percussive_pattern_64(.25);
    data.settings_522.sd_522_pattern = init_percussive_pattern_64(.50);
    
    data.settings_522.clap_522_pattern = init_pattern(SD_PATTERNS[random(NR_SD_PATTERNS)], 16);
}

void play_522(ApplicationData& data)
{
    uint8_t velocity = 63;

    Settings522& settings = data.settings_522;

    if (gate(data.settings_522.ac_522_pattern, data.step))
    {
        velocity = 127;
    }

    if (gate(data.settings_522.bd_522_pattern, data.step) && !data.uiState.kill_low)
    {
        note_on(NOTE_522_BD_LONG, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.lo_tom_522_pattern, data.step) && !data.uiState.kill_perc)
    {
        note_on(NOTE_522_LO_TOM, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.mi_tom_522_pattern, data.step) && !data.uiState.kill_perc)
    {
        note_on(NOTE_522_HI_TOM, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.rs_522_pattern, data.step) && !data.uiState.kill_mid)
    {
        note_on(NOTE_522_RS, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.clave_522_pattern, data.step) && !data.uiState.kill_perc)
    {
        note_on(NOTE_522_CLAVE, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.clap_522_pattern, data.step) && !data.uiState.kill_mid)
    {
        note_on(NOTE_522_CP_LONG, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.hh_522_pattern, data.step) && !data.uiState.kill_high)
    {
        note_on(NOTE_522_HH, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.oh_522_pattern, data.step) && !data.uiState.kill_high)
    {
        note_on(NOTE_522_OH, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.cy_522_pattern, data.step) && !data.uiState.kill_high)
    {
        note_on(NOTE_522_CYMBAL, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.sd_522_pattern, data.step) && !data.uiState.kill_mid)
    {
        note_on(NOTE_522_SN, velocity, MIDI_CHANNEL_522, settings.storage);
    }
}


#endif // MFB_522_H
