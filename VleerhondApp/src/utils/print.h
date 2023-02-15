#pragma once

#include <stdio.h>

#include "core/defs.h"
#include "patterns/cv_patterns.h"
#include "patterns/gate_patterns.h"
#include "patterns/interval_pattern.h"

namespace Vleerhond {
void printPattern(const BinaryPattern& pattern, const uint8_t length) {
    for (int i = 0; i < length; i++) {
        printf("%-3d ", pattern.gate(i));
    }
    printf("\n");
}

void printPattern(const GatePatternAB& pattern) {
    for (int i = 0; i < 4; i++) {
        printPattern(pattern.patterns[pattern.abPattern.value(i)], 16);
    }
    printf("\n");
}

void printPattern(const CvPattern& pattern) {
    for (int i = 0; i < 16; i++) {
        printf("%-3d ", pattern.value(i));
    }
    printf("\n");
}

void printPattern(const CvPattern16& pattern) {
    printPattern(pattern.pattern);
    printf("\n");
}

void printPattern(const CvPatternAB& pattern) {
    for (int i = 0; i < 4; i++) {
        printPattern(pattern.patterns[pattern.abPattern.value(i)]);
    }
    printf("\n");
}

void printPattern(const IntervalPattern& pattern) {
    for (int i = 0; i < pattern.length; i++) {
        printf("%-2d ", (uint8_t)pattern.pattern[i]);
    }
    printf("\n");
}

void printScale(const Scale& scale) {
    std::vector<uint8_t> notes = scale.getScaleNotes();
    printf("Root = %d\n", scale.root);
    for (const int note : notes) {
        printf("%-2d  ", note);
    }
    printf("\n");
}
}  // namespace Vleerhond