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
    void MidiChannel::_sendNoteOn(const uint8_t pitch, const uint8_t velocity)
    {
        if (this->channel > 0)
        {
            MidiIO::sendNoteOn(pitch, velocity, this->channel, this->port_name);
        }
    }

    void MidiChannel::_sendNoteOff(const uint8_t pitch)
    {
        if (this->channel > 0)
        {
            MidiIO::sendNoteOff(pitch, this->channel, port_name);
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
                for (const auto& note : event_i.notes)
                {
                    this->_noteOn({ note });
                }
                this->events[i] = this->events[this->nr_of_events - 1];
                this->nr_of_events--;
            }
        }
    }

    void MidiChannel::noteOff(uint8_t pitch)
    {
        this->_sendNoteOff(pitch);
        NoteStruct stored = this->popFromStorage(pitch);
    }

    void MidiChannel::_noteOn(const std::vector<NoteStruct>& notes)
    {
        if (!this->getPedal())
        {
            this->untieNotes();
        }

        for (const auto& note : notes)
        {
            // If the played note was still active, stop it before replaying
            NoteStruct stored = this->popFromStorage(note.pitch);
            if (note.pitch == stored.pitch)
            {
                this->noteOff(note.pitch);
            }

            if (note.velocity > 0)
            {
                this->_sendNoteOn(note.pitch, note.velocity);
                this->addToStorage(note);
            }
        }
    }

    void MidiChannel::noteOn(const NoteStruct& note, const uint32_t delay)
    {
        std::vector<NoteStruct> notes = {note};
        noteOn(notes, delay);
    }

    void MidiChannel::noteOn(const std::vector<NoteStruct>& notes, const uint8_t delay)
    {
        if (this->nr_of_events < STORAGE_SIZE && delay > 0)
        {
            NoteEvent new_event(notes, Utils::millis() + delay);
            this->events[this->nr_of_events++] = new_event;
        }
        else
        {
            this->_noteOn(notes);
        }
    }

    void MidiChannel::addToStorage(const NoteStruct& note)
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

    NoteStruct MidiChannel::popFromStorage(uint8_t pitch)
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

    void MidiChannel::processActiveNotes()
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
                    this->noteOff(this->data[i].pitch);
                }
            }
        }
    }

    void MidiChannel::untieNotes()
    {
        for (int i = (int)this->size - 1; i >= 0; i--)
        {
            if (!this->pedal)
            {
                this->noteOff(this->data[i].pitch);
            }
        }
    }

    void MidiChannel::allNotesOff()
    {
        for (int i = (int)this->size - 1; i >= 0; i--)
        {
            noteOff(this->data[i].pitch);
        }
    }

    void MidiChannel::printStorage()
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

    void MidiChannel::setPedal(const bool value)
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
        MidiIO::sendCc(cc, value, send_channel, this->port_name);
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