#include "instruments/vermona/vermona_fugue.h"

namespace Vleerhond {
VermonaFugue::VermonaFugue(HarmonyStruct& harmony, TimeStruct& time)
    : InstrumentGroup(harmony, time),
      player_1(harmony, time, fugue),
      player_2(harmony, time, fugue),
      player_3(harmony, time, fugue),
      player_4(harmony, time, fugue) {
    instruments.push_back(&player_1);
    instruments.push_back(&player_2);
    instruments.push_back(&player_3);
    instruments.push_back(&player_4);

    player_1.manual_pitch_offset = 0;
    player_2.manual_pitch_offset = 32;
    player_3.manual_pitch_offset = 64;
    player_4.manual_pitch_offset = 96;

    // for (const FuguePlayer& player : { player_1, player_2, player_3, player_4
    // })
    //{
    //}
}
}  // namespace Vleerhond
