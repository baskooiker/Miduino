#pragma once

#include "instruments/instrument_group.h"
#include "instruments/tonal_instruments/fugue_player.h"

namespace Vleerhond {
class VermonaFugue : public InstrumentGroup {
   public:
    Fugue fugue;
    FuguePlayer player_1;
    FuguePlayer player_2;
    FuguePlayer player_3;
    FuguePlayer player_4;

    VermonaFugue(HarmonyStruct& harmony, TimeStruct& time);

    std::shared_ptr<MidiChannel> getChannel() {
        // Uggly assumption that all instruments use the same channel
        return player_1.getChannel();
    }

    void setChannel(std::shared_ptr<MidiChannel> channel) {
        // std::cout << "VermonaFugue::setChannel\n";
        // Uggly assumption that all instruments use the same channel
        player_1.setChannel(channel);
        player_2.setChannel(channel);
        player_3.setChannel(channel);
        player_4.setChannel(channel);
    }

    void randomize() {
        fugue.randomizeFugue();
        player_1.randomize();
        player_2.randomize();
        player_3.randomize();
        player_4.randomize();
    }
};
}  // namespace Vleerhond
