#pragma once

#include "defs.h"

void release_step_1(ApplicationData& data)
{
    data.tanzbar_settings.randomize_tanzbar_sound();
    data.tanzbar_settings.randomize_tanzbar();
    data.time.global_shuffle = randui8(64);
    data.modulators.randomize();
}

void release_step_2(ApplicationData& data)
{
    data.tanzbar_settings.randomize_tanzbar_sound();
    data.tanzbar_settings.randomize_tanzbar();
    data.tanzbar_settings.randomize_tanzbar_kick();
    data.time.global_shuffle = randui8(64);
    data.modulators.randomize();
}

void release_step_1_2(ApplicationData& data) {}

void release_step_3(ApplicationData& data)
{

}

void release_step_4(ApplicationData& data)
{

}

void release_step_3_4(ApplicationData& data) {}

void release_step_5(ApplicationData& data)
{
    data.harmony.randomize();
    data.harmony.type = HarmonyType::HarmonyConst;
    data.harmony.const_value = 0;
}

void release_step_6(ApplicationData& data)
{
    data.harmony.randomize();
    data.harmony.type = HarmonyType::HarmonyConst;
    data.harmony.switch_const_chord();
}


void release_step_5_and_6(ApplicationData& data) 
{
    data.harmony.randomize();
    data.harmony.type = HarmonyType::HarmonyHigh;
}

void release_step_7(ApplicationData& data)
{

}

void release_step_8(ApplicationData& data)
{

}

void release_step_7_8(ApplicationData& data) {}

void release_step_9(ApplicationData& data)
{
    data.bass_settings.randomize();
}

void release_step_10(ApplicationData& data)
{
    data.bass_settings.randomize();
}

void release_step_9_10s(ApplicationData& data) 
{
    data.bass_settings.randomize();
}

void release_step_11(ApplicationData& data)
{
    randomize_bass_dub(data.bass_dub_settings);
    data.bass_dub_settings.style = BassDubStyle::DubUnison;
}

void release_step_12(ApplicationData& data)
{
    randomize_bass_dub(data.bass_dub_settings);
    data.bass_dub_settings.style = BassDubStyle::DubOctave;
}

void release_step_11_and_12(ApplicationData& data)
{
    randomize_bass_dub(data.bass_dub_settings);
    data.bass_dub_settings.style = BassDubStyle::DubOctProbability;
}

void release_step_13(ApplicationData& data)
{
    randomize_mono(data.mono_settings);
    switch (randui8(2))
    {
    case 0: data.mono_settings.style = MonoStyle::MonoPolyRhythm; break;
    case 1: data.mono_settings.style = MonoStyle::MonoLeadPattern; break;
    }
}

void release_step_14(ApplicationData& data)
{
    randomize_mono(data.mono_settings);
    data.mono_settings.style = MonoStyle::MonoSixteenths;
}

void release_step_13_and_14(ApplicationData& data)
{
    randomize_mono(data.mono_settings);
    switch (randui8(2))
    {
    case 0: data.mono_settings.style = MonoStyle::MonoPolyRhythm; break;
    case 1: data.mono_settings.style = MonoStyle::MonoLeadPattern; break;
    }

    data.mono_settings.arp_data.type = ArpType::CLOSEST;
}

void release_step_15(ApplicationData& data)
{
    randomize_mono_dub(data.mono_dub_settings);
    data.mono_dub_settings.style = MonoDubStyle::MonoDubLead;
    switch (randui8(2))
    {
    case 0: data.mono_dub_settings.settings.style = MonoStyle::MonoPolyRhythm; break;
    case 1: data.mono_dub_settings.settings.style = MonoStyle::MonoLeadPattern; break;
    }
}

void release_step_16(ApplicationData& data)
{
    randomize_mono_dub(data.mono_dub_settings);
    data.mono_dub_settings.style = MonoDubStyle::MonoDubLead;
    data.mono_dub_settings.settings.style = MonoStyle::MonoSixteenths;
}

void release_step_15_and_16(ApplicationData& data)
{
    randomize_mono_dub(data.mono_dub_settings);
    data.mono_dub_settings.style = MonoDubStyle::MonoDubUnison;
}
