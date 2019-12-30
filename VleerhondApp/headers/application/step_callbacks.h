#pragma once

#include "core/defs.h"

namespace Vleerhond
{
    void release_button_left_top_07(ApplicationData& data)
    {
        data.neutron_mono.total_randomize();
        data.neutron_mono.set_style(MonoStyle::MonoPolyRhythm);
        data.neutron_mono.set_pitch_mode(MonoPitchMode::SEQUENCE);
    }

    void release_button_left_btm_07(ApplicationData& data)
    {
        // Rocket experiment
        data.neutron_mono.total_randomize();
        data.neutron_mono.set_style(MonoStyle::MonoSixteenths);
        data.neutron_mono.set_pitch_mode(MonoPitchMode::ARP);
    }

    void release_buttons_left_07(ApplicationData& data)
    {
        // Neutron
        data.neutron_mono.total_randomize();
        data.neutron_mono.set_style(MonoStyle::MonoPolyRhythm);
        data.neutron_mono.set_arp_type(ArpType::CLOSEST);
        data.neutron_mono.set_pitch_mode(MonoPitchMode::ARP);
    }
}