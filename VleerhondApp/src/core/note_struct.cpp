#include "core/note_struct.h"

namespace Vleerhond {
NoteStruct::NoteStruct() {
    pitch = 0;
    velocity = 0;
    length = 0;
    type = NoteType::Normal;
}

NoteStruct::NoteStruct(
    const uint8_t pitch, const uint8_t velocity, const uint8_t length,
    const NoteType type) {
    this->pitch = pitch;
    this->velocity = velocity;
    this->length = length;
    this->type = type;
}
NoteEvent::NoteEvent() { time = 0; }
NoteEvent::NoteEvent(const std::vector<NoteStruct>& notes, const uint32_t time)
    : notes(notes), time(time) {}
}  // namespace Vleerhond