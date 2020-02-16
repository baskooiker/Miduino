#include "instruments/neutron/neutron_mono.h"

namespace Vleerhond
{
    NeutronMono::NeutronMono(HarmonyStruct & harmony, Modulators & modulators, TimeStruct & time)
        : Mono(harmony, modulators, time, MIDI_CHANNEL_NEUTRON)
    {
        settings.min_velocity = 16;
        settings.min_velocity = 127;
    }
    inline void NeutronMono::sendSysexMessage(const std::vector<uint8_t> msg)
    {
        std::vector<uint8_t> bytes = sysex_header;
        for (const uint8_t& v : msg)
        {
            bytes.push_back(v);
        }
        bytes.push_back(0xF7);

        MidiIO::sendBytes(bytes);
    }

    /* 0-24 is autoglide range -12/12. 12 is no autoglide
    */

    inline void NeutronMono::setAutoglide(const uint8_t value)
    {
        sendSysexMessage({ 0x0A, 0x24, value });
        sendSysexMessage({ 0x0A, 0x25, value });
    }
    inline void NeutronMono::setShapeBlend(const bool value)
    {
        const uint8_t hex_value = value ? 0x00 : 0x01;
        sendSysexMessage({ 0x0A, 0x20, hex_value });
        sendSysexMessage({ 0x0A, 0x21, hex_value });
    }

    void NeutronMono::setVcfModSource(const uint8_t value)
    {
        sendSysexMessage({ 0x0A, 0x12, value });
    }

    void NeutronMono::setVcfModDepth(const uint8_t value)
    {
        sendSysexMessage({ 0x0A, 0x14, value });
    }

    inline void NeutronMono::randomize()
    {
        setAutoglide(24);
        setShapeBlend(true);
        setVcfModSource(3);
        setVcfModDepth(63);

        Mono::randomize();
        settings.min_velocity = Rand::randui8(32, 64);
        settings.max_velocity = Rand::randui8(100, 127);
    }
}
