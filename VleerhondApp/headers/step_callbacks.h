#pragma once

#include "defs.h"

namespace Vleerhond
{
    void release_button_left_top_07(ApplicationData& data)
    {
        //data.mono.total_randomize();
        //switch (Rand::randui8(2))
        //{
        //case 0: data.mono.set_style(MonoStyle::MonoPolyRhythm); break;
        //case 1: data.mono.set_style(MonoStyle::MonoLeadPattern); break;
        //}
        //data.mono.set_style(MonoStyle::MonoPolyRhythm);

        // Rocket experiment
        data.neutron_mono.total_randomize();
        //switch (Rand::randui8(2))
        //{
        //case 0: data.rocket_mono.set_style(MonoStyle::MonoPolyRhythm); break;
        //case 1: data.rocket_mono.set_style(MonoStyle::MonoLeadPattern); break;
        //}
        data.neutron_mono.set_style(MonoStyle::MonoPolyRhythm);

        //data.fugue_vermona_3.randomize();
    }

    void release_button_left_btm_07(ApplicationData& data)
    {
        //data.mono.total_randomize();
        //data.mono.set_style(MonoStyle::MonoSixteenths);
        //data.fugue_vermona_4.randomize();

        // Rocket experiment
        data.neutron_mono.total_randomize();
        data.neutron_mono.set_style(MonoStyle::MonoSixteenths);

    }

    void release_buttons_left_07(ApplicationData& data)
    {
        //data.mono.total_randomize();
        //switch (Rand::randui8(2))
        //{
        //case 0: data.mono.set_style(MonoStyle::MonoPolyRhythm); break;
        //case 1: data.mono.set_style(MonoStyle::MonoLeadPattern); break;
        //}
        //data.mono.set_arp_type(ArpType::CLOSEST);

        // Neutron
        data.neutron_mono.total_randomize();
        switch (Rand::randui8(2))
        {
        case 0: data.neutron_mono.set_style(MonoStyle::MonoPolyRhythm); break;
        case 1: data.neutron_mono.set_style(MonoStyle::MonoLeadPattern); break;
        }
        data.neutron_mono.set_arp_type(ArpType::CLOSEST);
    }
}