#pragma once

#include <string>

#include "enums.h"

namespace Vleerhond
{
    class Strings
    {
    public:

        static std::string get_string(const ScaleType scale)
        {
            switch (scale)
            {
            case ScaleType::AEOLIAN:
                return "aeolian";
            case ScaleType::DORIAN:
                return "dorian";
            case ScaleType::IONIAN:
                return "ionian";
            default:
                return "unkown scale";
            }
        }

        static std::string get_string(const Root root)
        {
            switch (root)
            {
            case Root::ROOT_C:
                return "C";
            case Root::ROOT_C_SHARP:
                return "C#";
            case Root::ROOT_D:
                return "D";
            case Root::ROOT_D_SHARP:
                return "D#";
            case Root::ROOT_E:
                return "E";
            case Root::ROOT_F:
                return "F";
            case Root::ROOT_F_SHARP:
                return "F#";
            case Root::ROOT_G:
                return "G";
            case Root::ROOT_G_SHARP:
                return "G#";
            case Root::ROOT_A:
                return "A";
            case Root::ROOT_A_SHARP:
                return "A#";
            case Root::ROOT_B:
                return "B";
            default:
                return "unkown root";
            }
        }

        static const std::string get_string(const MonoStyle style)
        {
            switch (style)
            {
            case MonoStyle::MonoPolyRhythm:
                return "poly_rhythm";
            case MonoStyle::MonoSixteenths:
                return "sixteenths";
            default:
                return "invalid style";
            }
        }
    };
}
