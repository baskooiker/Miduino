#include "pch.h"
#include <iostream>
#include "stdio.h"
#include <stdlib.h>

#include <time.h>
#include <sys\timeb.h> 
uint32_t millis()
{
    // TODO: Properly move generalize this
    timeb t;
    ftime(&t);
    return (uint32_t)(t.time * 1000 + t.millitm);
}

#include "arp.h"
#include "application/callbacks.h"
#include "defs.h"
#include "euclid.h"
#include "intervals.h"
#include "modulators.h"
#include "print.h"
#include "rand.h"
#include "utils.h"
#include "print_midi.h"
#include "note_struct.h"
#include "interval_pattern.h"
#include "midi/midi_io.h"
#include "midi/midi_channel.h"

#include "instruments/tanzbar/tanzbar.h"
#include "instruments/mfb_522/mfb_522.h"

namespace Vleerhond
{
    ///////////////////////////////////////////////////
    // TODO: move to separate file
    void MidiIO::send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
    {
        printf("note_on(%-3d, %-3d, %-2d)\n", pitch, velocity, channel);
        note_hits[pitch % 12]++;
    }

    void MidiIO::send_note_off(const uint8_t pitch, const uint8_t channel)
    {
        printf("note_off(%-3d, %-2d)\n", pitch, channel);
    }

    void MidiIO::send_cc(uint8_t cc, uint8_t value, uint8_t channel)
    {
        printf("send_cc(%-3d, %-3d, %-2d)\n", cc, value, channel);
    }
    ///////////////////////////////////////////////////////
    void test_randf()
    {
        uint16_t numbers[10] = { 0 };
        for (int i = 0; i < 10; i++) numbers[i] = 0;

        for (int i = 0; i < 10000; i++)
        {
            float random = Rand::randf();
            for (int i = 1; i < 11; i++)
            {
                if (random < i * .1f)
                {
                    numbers[i - 1]++;
                    break;
                }
            }
        }

        for (int i = 0; i < 10; i++)
        {
            printf("%d ", numbers[i]);
        }
        printf("\n");
    }

    void test_randui8()
    {
        uint16_t numbers[16];
        for (int i = 0; i < 16; i++) numbers[i] = 0;

        for (int i = 0; i < 16000; i++)
        {
            numbers[Rand::randui8(16)]++;
        }

        for (int i = 0; i < 16; i++)
        {
            printf("%d ", numbers[i]);
        }
        printf("\n");
    }

    void test_randi8()
    {
        uint16_t numbers[11] = { 0 };

        for (int i = 0; i < 16000; i++)
        {
            numbers[Rand::randi8(-5, 5) + 5]++;
        }

        for (int i = 0; i < 11; i++)
        {
            printf("%d: %d \n", i - 5, numbers[i]);
        }
        printf("\n");
    }

    void test_set_euclid()
    {
        BinaryPattern pattern;
        uint8_t length = 16;
        uint8_t steps = 5;
        print_pattern(pattern, length);
        pattern.set_euclid(length, steps);
        print_pattern(pattern, length);
    }

    void test_coef()
    {
        GatePatternAB pattern;
        pattern.set_coef_kick_pattern();
        print_pattern(pattern);
        printf("\n");

        pattern.set_coef_hat_pattern();
        print_pattern(pattern);
        printf("\n");

        pattern.set_coef_snare_pattern();
        print_pattern(pattern);
        printf("\n");

        pattern.randomize(0.f);
        print_pattern(pattern);
        printf("\n");
    }

    void test_arp_type(ArpData& data, Scale& scale)
    {
        data.counter = 0;
        for (int chord = 0; chord < 8; chord += 2)
        {
            for (int i = 0; i < 8; i++)
            {
                uint8_t p = data.get_next_arp_pitch(scale, chord);
                printf("%-2d ", p);
            }
            printf("\n");
        }
        printf("\n");
    }

    void arp_test()
    {
        printf("Test Arp\n");
        ArpData data;
        Scale scale(ScaleType::AEOLIAN, Root::ROOT_D);

        data.type = ArpType::CLOSEST;
        data.range = 16;
        data.min = 32;
        data.counter = 0;

        for (int chord = 0; chord < 8; chord += 2)
        {
            for (int i = 0; i < 12; i++)
            {
                printf("%d ", scale.chord_contains(i, chord));
            }
            printf("\n");
        }

        data.type = ArpType::UP;
        test_arp_type(data, scale);
        data.type = ArpType::DOWN;
        test_arp_type(data, scale);
        data.type = ArpType::UPDOWN;
        test_arp_type(data, scale);
        data.type = ArpType::PICKING_IN;
        test_arp_type(data, scale);
        data.type = ArpType::CLOSEST;
        test_arp_type(data, scale);
    }

    void storage_test()
    {
        MidiChannel midi_channel(0);
        midi_channel.print_storage();
        midi_channel.add_to_storage(NoteStruct(36, 5));
        midi_channel.print_storage();
        midi_channel.add_to_storage(NoteStruct(36, 1));
        midi_channel.print_storage();
        midi_channel.add_to_storage(NoteStruct(38, 3));
        midi_channel.print_storage();
        midi_channel.add_to_storage(NoteStruct(40, 2));
        midi_channel.print_storage();
        midi_channel.process_active_notes();
        midi_channel.print_storage();
        midi_channel.process_active_notes();
        midi_channel.print_storage();
        midi_channel.process_active_notes();
        midi_channel.print_storage();
    }

    void interval_pattern_test()
    {
        uint16_t counter_4 = 0;
        uint16_t counter_8 = 0;
        uint16_t counter_16 = 0;
        uint16_t counter_32 = 0;
        uint16_t counter_12 = 0;
        IntervalPattern pattern;
        //IntervalProbs probs = { 4, 4, 4, 4, 4 };
        IntervalProbs probs = { 2, 0, 0, 2, 2 };
        for (int x = 0; x < 1000; x++)
        {
            pattern.randomize_interval(probs);
            for (int i = 0; i < 16; i++)
            {
                switch (pattern.pattern[i])
                {
                case TimeDivision::Quarter: counter_4++; break;
                case TimeDivision::Eight: counter_8++; break;
                case TimeDivision::Sixteenth: counter_16++; break;
                case TimeDivision::Thirtysecond: counter_32++; break;
                case TimeDivision::TripletEight: counter_12++; break;
                }
            }
            for (int i = 0; i < 16; i++)
            {
                for (int step = 0; step < 16; step++)
                {
                    for (int tick = 0; tick < 6; tick++)
                    {
                        TimeStruct time;
                        time.tick = tick + step * TICKS_PER_STEP;
                        //uint32_t s1 = get_step(pattern.time_division, step, tick, false);
                        //uint8_t s = (uint8_t)(s1 % 16L);
                        TimeDivision div = pattern.interval(time);
                        //printf("%2d %2d", s1, s);
                        //sbool b = get_step(div, step, tick) != NO_STEP;
                        //printf("%d ", b);
                    }
                    //sprintf("\n");
                }
                //printf("\n");
            }
            printf("\n");
        }
        printf("%3d ", counter_4);
        printf("%3d ", counter_8);
        printf("%3d ", counter_16);
        printf("%3d ", counter_32);
        printf("%3d ", counter_12);
        printf("\n");
    }

    void test_intervals()
    {
        printf("Test Intervals\n");
        uint32_t step = 0;
        for (int bar = 0; bar < 16; bar++)
        {
            for (int _step = 0; _step < 16; _step++)
            {
                TimeStruct time;
                time.tick = TICKS_PER_STEP * step++;
                printf("%-3d ", time.get_count(TimeDivision::Sixteenth));
            }
            printf("\n");
        }
        printf("\n\n");
    }

    //void test_gates()
    //{
    //    printf("Test Gates\n");
    //    ApplicationData data;
    //    data.tanzbar_lo.randomize();

    //    print_pattern(data.tanzbar_lo.bd_pattern);

    //    for (int bar = 0; bar < 16; bar++)
    //    {
    //        for (int _step = 0; _step < 16; _step++)
    //        {
    //            printf("%-3d ", data.tanzbar_lo.bd_pattern.gate(data.time));
    //            data.time.tick += 6;
    //        }
    //        printf("\n");
    //    }
    //    printf("\n\n");
    //}

    void test_chords()
    {
        printf("Test Chords\n");
        ApplicationData data;
        data.randomize_all();
        data.harmony.type = HarmonyType::HarmonyHigh;

        uint32_t step = 0;
        for (int bar = 0; bar < 16; bar++)
        {
            for (int _step = 0; _step < 16; _step++)
            {
                printf("%-3d ", data.harmony.get_chord_step(data.time));
            }
            printf("\n");
        }
        printf("\n\n");
    }

    //void test_503()
    //{
    //    printf("Test 503\n");
    //    ApplicationData data;
    //    data.randomize_all();
    //    Mfb503Settings settings(data.harmony, data.time);
    //    data.harmony.type = HarmonyType::HarmonyHigh;

    //    settings.randomize_503_seq();

    //    settings.play_pitch_bd = true;

    //    uint32_t step = 0;
    //    for (int bar = 0; bar < 16; bar++)
    //    {
    //        for (int _step = 0; _step < 16; _step++)
    //        {
    //            data.time.tick = TICKS_PER_STEP * step++;
    //            settings.play_bd(data.harmony, data.time);
    //        }
    //        printf("\n");
    //    }
    //    printf("\n\n");
    //}

    void test_poly()
    {

        printf("Test Poly\n");
        ApplicationData data;
        data.randomize_all();
        data.harmony.type = HarmonyType::HarmonyHigh;

        uint32_t step = 0;
        for (int bar = 0; bar < 16; bar++)
        {
            printf("bar %-2d\n", bar);
            for (int _step = 0; _step < 16; _step++)
            {
                printf("step %-2d\n", step);
                data.time.tick = TICKS_PER_STEP * step++;
                //play_poly(data, data.time);

                //print_storage(data.poly_settings.storage);

                //for(int tick = 0; tick < 6; tick++)
                //    stop_notes(data.poly_settings.storage);
            }
            printf("\n");
        }
        printf("\n\n");
    }

    void test_play_all()
    {
        printf("Test Play All\n");
        ApplicationData data;
        data.randomize_all();
        data.play_all();
    }

    void test_mono()
    {

        printf("Test Mono\n");
        ApplicationData data;
        data.randomize_all();
        data.harmony.type = HarmonyType::HarmonyHigh;

        //data.mono.style = MonoStyle::MonoLeadPattern;

        uint32_t step = 0;
        for (int bar = 0; bar < 16; bar++)
        {
            printf("bar %-2d\n", bar);
            for (int _step = 0; _step < 16; _step++)
            {
                printf("step %-2d\n", step);
                data.time.tick = TICKS_PER_STEP * step++;
                data.mono.play();

                data.mono.midi_channel.print_storage();
            }
            printf("\n");
        }
        printf("\n\n");
    }

    //void test_bass()
    //{
    //    printf("Test Bass\n");
    //    ApplicationData data;
    //    data.randomize_all();

    //    data.harmony.type = HarmonyType::HarmonyHigh;

    //    data.rocket_bass.style = BassStyle::BassEuclid;
    //    data.rocket_bass.pitch_range = 0;
    //    data.rocket_bass.density = 0;

    //    uint32_t step = 0;
    //    for (int bar = 0; bar < 4; bar++)
    //    {
    //        printf("bar %-2d\n", bar);
    //        for (int _step = 0; _step < 16; _step++)
    //        {
    //            printf("step %-2d\n", step);
    //            data.time.tick = TICKS_PER_STEP * step++;
    //            data.rocket_bass.play();

    //            data.mono.midi_channel.print_storage();

    //            for (int tick = 0; tick < 6; tick++)
    //                data.process_active_notes();
    //        }
    //        printf("\n");
    //    }
    //    printf("\n\n");
    //}

    void test_randomize_interval_lead()
    {
        printf("Test Randomize Interval Lead\n");
        IntervalPattern p;
        for (int i = 0; i < 16; i++)
        {
            p.randomize_interval_lead();
            print_pattern(p);
        }
    }

    //void test_distributed_range()
    //{
    //    for (int i = 0; i < 128; i++)
    //    {
    //        uint8_t dist = get_distributed_range(randui8(128), i, 3);
    //        printf("%d\n", dist);
    //    }
    //}

    void cb_1(ApplicationData& data) { printf("cb_1\n"); }
    void cb_2(ApplicationData& data) { printf("cb_2\n"); }
    void cb_3(ApplicationData& data) { printf("cb_3\n"); }
    
    /*
    void test_double_callbacks()
    {
        printf("test double callbacks\n");
        ApplicationData data;

        press_step(data.ui_state.step_state, BTN_LEFT_BTM_01);
        handle_step_release(data, BTN_LEFT_TOP_01, BTN_LEFT_TOP_01, BTN_LEFT_BTM_01, cb_1, cb_2, cb_3);

        release_step(data.ui_state.step_state, BTN_LEFT_BTM_01);
        handle_step_release(data, BTN_LEFT_TOP_01, BTN_LEFT_TOP_01, BTN_LEFT_BTM_01, cb_1, cb_2, cb_3);

        press_step(data.ui_state.step_state, BTN_LEFT_TOP_01);
        handle_step_release(data, BTN_LEFT_BTM_01, BTN_LEFT_TOP_01, BTN_LEFT_BTM_01, cb_1, cb_2, cb_3);

        release_step(data.ui_state.step_state, BTN_LEFT_TOP_01);
        handle_step_release(data, BTN_LEFT_BTM_01, BTN_LEFT_TOP_01, BTN_LEFT_BTM_01, cb_1, cb_2, cb_3);
    }
    */

    void test_shuffle()
    {
        printf("Test shuffle\n");

        TimeStruct time;
        time.reset();
        time.tick = 6;

        for (int i = 0; i < 128; i++)
        {
            printf("%d\n", time.get_shuffle_delay(i));
        }
    }

    void test_lfo()
    {
        Lfo lfo;
        lfo.rate = 96;
        lfo.offset = 62;

        TimeStruct time;
        for (int i = 0; i < TICKS_IN_BAR; i++)
        {
            time.tick = i;
            printf("%d\n", lfo.value(time));
        }
    }

    void test_chord_patterns()
    {
        printf("Test Chord patterns\n");

        for (int i = 0; i < 16; i++)
        {
            uint8_t length = 0;
            uint8_t chord_time_pattern[4] = { 0, 0, 0, 0 };
            ChordUtils::get_chord_time_pattern(chord_time_pattern, length);
            for (int j = 0; j < length; j++)
                printf("%2d ", chord_time_pattern[j]);
            printf("\n");
        }

        printf("\n");

        for (int i = 0; i < 16; i++)
        {
            uint8_t chords[] = { 5, 6, 7, 8 };
            uint8_t seq[4] = { 0, 0, 0, 0 };
            uint8_t length = i % 4 + 1;
            ChordUtils::get_chord_seq(chords, length, seq);
            for (int j = 0; j < length; j++)
                printf("%2d ", seq[j]);
            printf("\n");
        }

        printf("\n");

        Scale scale;

        for (int i = 0; i < 16; i++)
        {
            CvPattern16 pat;
            if (i % 2 == 0)
                pat.set_chord_pattern(scale);
            else
                pat.set_chord_pattern(scale, 4);
            for (int i = 0; i < 16; i++)
            {
                printf("%3d ", pat.value(i));
            }
            printf("\n\n");
        }
    }

    void test_scale()
    {
        printf("Test scales\n");
        ScaleType types[] = {
            ScaleType::AEOLIAN,
            ScaleType::DORIAN,
            ScaleType::IONIAN
        };

        for (int i = 0; i < 3; i++)
        {
            Scale scale;
            scale.set_scale(types[i]);
            uint8_t notes[8] = { 0 };
            uint8_t length = 0;
            scale.get_available_chords_indices(notes, length);
            for (int j = 0; j < length; j++)
            {
                printf("%2d ", notes[j]);
            }
            printf("\n");
        }

        Scale scale;
        for (int i = 0; i < 24; i++)
        {
            if (scale.contains(i))
            {
                ofLogNotice("test", "%3d -> %3d", i, scale.get_ascending(i, (uint8_t)NoteInterval::IntervalFifth));
            }
        }
    }

    void test_harmony()
    {
        printf("Test Harmony\n");
        HarmonyStruct harmony;
        TimeStruct time;
        harmony.randomize();
        harmony.type = HarmonyType::HarmonyHigh;
        for (int bar = 0; bar < 4; bar++)
        {
            for (int i = 0; i < 16; i++)
            {
                printf("%2d ", harmony.get_chord_step(time));
                time.tick += TICKS_PER_STEP;
            }
            printf("\n");
        }
        printf("\n\n");
    }

    //void test_application_settings()
    //{
    //    printf("Test ApplicationData\n");
    //    ApplicationData data;

    //    ChannelStruct channels[4];
    //    uint8_t length;
    //    data.rocket_bass.midi_channel.get_channels(channels, length);
    //    for (int i = 0; i < length; i++)
    //    {
    //        printf("%2d: %2d\n", channels[i].channel, channels[i].pitch_offset);
    //    }
    //}

    void time_struct_test()
    {
        printf("TimeStruct Test\n");
        TimeStruct time;
        time.average_pulse_time = 60000.f / 120.f;
        printf("Maximum offset (120 bpm): %d\n", time.get_shuffle_delay(0, 127));

        time.average_pulse_time = 60000.f / 140.f;
        printf("Maximum offset (140 bpm): %d\n", time.get_shuffle_delay(0, 127));
    }

    void test_utils_rerange()
    {
        printf("test_utils_rerange\n");
        for (int i = 0; i < 128; i++)
        {
            printf("%3d ", Utils::rerange(i, 32, 64));
            if (i % 16 == 0)
            {
                printf("\n");
            }
        }
    }

    void test_instrument(ApplicationData& data, InstrumentBase& instr)
    {
        data.time.tick = 0;
        instr.randomize();
        for (int i = 0; i < 4; i++)
        {
            for (int step = 0; step < 16; step++)
            {
                instr.play();
                data.time.tick += 6;
            }
            printf("\n");
        }
        printf("\n");
    }

    //void  test_mfb_503_kick()
    //{
    //    ApplicationData data;
    //    data.mfb_503_kick.set_volume(0);
    //    test_instrument(data, data.mfb_503_kick);
    //}

    //void  test_mfb_503_toms()
    //{
    //    ApplicationData data;
    //    data.mfb_503_toms.set_volume(0);
    //    test_instrument(data, data.mfb_503_toms);
    //}

    //void test_mfb_503_cymbal()
    //{
    //    ApplicationData data;
    //    data.mfb_503_cymbal.set_volume(0);
    //    test_instrument(data, data.mfb_503_cymbal);
    //}

    void test_set_diddles()
    {
        for (int i = 0; i < 16; i++)
        {
            GatePattern16 pat;
            pat.pattern.set_diddles(.25, true, 16);
            print_pattern(pat);
        }
    }

    void test_shift_one()
    {
        uint8_t max_tests = 8;
        for (int i = 0; i < max_tests; i++)
        {
            GatePatternAB pattern;
            pattern.set_coef_hat_pattern();
            uint8_t ab_idx = pattern.abPattern.value(Rand::randui8(4));
            //ofLogVerbose("TEST", "ab_idx = %d", ab_idx);
            pattern.patterns[ab_idx].shift_up();
            print_pattern(pattern);
            //ofLogNotice("TEST","");
        }
    }

    void test_settings()
    {
        Modulators mod;
        TimeStruct time;
        TanzbarCp tbm(mod, time);
        Mfb522Clap mfbs(mod, time);

        std::cout << "TB  p_off" << tbm.settings.p_off << std::endl;
        std::cout << "MFB p_off" << mfbs.settings.p_off << std::endl;
    }

    int main()
    {
        test_settings();

        return 0;
    }

}

int main()
{
    ofLogToConsole();
    return Vleerhond::main();
}