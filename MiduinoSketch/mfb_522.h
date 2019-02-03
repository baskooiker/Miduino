#pragma once

#include "cv.h"
#include "defs.h"
#include "gate.h"
#include "intervals.h"
#include "midi_io.h"
#include "rhythms.h"

void randomize_522_seq(ApplicationData& data)
{
    randomize(data.mfb_522_settings.ac_522_pattern, .25f);

    set_coef_kick_pattern(data.mfb_522_settings.bd_522_pattern);
    set_coef_kick_pattern(data.mfb_522_settings.cy_522_pattern);

    randomize(data.mfb_522_settings.lo_tom_522_pattern, .25f);
    randomize(data.mfb_522_settings.mi_tom_522_pattern, .25f);
    randomize(data.mfb_522_settings.rs_522_pattern, .25f);
    randomize(data.mfb_522_settings.clave_522_pattern, .25);
    randomize(data.mfb_522_settings.sd_522_pattern, .50);

    set_coef_snare_pattern(data.mfb_522_settings.clap_522_pattern);

    float r = randf();
    if (r < .25)
    {
        randomize(data.mfb_522_settings.hh_522_pattern, .25f);
        randomize(data.mfb_522_settings.oh_522_pattern, .25f);

        data.mfb_522_settings.use_hh_int = false;
    }
    else if (r < .5)
    {
        uint8_t steps[8] = { 6, 7, 8, 10, 12, 14, 15, 16 };
        randomize_order(steps, 8);
        uint8_t gates = randi(MAX((uint8_t)(steps[0] * .3), 2), (uint8_t)(steps[0] * .8));
        set_euclid(data.mfb_522_settings.hh_522_pattern.patterns[0], steps[0], gates);
        data.mfb_522_settings.hh_522_pattern.length = steps[0];
        
        gates = randi(MAX((uint8_t)(steps[1] * .3), 2), (uint8_t)(steps[1] * .8));
        set_euclid(data.mfb_522_settings.hh_522_pattern.patterns[0], steps[1], gates);
        data.mfb_522_settings.hh_522_pattern.length = steps[1];
        data.mfb_522_settings.use_hh_int = false;
    }
    else
    {
        randomize_interval_hat(data.mfb_522_settings.hh_int_pattern);
        data.mfb_522_settings.use_hh_int = true;
    }

}

void play_522(ApplicationData& data, const TimeStruct& time)
{
    uint8_t velocity = 63;

    Mfb522Settings& settings = data.mfb_522_settings;

    if (gate(data.mfb_522_settings.ac_522_pattern, time))
    {
        velocity = 127;
    }

    if (gate(data.mfb_522_settings.bd_522_pattern, time) && !data.ui_state.kill_low)
    {
        note_on(make_note(NOTE_522_BD_LONG, velocity), settings.storage);
    }
    if (gate(data.mfb_522_settings.lo_tom_522_pattern, time))
    {
        note_on(make_note(NOTE_522_LO_TOM, velocity), settings.storage);
    }
    if (gate(data.mfb_522_settings.mi_tom_522_pattern, time))
    {
        note_on(make_note(NOTE_522_HI_TOM, velocity), settings.storage);
    }
    if (gate(data.mfb_522_settings.rs_522_pattern, time))
    {
        note_on(make_note(NOTE_522_RS, velocity), settings.storage);
    }
    if (gate(data.mfb_522_settings.clave_522_pattern, time))
    {
        note_on(make_note(NOTE_522_CLAVE, velocity), settings.storage);
    }
    if (gate(data.mfb_522_settings.clap_522_pattern, time))
    {
        note_on(make_note(NOTE_522_CP_LONG, velocity), settings.storage);
    }
    if (gate(data.mfb_522_settings.oh_522_pattern, time))
    {
        note_on(make_note(NOTE_522_OH, velocity), settings.storage);
    }
    if (gate(data.mfb_522_settings.cy_522_pattern, time))
    {
        note_on(make_note(NOTE_522_CYMBAL, velocity), settings.storage);
    }
    if (gate(data.mfb_522_settings.sd_522_pattern, time))
    {
        note_on(make_note(NOTE_522_SN, velocity), settings.storage);
    }
    if (!data.mfb_522_settings.use_hh_int && !data.ui_state.kill_high)
    {
        if (gate(data.mfb_522_settings.hh_522_pattern, time))
        {
            note_on(make_note(NOTE_522_HH, velocity), settings.storage);
        }
    }
    else if(!data.ui_state.kill_high)
    {
        if (interval_hit(data.mfb_522_settings.hh_int_pattern, time))
        {
            note_on(make_note(NOTE_522_HH, velocity), settings.storage);
        }
    }
}
