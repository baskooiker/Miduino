#pragma once

#include <stdio.h>

namespace Vleerhond
{
    void print_pattern(const BinaryPattern& pattern, const uint8_t length)
    {
        for (int i = 0; i < length; i++)
        {
            printf("%-3d ", pattern.gate(i));
        }
        printf("\n");
    }

    void print_pattern(const GatePatternAB& pattern)
    {
        for (int i = 0; i < 4; i++)
        {
            print_pattern(pattern.patterns[pattern.abPattern.value(i)], 16);
        }
        printf("\n");
    }

    void print_pattern(const CvPattern& pattern)
    {
        for (int i = 0; i < 16; i++)
        {
            printf("%-3d ", pattern.value(i));
        }
        printf("\n");
    }

    void print_pattern(const CvPattern16& pattern)
    {
        print_pattern(pattern.pattern);
        printf("\n");
    }

    void print_pattern(const CvPatternAB& pattern)
    {
        for (int i = 0; i < 4; i++)
        {
            print_pattern(pattern.patterns[pattern.abPattern.value(i)]);
        }
        printf("\n");
    }

    void print_pattern(const IntervalPattern& pattern)
    {
        for (int i = 0; i < pattern.length; i++)
        {
            printf("%-2d ", (uint8_t)pattern.pattern[i]);
        }
        printf("\n");
    }

    void print_scale(const Scale& scale)
    {
        uint8_t length = 0;
        uint8_t* notes = scale.get_scale_notes(length);
        printf("Root = %d\n", scale.root);
        for (int i = 0; i < length; i++)
        {
            printf("%-2d  ", notes[i]);
        }
        printf("\n");
    }
}