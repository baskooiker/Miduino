#pragma once

#include "defs.h"

namespace Vleerhond
{
    void release_button_left_top_07(ApplicationData& data)
    {
        data.mono_settings.total_randomize();
        switch (Rand::randui8(2))
        {
        case 0: data.mono_settings.set_style(MonoStyle::MonoPolyRhythm); break;
        case 1: data.mono_settings.set_style(MonoStyle::MonoLeadPattern); break;
        }
    }

    void release_button_left_btm_07(ApplicationData& data)
    {
        data.mono_settings.total_randomize();
        data.mono_settings.set_style(MonoStyle::MonoSixteenths);
    }

    void release_buttons_left_07(ApplicationData& data)
    {
        data.mono_settings.total_randomize();
        switch (Rand::randui8(2))
        {
        case 0: data.mono_settings.set_style(MonoStyle::MonoPolyRhythm); break;
        case 1: data.mono_settings.set_style(MonoStyle::MonoLeadPattern); break;
        }
        data.mono_settings.set_arp_type(ArpType::CLOSEST);
    }

    void release_button_left_top_08(ApplicationData& data)
    {
        data.mono_dub_settings.total_randomize();
        data.mono_dub_settings.dub_style = MonoDubStyle::MonoDubLead;
        
        switch (Rand::randui8(2))
        {
        case 0: data.mono_dub_settings.lead_settings.set_style(MonoStyle::MonoPolyRhythm); break;
        case 1: data.mono_dub_settings.lead_settings.set_style(MonoStyle::MonoLeadPattern); break;
        }
    }

    void release_button_left_btm_08(ApplicationData& data)
    {
        data.mono_dub_settings.total_randomize();
        data.mono_dub_settings.dub_style = MonoDubStyle::MonoDubLead;
        data.mono_dub_settings.lead_settings.set_style(MonoStyle::MonoSixteenths);
    }

    void release_buttons_left_08(ApplicationData& data)
    {
        ofLogNotice("step_callbacks", "Release left 08 both");
        data.mono_dub_settings.total_randomize();
        data.mono_dub_settings.dub_style = MonoDubStyle::MonoDubUnison;
    }
}