#pragma once

#include "bass_settings.h"
#include "defs.h"
#include "fugue_settings.h"
#include "harmony_struct.h"
#include "modulators.h"
#include "tanzbar_settings.h"

class ApplicationData 
{
public:
    TimeStruct time;
    HarmonyStruct harmony;

    FugueSettings fugue_settings;
    Modulators modulators;

    TanzbarSettings tanzbar_settings;
    BassSettings bass_settings;
    BassDubSettings bass_dub_settings;
    MonoSettings mono_settings;
    MonoDubSettings mono_dub_settings;

    UiState ui_state;

    ApplicationData()
    {
        tanzbar_settings.storage.channel = MIDI_CHANNEL_TANZBAR;
        mono_settings.storage.channel = MIDI_CHANNEL_MONO;
        mono_dub_settings.settings.storage.channel = MIDI_CHANNEL_MONO_2;
        bass_settings.storage.channel = MIDI_CHANNEL_BASS;
        bass_dub_settings.storage.channel = MIDI_CHANNEL_BASS_DUB;

        bass_settings.fugue_id = 0;
        bass_dub_settings.fugue_id = 1;
        mono_settings.fugue_id = 2;
        mono_dub_settings.settings.fugue_id = 3;
    }
};
