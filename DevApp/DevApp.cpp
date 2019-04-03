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
#include "callbacks.h"
#include "defs.h"
#include "euclid.h"
#include "intervals.h"
#include "modulators.h"
#include "print.h"
#include "rand.h"
#include "utils.h"
#include "print_midi.h"
#include "note_struct.h"
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
    storage.add_to_storage(NoteStruct(36, 5));
    storage.print_storage();
    storage.add_to_storage(NoteStruct(36, 1));
    storage.print_storage();
    storage.add_to_storage(NoteStruct(38, 3));
    storage.print_storage();
    storage.add_to_storage(NoteStruct(40, 2));
    storage.print_storage();
    storage.process_active_notes();
    storage.print_storage();
    storage.process_active_notes();
    storage.print_storage();
    storage.process_active_notes();
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

void test_gates()
{
    printf("Test Gates\n");
    ApplicationData data;
    data.tanzbar.randomize_tanzbar();

    print_pattern(data.tanzbar.bd_pattern);

    for (int bar = 0; bar < 16; bar++)
    {
        for (int _step = 0; _step < 16; _step++)
        {
            printf("%-3d ", data.tanzbar.bd_pattern.gate(data.time));
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

void test_503()
{
    printf("Test 503\n");
    ApplicationData data;
    data.randomize_all();
    Mfb503Settings settings(data.harmony, data.time);
    data.harmony.type = HarmonyType::HarmonyHigh;

    settings.randomize_503_seq();

    settings.play_pitch_bd = true;

    uint32_t step = 0;
    for (int bar = 0; bar < 16; bar++)
    {
        for (int _step = 0; _step < 16; _step++)
        {
            data.time.tick = TICKS_PER_STEP * step++;
            settings.play_bd(data.harmony, data.time);
        }
        printf("\n");
    }
    printf("\n\n");
}

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

    data.mono_settings.style = MonoStyle::MonoLeadPattern;

    uint32_t step = 0;
    for (int bar = 0; bar < 16; bar++)
    {
        printf("bar %-2d\n", bar);
        for (int _step = 0; _step < 16; _step++)
        {
            printf("step %-2d\n", step);
            data.time.tick = TICKS_PER_STEP * step++;
            data.mono_settings.play();

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
    data.randomize_all();

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
            data.bass_settings.play();
            data.bass_dub_settings.play();

            data.mono_settings.storage.print_storage();

            for (int tick = 0; tick < 6; tick++)
                data.process_active_notes();
        }
        printf("\n");
    }
    printf("\n\n");
}

void test_fugue()
{
    printf("Test Fugue\n");
    PrintMidi::reset_note_hits();
    ApplicationData data;
    data.randomize_all();
    print_scale(data.harmony.scale);
    data.set_fugue();

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
            //data.play_all();
            //play_bass(data, data.time);
            //play_bass_dub(
            //    data,
            //    data.bass_settings,
            //    data.bass_dub_settings,
            //    data.harmony,
            //    data.time);
            //play_mono(data, data.mono_settings, data.harmony, data.time);
            //data.mono_dub_settings.play();

            //print_storage(data.mono_settings.storage);

            for (int tick = 0; tick < 6; tick++)
                data.process_active_notes();
        }
        printf("\n");
    }
    printf("\n\n");
    PrintMidi::print_note_hits();

    //printf(
    //    "dub style = %2d, mono style = %2d\n", 
    //    data.mono_dub_settings.dub_style, 
    //    data.mono_dub_settings.style
    //);
}

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
        uint8_t chords[] = {5, 6, 7, 8};
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
    //time_struct_test();
    test_utils_rerange();
}
