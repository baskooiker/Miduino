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
#include "app_utils.h"
#include "callbacks.h"
#include "coef.h"
#include "defs.h"
#include "euclid.h"
#include "intervals.h"
#include "modulators.h"
#include "print.h"
#include "rand.h"
#include "utils.h"

#include "lead.h"
#include "mfb_503.h"
#include "mono.h"
#include "poly.h"
#include "bass.h"
#include "bass_dub.h"

static uint16_t note_hits[12] = { 0 };

void reset_note_hits()
{
    for (int i = 0; i < 12; i++)
    {
        note_hits[i] = 0;
    }
}

void print_note_hits()
{
    for (int i = 0; i < 12; i++)
    {
        printf("Note %-2d: %-3d\n", i, note_hits[i]);
    }
    printf("\n");
}

void send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
{
    printf("note_on(%-3d, %-3d, %-2d)\n", pitch, velocity, channel);
    note_hits[pitch % 12]++;
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

void test_randui8()
{
    uint16_t numbers[16];
    for (int i = 0; i < 16; i++) numbers[i] = 0;

    for (int i = 0; i < 16000; i++)
    {
        numbers[randui8(16)]++;
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
        numbers[randi8(-5, 5) + 5]++;
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
    set_euclid(pattern, length, steps);
    print_pattern(pattern, length);
}

void test_coef()
{
    GatePatternAB pattern;
    set_coef_kick_pattern(pattern);
    print_pattern(pattern);
    printf("\n");

    set_coef_hat_pattern(pattern);
    print_pattern(pattern);
    printf("\n");

    set_coef_snare_pattern(pattern);
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
    ArpData data;
    Scale scale(AEOLIAN, ROOT_D);

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
    PitchStorage storage;
    storage.print_storage();
    storage.add_to_storage(make_note(36, 5));
    storage.print_storage();
    storage.add_to_storage(make_note(36, 1));
    storage.print_storage();
    storage.add_to_storage(make_note(38, 3));
    storage.print_storage();
    storage.add_to_storage(make_note(40, 2));
    storage.print_storage();
    storage.stop_notes();
    storage.print_storage();
    storage.stop_notes();
    storage.print_storage();
    storage.stop_notes();
    storage.print_storage();
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
                    TimeStruct time;
                    time.tick = tick + step * TICKS_PER_STEP;
                    //uint32_t s1 = get_step(pattern.time_division, step, tick, false);
                    //uint8_t s = (uint8_t)(s1 % 16L);
                    TimeDivision div = interval(pattern, time);
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
            printf("%-3d ", get_count(TimeDivision::Sixteenth, time));
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_gates()
{
    printf("Test Gates\n");
    ApplicationData data;
    data.tanzbar_settings.randomize_tanzbar();

    print_pattern(data.tanzbar_settings.bd_pattern);

    for (int bar = 0; bar < 16; bar++)
    {
        for (int _step = 0; _step < 16; _step++)
        {
            printf("%-3d ", data.tanzbar_settings.bd_pattern.gate(data.time));
            data.time.tick += 6;
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_chords()
{
    printf("Test Chords\n");
    ApplicationData data;
    randomize_all(data);
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

void test_503()
{
    printf("Test 503\n");
    ApplicationData data;
    randomize_all(data);
    Mfb503Settings settings;
    data.harmony.type = HarmonyType::HarmonyHigh;

    randomize_503_seq(settings);

    settings.play_pitch_bd = true;

    uint32_t step = 0;
    for (int bar = 0; bar < 16; bar++)
    {
        for (int _step = 0; _step < 16; _step++)
        {
            data.time.tick = TICKS_PER_STEP * step++;
            play_bd(settings, data.harmony, data.time);
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_poly()
{

    printf("Test Poly\n");
    ApplicationData data;
    randomize_all(data);
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
    randomize_all(data);
    play_all(data);
}

void test_mono()
{

    printf("Test Mono\n");
    ApplicationData data;
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
            data.time.tick = TICKS_PER_STEP * step++;
            play_mono(data, data.mono_settings, data.harmony, data.time);

            data.mono_settings.storage.print_storage();
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_bass()
{
    printf("Test Bass\n");
    ApplicationData data;
    randomize_all(data);

    data.harmony.type = HarmonyType::HarmonyHigh;

    data.bass_settings.style = BassStyle::BassEuclid;
    data.bass_dub_settings.style = BassDubStyle::DubUnison;
    data.bass_dub_settings.density = 127;
    data.bass_settings.pitch_range = 0;
    data.bass_settings.density = 0;

    uint32_t step = 0;
    for (int bar = 0; bar < 4; bar++)
    {
        printf("bar %-2d\n", bar);
        for (int _step = 0; _step < 16; _step++)
        {
            printf("step %-2d\n", step);
            data.time.tick = TICKS_PER_STEP * step++;
            play_bass(
                data.bass_settings, 
                data.harmony,
                data.fugue_settings,
                data.time
            );
            //play_bass_dub(
            //    data,
            //    data.bass_settings,
            //    data.bass_dub_settings,
            //    data.harmony,
            //    data.time);

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
    reset_note_hits();
    ApplicationData data;
    randomize_all(data);
    print_scale(data.harmony.scale);
    set_fugue(data);

    print_scale(data.harmony.scale);

    print_pattern(data.fugue_settings.pattern);

    data.harmony.type = HarmonyType::HarmonyConst;

    uint32_t step = 0;
    for (int bar = 0; bar < 4; bar++)
    {
        printf("bar %-2d\n", bar);
        for (int _step = 0; _step < 16; _step++)
        {
            printf("step %-2d\n", step);
            data.time.tick = TICKS_PER_STEP * step++;
            //play_all(data);
            //play_bass(data, data.time);
            //play_bass_dub(
            //    data,
            //    data.bass_settings,
            //    data.bass_dub_settings,
            //    data.harmony,
            //    data.time);
            //play_mono(data, data.mono_settings, data.harmony, data.time);
            play_mono_dub(data, data.mono_dub_settings, data.mono_settings, data.harmony, data.time);

            //print_storage(data.mono_settings.storage);

            for (int tick = 0; tick < 6; tick++)
                stop_notes_all_instruments(data);
        }
        printf("\n");
    }
    printf("\n\n");
    print_note_hits();

    printf("dub style = %2d, mono style = %2d\n", data.mono_dub_settings.style, data.mono_dub_settings.settings.style);
}

void test_randomize_interval_lead()
{
    printf("Test Randomize Interval Lead\n");
    IntervalPattern p;
    for (int i = 0; i < 16; i++)
    {
        randomize_interval_lead(p);
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

void test_double_callbacks()
{
    printf("test double callbacks\n");
    ApplicationData data;

    press_step(data.ui_state.step_state, BSP_STEP_02);
    handle_step_release(data, BSP_STEP_01, BSP_STEP_01, BSP_STEP_02, cb_1, cb_2, cb_3);

    release_step(data.ui_state.step_state, BSP_STEP_02);
    handle_step_release(data, BSP_STEP_01, BSP_STEP_01, BSP_STEP_02, cb_1, cb_2, cb_3);

    press_step(data.ui_state.step_state, BSP_STEP_01);
    handle_step_release(data, BSP_STEP_02, BSP_STEP_01, BSP_STEP_02, cb_1, cb_2, cb_3);

    release_step(data.ui_state.step_state, BSP_STEP_01);
    handle_step_release(data, BSP_STEP_02, BSP_STEP_01, BSP_STEP_02, cb_1, cb_2, cb_3);
}

void test_shuffle()
{
    printf("Test shuffle\n");

    TimeStruct time;
    reset_time(time);
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
        get_chord_time_pattern(chord_time_pattern, length);
        for (int j = 0; j < length; j++)
            printf("%2d ", chord_time_pattern[j]);
        printf("\n");
    }

    printf("\n");

    for (int i = 0; i < 16; i++)
    {
        uint8_t chords[] = {5, 6, 7, 8};
        uint8_t seq[4] = { 0, 0, 0, 0 };
        uint8_t length = i % 4 + 1;
        get_chord_seq(chords, length, seq);
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
            set_chord_pattern(pat, scale);
        else
            set_chord_pattern(pat, scale, 4);
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

void test_application_settings()
{
    printf("Test ApplicationData\n");
    ApplicationData data;

    ChannelStruct channels[4];
    uint8_t length;
    data.bass_settings.storage.get_channels(channels, length);
    for (int i = 0; i < length; i++)
    {
        printf("%2d: %2d\n", channels[i].channel, channels[i].pitch_offset);
    }
}

void time_struct_test()
{
    printf("TimeStruct Test\n");
    TimeStruct time;
    time.average_pulse_time = 60000.f / 120.f;
    printf("Maximum offset (120 bpm): %d\n", time.get_shuffle_delay(0, 127));

    time.average_pulse_time = 60000.f / 140.f;
    printf("Maximum offset (140 bpm): %d\n", time.get_shuffle_delay(0, 127));
}

int main()
{
    //test_shuffle();
    //test_double_callbacks();
    //test_fugue();
    //test_bass();
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
    //test_randui8();
    //test_randi8();
    //test_lfo();
    //test_chord_patterns();
    //test_scale();
    //test_harmony();
    //test_application_settings();
    time_struct_test();
}
