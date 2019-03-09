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

NoteStruct pop_from_storage(PitchStorage& storage)
{
    if (storage.size > 0) {
        NoteStruct note = storage.data[storage.size-1];
        storage.size--;
        return note;
    }
    return { 0, 0, 0 };
}

NoteStruct pop_from_storage(PitchStorage& storage, uint8_t pitch)
{
    for (uint8_t i = 0; i < storage.size; i++)
    {
        if (storage.data[i].pitch == pitch) {
            NoteStruct note = storage.data[i];
            storage.data[i] = storage.data[storage.size - 1];
            storage.size--;
            return note;
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

void note_on(const NoteStruct note, PitchStorage& storage, const uint32_t delay)
{
    if (delay == 0)
        return note_on(note, storage);

    if (storage.nr_of_events < STORAGE_SIZE)
    {
        NoteEvent new_event = {};
        new_event.note = note;
        new_event.time = millis() + delay;

        storage.events[storage.nr_of_events++] = new_event;
    }
    else
    {
        note_on(note, storage);
    }
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
