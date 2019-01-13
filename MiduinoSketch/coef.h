#pragma once

#include "rhythms.h"
#include "defs.h"
#include "gate.h"
#include "rand.h"

void set_coef_pattern(BinaryPattern& pattern, const Coefficients coef)
{
    set_gate(pattern, 0, randf() < coef.one);
    set_gate(pattern, 4, randf() < coef.two);
    set_gate(pattern, 8, randf() < coef.three);
    set_gate(pattern, 12, randf() < coef.four);
    for (int i = 2; i < 16; i+=4)
        set_gate(pattern, i, randf() < coef.eights);
    for (int i = 1; i < 16; i += 4)
        set_gate(pattern, i, randf() < coef.down);
    for (int i = 3; i < 16; i += 4)
        set_gate(pattern, i, randf() < coef.up);
}

void set_coef_pattern(GatePatternAB& pattern, const Coefficients coef)
{
    for (int i = 0; i < 3; i++)
        set_coef_pattern(pattern.patterns[i], coef);
    set_ab_pattern(pattern.abPattern);
}

void set_coef_kick_pattern(GatePatternAB& pattern)
{
    Coefficients coef = { 0 };
    coef.one = 1.f;
    coef.two = 1.f;
    coef.three = 1.f;
    coef.four = 1.f;
    coef.eights = randf(.25);
    coef.up = randf(.125);
    coef.down = randf(.125);
    set_coef_pattern(pattern, coef);
}

void set_coef_snare_pattern(GatePatternAB& pattern)
{
    Coefficients coef = { 0 };
    coef.two = randf(.5f, .75f);
    coef.four = randf(.5f, .75f);
    coef.eights = randf(.125);
    coef.up = randf(.125);
    coef.down = randf(.125);
    set_coef_pattern(pattern, coef);
}

void set_coef_hat_pattern(GatePatternAB& pattern)
{
    Coefficients coef = { 0 };
    coef.eights = 1.f;
    coef.up = randf(.25);
    coef.down = randf(.25);
    set_coef_pattern(pattern, coef);
}
