#pragma once

#include "ofSoundPlayer.h"

#include "defs.h"

class Rompler909
{
public:
    ofSoundPlayer bd1;
    ofSoundPlayer oh;
    ofSoundPlayer hh;
    ofSoundPlayer cp;
    ofSoundPlayer lt;
    ofSoundPlayer mt;
    ofSoundPlayer ht;

    void play(const uint8_t pitch, const uint8_t velocity);

    Rompler909();
    ~Rompler909();

    void load_sounds();
};

