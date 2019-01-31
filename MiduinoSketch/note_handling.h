#pragma once

#include "defs.h"
#include "midi_io.h"

void note_off(uint8_t pitch, PitchStorage& storage);

void add_to_storage(const NoteStruct& note, PitchStorage& storage)
{
    for (uint8_t i = 0; i < storage.size; i++)
    {
        if (storage.data[i].pitch == note.pitch)
        {
            storage.data[i] = note;
            return;
        }
    }
    storage.data[storage.size] = note;
    storage.size++;
}

NoteStruct pop_from_storage(PitchStorage& s)
{
    if (s.size > 0) {
        NoteStruct v = s.data[s.size];
        s.data[s.size--] = { 0, 0, 0 };
        return v;
    }
    return { 0, 0, 0 };
}

NoteStruct pop_from_storage(PitchStorage& s, uint8_t pitch)
{
    for (uint8_t i = 0; i < s.size; i++)
    {
        if (s.data[i].pitch == pitch) {
            NoteStruct v = s.data[i];
            s.data[i] = s.data[s.size - 1];
            s.size--;
            return v;
        }
    }
    return { 0, 0, 0 };
}

void stop_notes(PitchStorage& storage)
{
    if (storage.size == 0) return;
    for (uint8_t i = 0; i < storage.size; i++)
    {
        if (storage.data[i].length != HOLD_NOTE
            && storage.data[i].length > 0)
        {
            storage.data[i].length -= 1;
        }
    }

    for (int i = storage.size - 1; i >= 0; i--)
    {
        if (storage.data[i].length == 0)
        {
            note_off(storage.data[i].pitch, storage);
        }
    }
}

void untie_notes(PitchStorage& storage)
{
    for (int i = storage.size - 1; i >= 0; i--)
    {
        switch (storage.data[i].type)
        {
        case NoteType::Slide:
            storage.data[i].length = 0;
            break;
        case NoteType::Tie:
            note_off(storage.data[i].pitch, storage);
            break;
        }
    }
}

void note_off(uint8_t pitch, PitchStorage& storage)
{
    send_note_off(pitch, storage.channel);
    NoteStruct stored = pop_from_storage(storage, pitch);
}

void note_on(const NoteStruct note, PitchStorage& storage)
{
    untie_notes(storage);
    NoteStruct stored = pop_from_storage(storage, note.pitch);
    if (note.pitch == stored.pitch)
    {
        note_off(note.pitch, storage);
    }
    send_note_on(note.pitch, note.velocity, storage.channel);
    add_to_storage(note, storage);
}

void note_on(const NoteStruct* notes, const uint8_t length, PitchStorage& storage)
{
    untie_notes(storage);
    for (int i = 0; i < length; i++)
    {
        NoteStruct stored = pop_from_storage(storage, notes[i].pitch);
        if (notes[i].pitch == stored.pitch)
        {
            note_off(notes[i].pitch, storage);
        }
        send_note_on(notes[i].pitch, notes[i].velocity, storage.channel);
    }
    for (int i = 0; i < length; i++)
    {
        add_to_storage(notes[i], storage);
    }
}
