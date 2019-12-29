#pragma once

#include "mono.h"
#include "patterns/modulators.h"

namespace Vleerhond
{
class NeutronMono : public Mono
{
private:
    const std::vector<uint8_t> sysex_header = { 0xF0, 0x00, 0x20, 0x32, 0x28, 0x7F };
public:
    NeutronMono(
        HarmonyStruct& harmony,
        Modulators& modulators,
        TimeStruct& time,
        const uint8_t midi_channel) :
        Mono(harmony, modulators, time, MIDI_CHANNEL_NEUTRON)
    {
        settings.p_coef = 0;
    }

    void sendSysexMessage(const std::vector<uint8_t> msg)
    {
        std::vector<uint8_t> bytes = sysex_header;
        for (const uint8_t& v: msg)
        {
            bytes.push_back(v);
        }
        bytes.push_back(0xF7);

        MidiIO::sendBytes(bytes);
    }

    /* 0-24 is autoglide range -12/12. 12 is no autoglide
    */
    virtual void setAutoglide(const uint8_t value)
    {
        sendSysexMessage({ 0x0A, 0x24, value });
        sendSysexMessage({ 0x0A, 0x25, value });
    }

    virtual void setShapeBlend(const bool value)
    {
        const uint8_t hex_value = value ? 0x00 : 0x01;
        sendSysexMessage({ 0x0A, 0x20, hex_value });
        sendSysexMessage({ 0x0A, 0x21, hex_value });
    }

    virtual void randomize()
    {
        setAutoglide(24);
        setShapeBlend(true);
        Mono::randomize();
    }

};
}