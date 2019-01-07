#ifndef MFB_522_H
#define MFB_522_H

#include "cv.h"
#include "defs.h"
#include "gate.h"
#include "intervals.h"
#include "midi_io.h"
#include "rhythms.h"

void randomize_522_seq(ApplicationData& data)
{
    data.settings_522.ac_522_pattern = init_percussive_pattern(.25);

    set_coef_kick_pattern(data.settings_522.bd_522_pattern);
    set_coef_kick_pattern(data.settings_522.cy_522_pattern);

    data.settings_522.lo_tom_522_pattern = init_percussive_pattern(.25);
    data.settings_522.mi_tom_522_pattern = init_percussive_pattern(.25);
    data.settings_522.rs_522_pattern = init_percussive_pattern(.25);
    data.settings_522.clave_522_pattern = init_percussive_pattern_64(.25);
    data.settings_522.sd_522_pattern = init_percussive_pattern_64(.50);

    data.settings_522.clap_522_pattern = init_pattern(SD_PATTERNS[randi(NR_SD_PATTERNS)], 16);

    float r = randf();
    if (r < .25)
    {
        data.settings_522.hh_522_pattern = init_percussive_pattern_64(.75);
        data.settings_522.oh_522_pattern = init_percussive_pattern_64(.25);
        data.settings_522.use_hh_int = false;
    }
    else if (r < .5)
    {
        uint8_t steps[8] = { 6, 7, 8, 10, 12, 14, 15, 16 };
        randomize_order(steps, 8);
        uint8_t gates = randi(MAX((uint8_t)(steps[0] * .3), 2), (uint8_t)(steps[0] * .8));
        set_euclid(data.settings_522.hh_522_pattern.patterns[0], steps[0], gates);
        data.settings_522.hh_522_pattern.length = steps[0];
        
        gates = randi(MAX((uint8_t)steps[1] * .3, 2), (uint8_t)(steps[1] * .8));
        set_euclid(data.settings_522.hh_522_pattern.patterns[0], steps[1], gates);
        data.settings_522.hh_522_pattern.length = steps[1];
        data.settings_522.use_hh_int = false;
    }
    else
    {
        randomize_interval(data.settings_522.hh_int_pattern, hat_interval_probs);
        data.settings_522.use_hh_int = true;
    }

}

void play_522(ApplicationData& data)
{
    uint8_t velocity = 63;

    Settings522& settings = data.settings_522;

    if (gate(data.settings_522.ac_522_pattern, data.step, data.ticks))
    {
        velocity = 127;
    }

    if (gate(data.settings_522.bd_522_pattern, data.step, data.ticks))
    {
        note_on(NOTE_522_BD_LONG, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.lo_tom_522_pattern, data.step, data.ticks))
    {
        note_on(NOTE_522_LO_TOM, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.mi_tom_522_pattern, data.step, data.ticks))
    {
        note_on(NOTE_522_HI_TOM, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.rs_522_pattern, data.step, data.ticks))
    {
        note_on(NOTE_522_RS, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.clave_522_pattern, data.step, data.ticks))
    {
        note_on(NOTE_522_CLAVE, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.clap_522_pattern, data.step, data.ticks))
    {
        note_on(NOTE_522_CP_LONG, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.oh_522_pattern, data.step, data.ticks))
    {
        note_on(NOTE_522_OH, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.cy_522_pattern, data.step, data.ticks))
    {
        note_on(NOTE_522_CYMBAL, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (gate(data.settings_522.sd_522_pattern, data.step, data.ticks))
    {
        note_on(NOTE_522_SN, velocity, MIDI_CHANNEL_522, settings.storage);
    }
    if (!data.settings_522.use_hh_int)
    {
        if (gate(data.settings_522.hh_522_pattern, data.step, data.ticks))
        {
            note_on(NOTE_522_HH, velocity, MIDI_CHANNEL_522, settings.storage);
        }
    }
    else 
    {
        if (interval_hit(data.settings_522.hh_int_pattern, data.step, data.ticks))
        {
            note_on(NOTE_522_HH, velocity, MIDI_CHANNEL_522, settings.storage);
        }
    }
}

#endif // MFB_522_H
