#pragma once

#include "ofLog.h"

#include "core/defs.h"
#include "core/note_struct.h"
#include "midi/midi_channel.h"
#include "midi/midi_io.h"
#include "utils/utils.h"

#define STORAGE_SIZE 16

namespace Vleerhond
{
    void MidiChannel::_send_note_on(const uint8_t pitch, const uint8_t velocity)
    {
        if (this->channel > 0)
        {
            MidiIO::send_note_on(pitch, velocity, this->channel, this->port_name);
        }
    }

    void MidiChannel::_send_note_off(const uint8_t pitch)
    {
        if (this->channel > 0)
        {
            MidiIO::send_note_off(pitch, this->channel, port_name);
        }
    }

    MidiChannel::MidiChannel(const uint8_t midi_channel, const std::string& port_name)
        : channel(midi_channel)
        , port_name(port_name)
    {
    }

    MidiChannel::MidiChannel(const uint8_t channel, const uint8_t cc_channel, const std::string & port_name)
        : channel(channel)
        , cc_channel(cc_channel)
        , port_name(port_name)
    {
    }

    void MidiChannel::processNoteEvents()
    {
        uint32_t time = Utils::millis();
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
        if (!this->getPedal())
        {
            this->untie_notes();
        }

        // If the played note was still active, stop it before replaying
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
            NoteEvent new_event(note, Utils::millis() + delay);
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
        for (uint8_t i = 0; i < (int)this->size; i++)
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
                if (!this->pedal)
                {
                    this->note_off(this->data[i].pitch);
                }
            }
        }
    }

    void MidiChannel::untie_notes()
    {
        for (int i = (int)this->size - 1; i >= 0; i--)
        {
            if (!this->pedal)
            {
                this->note_off(this->data[i].pitch);
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

    void MidiChannel::print_storage()
    {
        printf("\nStorage size: %d\n", (int)this->size);
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
    bool MidiChannel::getPedal()
    {
        return this->pedal;
    }

    // TODO: Remove when obsolete

    std::string MidiChannel::getPortName()
    {
        return this->port_name;
    }
    void MidiChannel::sendCC(const uint8_t cc, const uint8_t value)
    {
        uint8_t send_channel = this->channel;
        if (this->cc_channel >= 0 && this->cc_channel < 16)
        {
            send_channel = cc_channel;
        }
        MidiIO::send_cc(cc, value, send_channel, this->port_name);
    }
    void MidiChannel::sendBytes(std::vector<uint8_t>& bytes)
    {
        MidiIO::sendBytes(bytes, this->port_name);
    }
    void MidiChannel::sendProgramChange(const uint8_t program_change)
    {
        uint8_t send_channel = this->channel;
        if (this->cc_channel >= 0 && this->cc_channel < 16)
        {
            send_channel = cc_channel;
        }
        MidiIO::sendProgramChange(send_channel, program_change, this->port_name);
    }
}