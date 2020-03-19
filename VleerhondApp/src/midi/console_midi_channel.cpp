#include "midi/console_midi_channel.h"

#include <iostream>
#include <sstream>

namespace Vleerhond
{
    ConsoleMidiChannel::ConsoleMidiChannel(const std::string & port_name)
        : MidiChannel(-2, port_name)
    {
    }
    std::string ConsoleMidiChannel::getStorageString()
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
    void ConsoleMidiChannel::_sendNoteOn(const uint8_t pitch, const uint8_t velocity)
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
    void ConsoleMidiChannel::_sendNoteOff(const uint8_t pitch)
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
    void ConsoleMidiChannel::print()
    {
        for (auto& note : notes)
        {
            std::cout << (int)note.first << ": " << note.second << std::endl;
        }
        std::cout << std::endl;
    }
    int ConsoleMidiChannel::activeNoteCount()
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
}
