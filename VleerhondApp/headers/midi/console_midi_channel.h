#pragma once

#include <set>

#include "midi/midi_channel.h"


// MOVE TO CPP
#include <iostream>
#include <sstream>

namespace Vleerhond
{
    class ConsoleMidiChannel : public MidiChannel
    {
    private:
        std::map<uint8_t, int> notes;

    public:
        ConsoleMidiChannel(const std::string& port_name)
            : MidiChannel(16, port_name)
        {
        }

        virtual std::string getStorageString()
        {
            std::stringstream ss;
            ss << "[";
            for (int i = 0; i < size; i++)
            {
                ss << "(" << (int)data[i].pitch << ", " << (int)data[i].length << ")";
            }
            ss << "]";
            return ss.str();
        }

        virtual void _send_note_on(const uint8_t pitch, const uint8_t velocity)
        {
            if (notes.find(pitch) == notes.end())
            {
                notes[pitch] = 1;
            }
            else
            {
                notes[pitch] = notes[pitch] + 1;
            }
            std::cout << "sendNoteOn(" << (int)pitch;
            std::cout << ", " << (int)velocity;
            std::cout << ") - count = " << activeNoteCount(); 
            std::cout << " - ";
            std::cout << getStorageString();
            std::cout << std::endl;
        }

        virtual void _send_note_off(const uint8_t pitch)
        {
            if (notes.find(pitch) == notes.end())
            {
                notes[pitch] = -1;
            }
            else
            {
                notes[pitch] = notes[pitch] - 1;
            }
            std::cout << "sendNoteOff(" << (int)pitch << ") - count = " << activeNoteCount();
            std::cout << " - ";
            std::cout << getStorageString();
            std::cout << std::endl;
        }

        virtual void print()
        {
            for (auto& note: notes)
            { 
                std::cout << (int)note.first << ": " << note.second << std::endl;
            }
            std::cout << std::endl;
        }

        int activeNoteCount()
        {
            int count = 0;
            for (auto& note : notes)
            {
                if (note.second > 0)
                {
                    count += 1;
                }
            }
            return count;
        }

    };
}