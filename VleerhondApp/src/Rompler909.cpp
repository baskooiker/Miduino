#include "Rompler909.h"

#include "rand.h"

void Rompler909::play(const uint8_t pitch, const uint8_t velocity)
{
    switch (pitch)
    {
    case NOTE_TANZBAR_BD1:
        bd1.setVolume(.5 + velocity / 127.);
        bd1.play();
        break;
    case NOTE_TANZBAR_HH:
        hh.setVolume(.5 + velocity / 127.);
        hh.play();
        break;
    case NOTE_TANZBAR_OH:
        oh.setVolume(.5 + velocity / 127.);
        oh.play();
        break;
    case NOTE_TANZBAR_CP:
        cp.setVolume(.5 + velocity / 127.);
        cp.play();
        break;
    case NOTE_TANZBAR_LT:
        lt.setVolume(.5 + velocity / 127.);
        lt.play();
        break;
    case NOTE_TANZBAR_MT:
        mt.setVolume(.5 + velocity / 127.);
        mt.play();
        break;
    case NOTE_TANZBAR_HT:
        ht.setVolume(.5 + velocity / 127.);
        ht.play();
        break;
    default:
        //printf("Note not implemented: %d\n", pitch);
        break;
    }
}

#include<filesystem>
#include <boost/filesystem.hpp>

Rompler909::Rompler909()
{
}

Rompler909::~Rompler909()
{
    bd1.unload();
}

static const std::string bd_names[] = {
    "909\\909_bd_01.wav",
    "909\\909_bd_02.wav",
    "909\\909_bd_03.wav",
    "909\\909_bd_04.wav",
    "909\\909_bd_05.wav",
    "909\\909_bd_06.wav",
    "909\\909_bd_07.wav",
    "909\\909_bd_08.wav",
    "909\\909_bd_09.wav",
    "909\\909_bd_10.wav",
    "909\\909_bd_11.wav",
    "909\\909_bd_12.wav",
    "909\\909_bd_13.wav",
    "909\\909_bd_14.wav",
    "909\\909_bd_15.wav",
    "909\\909_bd_16.wav",
    "909\\909_bd_17.wav",
    "909\\909_bd_18.wav",
    "909\\909_bd_19.wav",
    "909\\909_bd_20.wav",
    "909\\909_bd_21.wav",
    "909\\909_bd_22.wav",
    "909\\909_bd_23.wav",
    "909\\909_bd_24.wav",
    "909\\909_bd_25.wav",
    "909\\909_bd_26.wav",
    "909\\909_bd_27.wav"
};

void Rompler909::load_sounds()
{
    bd1.load(bd_names[Rand::randui8(sizeof(bd_names) / sizeof(*bd_names))]);
    hh.load("909\\909_clsd_hh_01.wav");
    oh.load("909\\909_open_hh_01.wav");
    cp.load("909\\909_clap.wav");
    lt.load("909\\909_lowtom_01.wav");
    mt.load("909\\909_midtom_01.wav");
    ht.load("909\\909_hitom_01.wav");

}