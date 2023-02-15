#include "instruments/neutron/neutron_mono.h"

#include "utils/rand.h"

namespace Vleerhond {
NeutronMono::NeutronMono(
    HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
    : Mono(harmony, modulators, time) {
    settings.min_velocity = 16;
    settings.min_velocity = 127;
}
void NeutronMono::sendSysexMessage(const std::vector<uint8_t> msg) {
    std::vector<uint8_t> bytes = sysex_header;
    for (const uint8_t& v : msg) {
        bytes.push_back(v);
    }
    bytes.push_back(0xF7);

    getChannel()->sendBytes(bytes);
}

/* 0-24 is autoglide range -12/12. 12 is no autoglide
 */

void NeutronMono::setAutoglide(const uint8_t value) {
    sendSysexMessage({0x0A, 0x24, value});
    sendSysexMessage({0x0A, 0x25, value});
}
void NeutronMono::setShapeBlend(const bool value) {
    const uint8_t hex_value = value ? 0x00 : 0x01;
    sendSysexMessage({0x0A, 0x20, hex_value});
    sendSysexMessage({0x0A, 0x21, hex_value});
}

void NeutronMono::setVcfModSource(const uint8_t value) {
    sendSysexMessage({0x0A, 0x12, value});
}

void NeutronMono::setVcfModDepth(const uint8_t value) {
    sendSysexMessage({0x0A, 0x14, value});
}

void NeutronMono::randomize() {
    setAutoglide(24);
    setShapeBlend(false);
    setVcfModSource(3);
    setVcfModDepth(32);

    Mono::randomize();
    settings.min_velocity = Rand::randui8(32, 64);
    settings.max_velocity = Rand::randui8(100, 127);
}

void NeutronMono::autoRandomize() {
    // This method is called every 4 bars. Should randomize every time.
    this->randomize();
}
}  // namespace Vleerhond
