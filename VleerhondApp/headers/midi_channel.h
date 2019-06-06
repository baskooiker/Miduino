#pragma once

#include "defs.h"
#include "midi_io.h"
#include "note_struct.h"

#define STORAGE_SIZE 8

namespace Vleerhond
{
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

    class MidiChannel
    {
    protected:
        NoteStruct data[STORAGE_SIZE];
        uint8_t size;

        NoteEvent events[STORAGE_SIZE];
        uint8_t nr_of_events;

        ChannelStruct channels[MAX_NUMBER_OF_CHANNELS];

        void _send_note_on(const uint8_t pitch, const uint8_t velocity)
        {
            for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++)
            {
                if (this->channels[i].channel > 0)
                {
                    if (this->channels[i].channel == MIDI_CHANNEL_ROCKET)
                    {
                        //printf("Rocket note on : %3d\n", pitch);
                    }
                    MidiIO::send_note_on(pitch + this->channels[i].pitch_offset, velocity, this->channels[i].channel);
                }
            }
        }

        void _send_note_off(const uint8_t pitch)
        {
            for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++)
            {
                if (this->channels[i].channel > 0)
                {
                    if (this->channels[i].channel == MIDI_CHANNEL_ROCKET)
                    {
                        //printf("Rocket note off: %3d\n", pitch);
                    }
                    MidiIO::send_note_off(pitch + this->channels[i].pitch_offset, this->channels[i].channel);
                }
            }
        }

    public:
        MidiChannel()
        {
            size = 0;
            nr_of_events = 0;
        }

        void set_channel(const uint8_t channel, const int8_t offset = 0)
        {
            for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++)
            {
                if (this->channels[i].channel == 0)
                {
                    this->channels[i] = ChannelStruct(channel, offset);
                    return;
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

        void note_on(const NoteStruct& note)
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

        void note_on(const NoteStruct& note, const uint32_t delay)
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
            return NoteStruct();
        }

        void process_active_notes()
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

        void untie_notes()
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

        void all_notes_off()
        {
            for (int i = (int)this->size - 1; i >= 0; i--)
            {
                note_off(this->data[i].pitch);
            }
        }

        void get_channels(ChannelStruct* channels, uint8_t& length)
        {
            length = 0;
            for (int i = 0; i < MAX_NUMBER_OF_CHANNELS; i++)
            {
                if (this->channels[i].channel > 0)
                {
                    channels[length++] = this->channels[i];
                }
            }
        }

        void print_storage()
        {
#ifndef ARDUINO
            printf("\nStorage size: %d\n", this->size);
            for (int i = 0; i < this->size; i++)
            {
                printf("%2d, %3d, %d\n",
                    this->data[i].pitch,
                    this->data[i].length,
                    this->data[i].type);
            }
#endif
        }

    };
}