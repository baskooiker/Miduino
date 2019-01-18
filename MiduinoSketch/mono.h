#pragma once

void randomize_mono(ApplicationData& data)
{
    data.settings_mono.arp_data.min = randi(36, 60);
    data.settings_mono.arp_data.range = randi(12, 24);
    switch (randi(4))
    {
    case 0: data.settings_mono.arp_data.type = ArpType::UP;
    case 1: data.settings_mono.arp_data.type = ArpType::DOWN;
    case 2: data.settings_mono.arp_data.type = ArpType::UPDOWN;
    case 3: data.settings_mono.arp_data.type = ArpType::PICKING_IN;
    }
}

void play_mono(ApplicationData& data)
{
    if (interval_hit(TimeDivision::TIME_DIVISION_SIXTEENTH, data.step, data.ticks))
    {
        uint8_t chord = cv(data.harmony, data.step);
        uint8_t pitch = get_arp_pitch(data.settings_mono.arp_data, data.scale, chord);
        note_on(pitch, 64, MIDI_CHANNEL_MONO, data.settings_mono.storage, 3);
    }
}
