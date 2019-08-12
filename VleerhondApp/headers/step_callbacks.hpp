#pragma once

#include "defs.hpp"

namespace Vleerhond
{
    void release_button_left_top_07(ApplicationData& data)
    {
        data.mono.total_randomize();
        switch (Rand::randui8(2))
        {
        case 0: data.mono.set_style(MonoStyle::MonoPolyRhythm); break;
        case 1: data.mono.set_style(MonoStyle::MonoLeadPattern); break;
        }
    }

    void release_button_left_btm_07(ApplicationData& data)
    {
        data.mono.total_randomize();
        data.mono.set_style(MonoStyle::MonoSixteenths);
    }

    void release_buttons_left_07(ApplicationData& data)
    {
        data.mono.total_randomize();
        switch (Rand::randui8(2))
        {
        case 0: data.mono.set_style(MonoStyle::MonoPolyRhythm); break;
        case 1: data.mono.set_style(MonoStyle::MonoLeadPattern); break;
        }
        data.mono.set_arp_type(ArpType::CLOSEST);
    }

    void release_button_left_top_08(ApplicationData& data)
    {
    }

    void release_button_left_btm_08(ApplicationData& data)
    {
    }

    void release_buttons_left_08(ApplicationData& data)
    {
        data.mono_dub.total_randomize();
    }
}