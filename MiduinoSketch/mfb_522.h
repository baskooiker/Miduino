#pragma once

#include "defs.h"
#include "intervals.h"
#include "midi_io.h"
#include "mfb_522_settings.h"

void randomize_522_seq(Mfb522Settings& settings)
{
    settings.ac_522_pattern.randomize(.25f);

    set_coef_kick_pattern(settings.bd_522_pattern);
    set_coef_kick_pattern(settings.cy_522_pattern);

    settings.lo_tom_522_pattern.randomize(.25f);
    settings.mi_tom_522_pattern.randomize(.25f);
    settings.rs_522_pattern.randomize(.25f);
    settings.clave_522_pattern.randomize(.25);
    settings.sd_522_pattern.randomize(.50);

    set_coef_snare_pattern(settings.clap_522_pattern);

    float r = randf();
    if (r < .25)
    {
        settings.hh_522_pattern.randomize(.25f);
        settings.oh_522_pattern.randomize(.25f);

        settings.use_hh_int = false;
    }
    else if (r < .5)
    {
        uint8_t steps[8] = { 6, 7, 8, 10, 12, 14, 15, 16 };
        randomize_order(steps, 8);
        uint8_t gates = randui8(MAX((uint8_t)(steps[0] * .3), 2), (uint8_t)(steps[0] * .8));
        set_euclid(settings.hh_522_pattern.patterns[0], steps[0], gates);
        settings.hh_522_pattern.length = steps[0];
        
        gates = randui8(MAX((uint8_t)(steps[1] * .3), 2), (uint8_t)(steps[1] * .8));
        set_euclid(settings.hh_522_pattern.patterns[0], steps[1], gates);
        settings.hh_522_pattern.length = steps[1];
        settings.use_hh_int = false;
    }
    else
    {
        randomize_interval_hat(settings.hh_int_pattern);
        settings.use_hh_int = true;
    }

}

void play_522(Mfb522Settings& settings, const TimeStruct& time)
{
    uint8_t velocity = 63;

    if (settings.ac_522_pattern.gate(time))
    {
        velocity = 127;
    }

    if (settings.bd_522_pattern.gate(time))
    {
        settings.storage.note_on(make_note(NOTE_522_BD_LONG, velocity));
    }
    if (settings.lo_tom_522_pattern.gate(time))
    {
        settings.storage.note_on(make_note(NOTE_522_LO_TOM, velocity));
    }
    if (settings.mi_tom_522_pattern.gate(time))
    {
        settings.storage.note_on(make_note(NOTE_522_HI_TOM, velocity));
    }
    if (settings.rs_522_pattern.gate(time))
    {
        settings.storage.note_on(make_note(NOTE_522_RS, velocity));
    }
    if (settings.clave_522_pattern.gate(time))
    {
        settings.storage.note_on(make_note(NOTE_522_CLAVE, velocity));
    }
    if (settings.clap_522_pattern.gate(time))
    {
        settings.storage.note_on(make_note(NOTE_522_CP_LONG, velocity));
    }
    if (settings.oh_522_pattern.gate(time))
    {
        settings.storage.note_on(make_note(NOTE_522_OH, velocity));
    }
    if (settings.cy_522_pattern.gate(time))
    {
        settings.storage.note_on(make_note(NOTE_522_CYMBAL, velocity));
    }
    if (settings.sd_522_pattern.gate(time))
    {
        settings.storage.note_on(make_note(NOTE_522_SN, velocity));
    }
    if (!settings.use_hh_int)
    {
        if (settings.hh_522_pattern.gate(time))
        {
            settings.storage.note_on(make_note(NOTE_522_HH, velocity));
        }
    }
    else
    {
        if (interval_hit(settings.hh_int_pattern, time))
        {
            settings.storage.note_on(make_note(NOTE_522_HH, velocity));
        }
    }
}
