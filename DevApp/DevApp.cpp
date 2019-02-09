#include "pch.h"
#include <iostream>
#include "stdio.h"
#include <stdlib.h>

#include "arp.h"
#include "app_utils.h"
#include "coef.h"
#include "defs.h"
#include "euclid.h"
#include "gate.h"
#include "init.h"
#include "intervals.h"
#include "note_handling.h"
#include "print.h"
#include "rand.h"
#include "rhythms.h"
#include "utils.h"

#include "lead.h"
#include "mfb_503.h"
#include "mfb_522.h"
#include "mono.h"
#include "poly.h"
#include "bass.h"
#include "bass_dub.h"

void send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
{
    printf("note_on(%-3d, %-3d, %-2d)\n", pitch, velocity, channel);
}

void send_note_off(const uint8_t pitch, const uint8_t channel)
{
    printf("note_off(%-3d, %-2d)\n", pitch, channel);
}

void send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    printf("send_cc(%-3d, %-3d, %-2d)\n", cc, value, channel);
}

void test_randf()
{
    uint16_t numbers[10] = { 0 };
    for (int i = 0; i < 10; i++) numbers[i] = 0;

    for (int i = 0; i < 10000; i++)
    {
        float random = randf();
        for (int i = 1; i < 11; i++)
        {
            if (random < i * .1f)
            {
                numbers[i-1]++;
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

void test_randi()
{
    uint16_t numbers[16];
    for (int i = 0; i < 16; i++) numbers[i] = 0;

    for (int i = 0; i < 16000; i++)
    {
        numbers[randi(16)]++;
    }

    for (int i = 0; i < 16; i++)
    {
        printf("%d ", numbers[i]);
    }
    printf("\n");
}

void test_set_euclid()
{
    BinaryPattern pattern;
    uint8_t length = 16;
    uint8_t steps = 5;
    print_pattern(pattern, length);
    set_euclid(pattern, length, steps);
    print_pattern(pattern, length);
}

void test_coef()
{
    GatePatternAB pattern = { 0 };
    set_coef_kick_pattern(pattern);
    print_pattern(pattern);
    printf("\n");

    set_coef_hat_pattern(pattern);
    print_pattern(pattern);
    printf("\n");

    set_coef_snare_pattern(pattern);
    print_pattern(pattern);
    printf("\n");

    randomize(pattern, 0.f);
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
            uint8_t p = get_next_arp_pitch(data, scale, chord);
            printf("%-2d ", p);
        }
        printf("\n");
    }
    printf("\n");
}

void arp_test()
{
    printf("Test Arp\n");
    ArpData data = init_arp_data();
    Scale scale = get_scale(AEOLIAN, ROOT_D);

    data.type = ArpType::CLOSEST;
    data.range = 16;
    data.min = 32;
    data.counter = 0;

    for (int chord = 0; chord < 8; chord += 2)
    {
        for (int i = 0; i < 12; i++)
        {
            printf("%d ", is_in_chord(i, scale, chord));
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
    PitchStorage storage = { 0 };
    print_storage(storage);
    add_to_storage(make_note(36, 5), storage);
    print_storage(storage);
    add_to_storage(make_note(36, 1), storage);
    print_storage(storage);
    add_to_storage(make_note(38, 3), storage);
    print_storage(storage);
    add_to_storage(make_note(40, 2), storage);
    print_storage(storage);
    stop_notes(storage);
    print_storage(storage);
    stop_notes(storage);
    print_storage(storage);
    stop_notes(storage);
    print_storage(storage);
}

void interval_pattern_test()
{
    uint16_t counter_4 = 0;
    uint16_t counter_8 = 0;
    uint16_t counter_16 = 0;
    uint16_t counter_32 = 0;
    uint16_t counter_12 = 0;
    IntervalPattern pattern = init_interval_pattern();
    //IntervalProbs probs = { 4, 4, 4, 4, 4 };
    IntervalProbs probs = { 2, 0, 0, 2, 2 };
    for (int x = 0; x < 1000; x++)
    {
        randomize_interval(pattern, probs);
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
                    //uint32_t s1 = get_step(pattern.time_division, step, tick, false);
                    //uint8_t s = (uint8_t)(s1 % 16L);
                    TimeDivision div = interval(pattern, step, tick);
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
            printf("%-3d ", get_count(TimeDivision::Sixteenth, step++, 0));
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_gates()
{
    printf("Test Gates\n");
    ApplicationData data = init_application_data();
    randomize_503_seq(data);

    print_pattern(data.mfb_503_settings.bd_pattern);

    data.time.step = 0;
    for (int bar = 0; bar < 16; bar++)
    {
        for (int _step = 0; _step < 16; _step++)
        {
            printf("%-3d ", gate(data.mfb_503_settings.bd_pattern, data.time));
            data.time.step++;
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_chords()
{
    printf("Test Chords\n");
    ApplicationData data = init_application_data();
    randomize_all(data);
    data.harmony.type = HarmonyType::HarmonyHigh;

    uint32_t step = 0;
    for (int bar = 0; bar < 16; bar++)
    {
        for (int _step = 0; _step < 16; _step++)
        {
            printf("%-3d ", get_chord_step(data.harmony, data.time));
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_503()
{
    printf("Test 503\n");
    ApplicationData data = init_application_data();
    randomize_all(data);
    data.harmony.type = HarmonyType::HarmonyHigh;
    randomize_503_seq(data);

    data.mfb_503_settings.play_pitch_bd = true;

    uint32_t step = 0;
    for (int bar = 0; bar < 16; bar++)
    {
        for (int _step = 0; _step < 16; _step++)
        {
            data.time.step = step++;
            play_bd(data, data.time);
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_poly()
{

    printf("Test Poly\n");
    ApplicationData data = init_application_data();
    randomize_all(data);
    data.harmony.type = HarmonyType::HarmonyHigh;

    uint32_t step = 0;
    for (int bar = 0; bar < 16; bar++)
    {
        printf("bar %-2d\n", bar);
        for (int _step = 0; _step < 16; _step++)
        {
            printf("step %-2d\n", step);
            data.time.step = step++;
            play_poly(data, data.time);

            //print_storage(data.poly_settings.storage);

            for(int tick = 0; tick < 6; tick++)
                stop_notes(data.poly_settings.storage);
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_play_all()
{
    printf("Test Play All\n");
    ApplicationData data = init_application_data();
    randomize_all(data);
    play_all(data);
}

void test_mono()
{

    printf("Test Mono\n");
    ApplicationData data = init_application_data();
    randomize_all(data);
    data.harmony.type = HarmonyType::HarmonyHigh;

    data.mono_settings.style = MonoStyle::MonoLeadPattern;

    uint32_t step = 0;
    for (int bar = 0; bar < 16; bar++)
    {
        printf("bar %-2d\n", bar);
        for (int _step = 0; _step < 16; _step++)
        {
            printf("step %-2d\n", step);
            data.time.step = step++;
            play_mono(data, data.mono_settings, data.harmony, data.time);

            print_storage(data.mono_settings.storage);

            /*for (int tick = 0; tick < 6; tick++)
                stop_notes(data.mono_settings.storage, MIDI_CHANNEL_MONO);*/
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_bass()
{
    printf("Test Bass\n");
    ApplicationData data = init_application_data();
    randomize_all(data);

    data.harmony.type = HarmonyType::HarmonyHigh;

    data.bass_settings.style = BassStyle::BassEuclid;
    data.bass_dub_settings.style = BassDubStyle::DubHitProbability;

    uint32_t step = 0;
    for (int bar = 0; bar < 4; bar++)
    {
        printf("bar %-2d\n", bar);
        for (int _step = 0; _step < 16; _step++)
        {
            printf("step %-2d\n", step);
            data.time.step = step++;
            play_bass(data, data.time);
            play_bass_dub(
                data,
                data.bass_settings,
                data.bass_dub_settings,
                data.harmony,
                data.time);

            //print_storage(data.mono_settings.storage);

            for (int tick = 0; tick < 6; tick++)
                stop_notes_all_instruments(data);
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_fugue()
{
    printf("Test Fugue\n");
    ApplicationData data = init_application_data();
    randomize_all(data);
    set_fugue(data);

    print_pattern(data.fugue_settings.pattern);

    data.harmony.type = HarmonyType::HarmonyConst;

    uint32_t step = 0;
    for (int bar = 0; bar < 4; bar++)
    {
        printf("bar %-2d\n", bar);
        for (int _step = 0; _step < 16; _step++)
        {
            printf("step %-2d\n", step);
            data.time.step = step++;
            //play_all(data);
            play_bass(data, data.time);
            play_bass_dub(
                data,
                data.bass_settings,
                data.bass_dub_settings,
                data.harmony,
                data.time);
            play_mono(data, data.mono_settings, data.harmony, data.time);
            play_mono_dub(data, data.mono_dub_settings, data.mono_settings, data.harmony, data.time);

            //print_storage(data.mono_settings.storage);

            for (int tick = 0; tick < 6; tick++)
                stop_notes_all_instruments(data);
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_randomize_interval_lead()
{
    printf("Test Randomize Interval Lead\n");
    IntervalPattern p = init_interval_pattern();
    for (int i = 0; i < 16; i++)
    {
        randomize_interval_lead(p);
        print_pattern(p);
    }
}

void test_distributed_range()
{
    for (int i = 0; i < 128; i++)
    {
        uint8_t dist = get_distributed_range(randi(128), i, 3);
        printf("%d\n", dist);
    }
}

int main()
{
    test_fugue();

    //test_bass();
    //test_distributed_range();
    //test_randomize_interval_lead();
    //test_mono();
    //test_play_all();
    //test_poly();
    //test_503();
    //test_chords();
    //test_intervals();
    //test_gates();
    //test_set_euclid();
    //arp_test();
    //interval_pattern_test();
    //storage_test();
    //test_coef();
    //test_randf();
    //test_randi();
}
