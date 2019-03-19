#pragma once

#include "defs.h"
#include "midi_io.h"

#define STORAGE_SIZE 8

class ChannelStruct
{
public:
    uint8_t channel;
    int8_t pitch_offset;

    ChannelStruct()
    {
        this->channel = 0;
        this->pitch_offset = 0;
    }

    ChannelStruct(const uint8_t channel, const int8_t pitch_offset = 0)
    {
        this->channel = channel;
        this->pitch_offset = pitch_offset;
    }
};

#define MAX_NUMBER_OF_CHANNELS 2

class PitchStorage
{
public:
    NoteStruct data[STORAGE_SIZE];
    uint8_t size;

protected:
    NoteEvent events[STORAGE_SIZE];
    uint8_t nr_of_events;

    ChannelStruct channels[MAX_NUMBER_OF_CHANNELS];

public:
    PitchStorage()
    {
        size = 0;
        nr_of_events = 0;
    }

    void _send_note_on(const uint8_t pitch, const uint8_t velocity)
    {
        for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++)
        {
            if (this->channels[i].channel > 0)
            {
                send_note_on(pitch + this->channels[i].pitch_offset, velocity, this->channels[i].channel);
            }
        }
    }

    void _send_note_off(const uint8_t pitch)
    {
        for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++)
        {
            if (this->channels[i].channel > 0)
            {
                send_note_off(pitch + this->channels[i].pitch_offset, this->channels[i].channel);
            }
        }
    }

    void set_channel(const uint8_t channel, const int8_t offset = 0)
    {
        for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++)
        {
            if (this->channels[i].channel == 0)
            {
                this->channels[i] = ChannelStruct(channel, offset);
            }
        }
    }

    void process_events()
    {
        uint32_t time = millis();
        for (int i = this->nr_of_events - 1; i >= 0; i--)
        {
            NoteEvent event_i = this->events[i];
            if (event_i.time <= time)
            {
                this->note_on(event_i.note);
                this->events[i] = this->events[this->nr_of_events - 1];
                this->nr_of_events--;
            }
        }
    }

    void note_off(uint8_t pitch)
    {
        this->_send_note_off(pitch);
        NoteStruct stored = this->pop_from_storage(pitch);
    }

    void note_on(const NoteStruct note)
    {
        // TODO: remove this intermediate function
        this->untie_notes();
        NoteStruct stored = this->pop_from_storage(note.pitch);
        if (note.pitch == stored.pitch)
        {
            this->note_off(note.pitch);
        }
        this->_send_note_on(note.pitch, note.velocity);
        this->add_to_storage(note);
    }

    void note_on(const NoteStruct note, const uint32_t delay)
    {
        // TODO: remove this intermediate function
        if (delay == 0)
            return this->note_on(note);

        if (this->nr_of_events < STORAGE_SIZE)
        {
            NoteEvent new_event = {};
            new_event.note = note;
            new_event.time = millis() + delay;

            this->events[this->nr_of_events++] = new_event;
        }
        else
        {
            this->note_on(note);
        }
    }

    void note_on(const NoteStruct* notes, const uint8_t length)
    {
        this->untie_notes();
        for (int i = 0; i < length; i++)
        {
            NoteStruct stored = this->pop_from_storage(notes[i].pitch);
            if (notes[i].pitch == stored.pitch)
            {
                this->note_off(notes[i].pitch);
            }
            this->_send_note_on(notes[i].pitch, notes[i].velocity);
        }
        for (int i = 0; i < length; i++)
        {
            this->add_to_storage(notes[i]);
        }
    }

    void add_to_storage(const NoteStruct& note)
    {
        for (uint8_t i = 0; i < this->size; i++)
        {
            if (this->data[i].pitch == note.pitch)
            {
                this->data[i] = note;
                return;
            }
        }
        this->data[this->size] = note;
        this->size++;
    }

    NoteStruct pop_from_storage()
    {
        if (this->size > 0) {
            NoteStruct note = this->data[this->size - 1];
            this->size--;
            return note;
        }
        return { 0, 0, 0 };
    }

    NoteStruct pop_from_storage(uint8_t pitch)
    {
        for (uint8_t i = 0; i < this->size; i++)
        {
            if (this->data[i].pitch == pitch) {
                NoteStruct note = this->data[i];
                this->data[i] = this->data[this->size - 1];
                this->size--;
                return note;
            }
        }
        return { 0, 0, 0 };
    }

    void stop_notes()
    {
        if (this->size == 0) return;
        for (uint8_t i = 0; i < this->size; i++)
        {
            if (this->data[i].length != 0xFF
                && this->data[i].length > 0)
            {
                this->data[i].length -= 1;
            }
        }

        for (int i = this->size - 1; i >= 0; i--)
        {
            if (this->data[i].length == 0)
            {
                this->note_off(this->data[i].pitch);
            }
        }
    }

    void untie_notes()
    {
        for (int i = this->size - 1; i >= 0; i--)
        {
            switch (this->data[i].type)
            {
            case NoteType::Slide:
                this->data[i].length = 0;
                break;
            case NoteType::Tie:
                this->note_off(this->data[i].pitch);
                break;
            }
        }
    }

    void all_notes_off()
    {
        NoteStruct p = { 0, 0, 0 };
        do {
            p = this->pop_from_storage();
            if (p.pitch > 0)
            {
                this->note_off(p.pitch);
            }
        } while (p.pitch != 0);
    }

};
