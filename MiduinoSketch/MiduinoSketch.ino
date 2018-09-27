#include <TimerObject.h>

#include <MIDI.h>

#include "defs.h"
#include "rhythms.h"
#include "basslines.h"
#include "storage.h"
#include "scales.h"

#include "mfb_503.h"
#include "mfb_522.h"
#include "p50.h"
#include "rocket.h"

MIDI_CREATE_DEFAULT_INSTANCE();

static ApplicationData data = {};

void setup() {
    MIDI.begin(MIDI_CHANNEL_OMNI);

    data.rocket_pattern = init_bassline();
  
    data.step = 0;
    data.ticks_counter = 0;
    data.swing = 4;

    data.rocket_density = .8;
    data.rocket_octave = 3;
    data.p50_octave = 5;
    data.root = ROOT_C;
    data.scale = AEOLIAN;

    init_storage(data.storage_503);
    init_storage(data.storage_522);
    init_storage(data.storage_p50);
    init_storage(data.storage_rocket);

    // Initialize patterns
    randomize_503_seq(&data);
    randomize_522_seq(&data);
    randomize_P50_seq(&data);
    randomize_rocket_seq(&data);
    root_rocket_seq(&data);

}

void note_on(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t* storage)
{
    uint8_t stored = pop_from_storage(storage, note);
    if (stored > 0)
    {
        MIDI.sendNoteOff(note, 0, channel);
    }
    MIDI.sendNoteOn(note, velocity, channel);
    add_to_storage(storage, note);
}

void note_off(uint8_t note, uint8_t channel, uint8_t* storage)
{
    MIDI.sendNoteOff(note, 0, channel);
    uint8_t stored = pop_from_storage(storage, note);
}

void send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    MIDI.sendControlChange(cc, value, channel);
}

void all_notes_off(uint8_t* storage, uint8_t channel)
{
    uint8_t p = 0;
    do {
        p = pop_from_storage(storage);
        if (p > 0)
        {
            MIDI.sendNoteOff(p, 0, channel);
        }
    } while (p != 0);
}

void play_all()
{
    play_503(&data);
    play_522(&data);
    play_rocket(&data);
    play_P50(&data);
}

void loop() {
	if (MIDI.read())
	{
		switch (MIDI.getType())
	    {
		    case midi::MidiType::Clock:
            {
                if (data.ticks_counter == (data.step % 2 == 1 ? 0 : data.swing))
                {
                    play_all();
                }
                if (data.ticks_counter == 0)
                {
                    data.step = (data.step + 1) % COMMON_DENOMINATOR;
                }
                data.ticks_counter = (data.ticks_counter + 1) % TICKS_PER_STEP;
            }
		    break;
		    case midi::MidiType::Stop:
                all_notes_off(data.storage_503, MIDI_CHANNEL_503);
                all_notes_off(data.storage_522, MIDI_CHANNEL_522);
                all_notes_off(data.storage_p50, MIDI_CHANNEL_P50);
                all_notes_off(data.storage_rocket, MIDI_CHANNEL_ROCKET);
		        data.step = 0;
                data.ticks_counter = 0;
		    break;
		    case midi::MidiType::ControlChange:
		        switch (MIDI.getData1())
		        {
			        case BSP_KNOB_08:
				        data.rocket_density = (MIDI.getData2() / 127.) * .8f + .2f;
				        break;
			        case BSP_STEP_01:
				        if (MIDI.getData2() == 0)
				        {
					        randomize_503_sound();
				        }
				        break;
			        case BSP_STEP_02:
				        if (MIDI.getData2() == 0)
				        {
					        randomize_503_seq(&data);
				        }
				        break;
			        case BSP_STEP_03:
				        randomize_522_seq(&data);
				        break;
                    case BSP_STEP_13:
                        if (MIDI.getData2() == 0)
                        {
                            root_rocket_seq(&data);
                        }
                        break;
                    case BSP_STEP_14:
                        if (MIDI.getData2() == 0)
                        {
                            modify_rocket_seq(&data);
                        }
                        break;
                    case BSP_STEP_15:
                        if (MIDI.getData2() == 0)
                        {
                            modify_rocket_seq(&data);
                        }
                        break;
			        case BSP_STEP_16:
			            if (MIDI.getData2() == 0)
			            {
				            randomize_rocket_seq(&data);
			            }
			        break;
			        default:
			        break;
		        }
		    break;
		    case midi::MidiType::NoteOn:
			    switch (MIDI.getData1())
			    {
				    case BSP_PAD_01:
				    data.bsp_pad_01_down = true;
				    break;
				    case BSP_PAD_02:
				    data.bsp_pad_02_down = true;
				    break;
				    case BSP_PAD_03:
				    data.bsp_pad_03_down = true;
				    break;
				    case BSP_PAD_04:
				    data.bsp_pad_04_down = true;
				    break;
				    case BSP_PAD_05:
				    data.bsp_pad_05_down = true;
				    break;
				    case BSP_PAD_06:
				    data.bsp_pad_06_down = true;
				    break;
				    case BSP_PAD_07:
				    data.bsp_pad_07_down = true;
				    break;
				    case BSP_PAD_08:
				    data.bsp_pad_08_down = true;
				    break;
				    case BSP_PAD_09:
				    data.bsp_pad_09_down = true;
				    break;
				    case BSP_PAD_10:
				    data.bsp_pad_10_down = true;
				    break;
				    case BSP_PAD_11:
				    data.bsp_pad_11_down = true;
				    break;
				    case BSP_PAD_12:
				    data.bsp_pad_12_down = true;
				    break;
				    case BSP_PAD_13:
				    data.bsp_pad_13_down = true;
				    break;
				    case BSP_PAD_14:
				    data.bsp_pad_14_down = true;
				    break;
				    case BSP_PAD_15:
				    data.bsp_pad_15_down = true;
				    break;
				    case BSP_PAD_16:
				    data.bsp_pad_16_down = true;
				    break;
				    default:
		    	    break;
			    }
			    break;
		    case midi::MidiType::NoteOff:
			    switch (MIDI.getData1())
			    {
				    case BSP_PAD_01:
				    data.bsp_pad_01_down = false;
				    break;
				    case BSP_PAD_02:
				    data.bsp_pad_02_down = false;
				    break;
				    case BSP_PAD_03:
				    data.bsp_pad_03_down = false;
				    break;
				    case BSP_PAD_04:
				    data.bsp_pad_04_down = false;
				    break;
				    case BSP_PAD_05:
				    data.bsp_pad_05_down = false;
				    break;
				    case BSP_PAD_06:
				    data.bsp_pad_06_down = false;
				    break;
				    case BSP_PAD_07:
				    data.bsp_pad_07_down = false;
				    break;
				    case BSP_PAD_08:
				    data.bsp_pad_08_down = false;
				    break;
				    case BSP_PAD_09:
				    data.bsp_pad_09_down = false;
				    break;
				    case BSP_PAD_10:
				    data.bsp_pad_10_down = false;
				    break;
				    case BSP_PAD_11:
				    data.bsp_pad_11_down = false;
				    break;
				    case BSP_PAD_12:
				    data.bsp_pad_12_down = false;
				    break;
				    case BSP_PAD_13:
				    data.bsp_pad_13_down = false;
				    break;
				    case BSP_PAD_14:
				    data.bsp_pad_14_down = false;
				    break;
				    case BSP_PAD_15:
				    data.bsp_pad_15_down = false;
				    break;
				    case BSP_PAD_16:
				    data.bsp_pad_16_down = false;
				    break;
				    default:
				    break;
			    }
			    break;
		    default:
		    break;
	    }
	}
}

