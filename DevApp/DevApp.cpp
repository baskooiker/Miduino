#include "pch.h"
#include <iostream>
#include "stdio.h"
#include <stdlib.h>

#include "arp.h"
#include "coef.h"
#include "defs.h"
#include "euclid.h"
#include "init.h"
#include "intervals.h"
#include "lead.h"
#include "p50.h"
#include "rand.h"
#include "rocket.h"
#include "storage.h"
#include "utils.h"

void note_on(uint8_t note, uint8_t velocity, uint8_t channel, PitchStorage& storage, uint8_t length){}

void note_off(uint8_t note, uint8_t channel, PitchStorage& storage)
{
    pop_from_storage(storage, note);
}

void send_cc(uint8_t cc, uint8_t value, uint8_t channel){}

void all_notes_off(PitchStorage& storage, uint8_t channel) {}

void print_pattern(BinaryPattern& pattern, uint8_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        printf("%d ", gate(pattern, i));
    }
    printf("\n");
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
    set_euclid(pattern, length, steps);
    print_pattern(pattern, length);
}

void test_coef()
{
    GatePatternAB pattern;
    set_coef_kick_pattern(pattern);
    print_pattern(pattern);
    printf("");

    set_coef_hat_pattern(pattern);
    print_pattern(pattern);
    printf("");

    randomize(pattern, 0.f);
    print_pattern(pattern);
    printf("");
}

void test_setup()
{
    ApplicationData data;

    randomize_503_seq(data);
    randomize_522_seq(data);
    randomize_P50_seq(data);
    randomize_rocket_seq(data);
    randomize_lead(data);
}

void arp_test()
{
    ArpData data = { 0 };
    Scale scale = get_scale(AEOLIAN, ROOT_D);

    data.type = ArpType::CLOSEST;
    data.range = 12;
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

    for (int chord = 0; chord < 8; chord += 2)
    {
        for (int i = 0; i < 8; i++)
        {
            uint8_t p = get_arp_pitch(data, scale, chord);
            printf("%2d ", p);
        }
        printf("\n");
    }
}

void print_storage(PitchStorage& storage)
{
    printf("\nsize: %d\n", storage.size);
    for (int i = 0; i < storage.size; i++)
    {
        printf("%2d %3d\n", storage.data[i].pitch, storage.data[i].length);
    }
}

void storage_test()
{
    PitchStorage storage = { 0 };
    print_storage(storage);
    add_to_storage(storage, 36, 5);
    print_storage(storage);
    add_to_storage(storage, 36, 1);
    print_storage(storage);
    add_to_storage(storage, 38, 3);
    print_storage(storage);
    add_to_storage(storage, 40, 2);
    print_storage(storage);
    stop_notes(storage, 0);
    print_storage(storage);
    stop_notes(storage, 0);
    print_storage(storage);
    stop_notes(storage, 0);
    print_storage(storage);
    stop_notes(storage, 0);
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
    IntervalProbs probs = { 4, 4, 4, 4, 4 };
    //IntervalProbs probs = { 4, 2, 0, 2, 2 };
    for (int x = 0; x < 1000; x++)
    {
        randomize_interval(pattern, probs);
        for (int i = 0; i < 16; i++)
        {
            switch (pattern.pattern[i])
            {
            case TimeDivision::TIME_DIVISION_FOURTH: counter_4++; break;
            case TimeDivision::TIME_DIVISION_EIGHT: counter_8++; break;
            case TimeDivision::TIME_DIVISION_SIXTEENTH: counter_16++; break;
            case TimeDivision::TIME_DIVISION_THIRTYTWO: counter_32++; break;
            case TimeDivision::TIME_DIVISION_TRIPLE_EIGHT: counter_12++; break;
            }
            //printf("%2d ", pattern.pattern[i]);
        }
        //printf("\n");
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

int main()
{
    interval_pattern_test();
    //storage_test();
    //arp_test();
    //test_setup();
    //test_coef();
    //test_set_euclid();
    //test_randf();
    //test_randi();
}
