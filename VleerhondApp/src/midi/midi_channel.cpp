#pragma once

#include "ofLog.h"

#include "defs.h"
#include "midi_channel.h"
#include "note_struct.h"

#define STORAGE_SIZE 8

namespace Vleerhond
{
    ChannelStruct::ChannelStruct()
    {
        this->channel = 0;
        this->pitch_offset = 0;
    }

    ChannelStruct::ChannelStruct(const uint8_t channel, const int8_t pitch_offset)
    {
        this->channel = channel;
        this->pitch_offset = pitch_offset;
    }

    void MidiChannel::_send_note_on(const uint8_t pitch, const uint8_t velocity)
    {
        if (this->channel.channel > 0)
        {
            MidiIO::send_note_on(pitch + this->channel.pitch_offset, velocity, this->channel.channel);
        }
    }

    void MidiChannel::_send_note_off(const uint8_t pitch)
    {
        if (this->channel.channel > 0)
        {
            MidiIO::send_note_off(pitch + this->channel.pitch_offset, this->channel.channel);
        }
    }

    MidiChannel::MidiChannel()
    {
        size = 0;
        nr_of_events = 0;
    }

    void MidiChannel::set_channel(const uint8_t midi_channel, const int8_t offset)
    {
        if (channel.channel == 0)
        {
            channel = ChannelStruct(midi_channel, offset);
            return;
        }
        else
        {
            ofLogWarning("MidiChannel", "Midi channel was already set!");
        }
    }

    void MidiChannel::process_events()
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

    void MidiChannel::note_off(uint8_t pitch)
    {
        this->_send_note_off(pitch);
        NoteStruct stored = this->pop_from_storage(pitch);
    }

    void MidiChannel::note_on(const NoteStruct& note)
    {
        // TODO: remove this intermediate function
        this->untie_notes();
        NoteStruct stored = this->pop_from_storage(note.pitch);

        if (note.pitch == stored.pitch)
        {
            this->note_off(note.pitch);
        }
        if (note.velocity > 0)
        {
            this->_send_note_on(note.pitch, note.velocity);
            this->add_to_storage(note);
        }
    }

    void MidiChannel::note_on(const NoteStruct& note, const uint32_t delay)
    {
        if (this->nr_of_events < STORAGE_SIZE && delay > 0)
        {
            NoteEvent new_event(note, millis() + delay);
            this->events[this->nr_of_events++] = new_event;
        }
        else
        {
            this->note_on(note);
        }
    }

    void MidiChannel::note_on(const NoteStruct* notes, const uint8_t length)
    {
        if (!pedal)
        {
            this->untie_notes();
        }
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

    void MidiChannel::add_to_storage(const NoteStruct& note)
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

    NoteStruct MidiChannel::pop_from_storage(uint8_t pitch)
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
        return NoteStruct();
    }

    void MidiChannel::process_active_notes()
    {
        if (this->size == 0) return;
        for (uint8_t i = 0; i < this->size; i++)
        {
            if (this->data[i].length > 0)
            {
                this->data[i].length -= 1;
            }
        }

        for (int i = (int)this->size - 1; i >= 0; i--)
        {
            if (this->data[i].length == 0)
            {
                this->note_off(this->data[i].pitch);
            }
        }
    }

    void MidiChannel::untie_notes()
    {
        for (int i = (int)this->size - 1; i >= 0; i--)
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

    void MidiChannel::all_notes_off()
    {
        for (int i = (int)this->size - 1; i >= 0; i--)
        {
            note_off(this->data[i].pitch);
        }
    }

    ChannelStruct& MidiChannel::get_channel()
    {
        return channel;
    }

    void MidiChannel::print_storage()
    {
        printf("\nStorage size: %d\n", this->size);
        for (int i = 0; i < this->size; i++)
        {
            printf("%2d, %3d, %d\n",
                this->data[i].pitch,
                this->data[i].length,
                this->data[i].type
            );
        }
    }

    void MidiChannel::set_pedal(const bool value)
    {
        pedal = value;
    }
}