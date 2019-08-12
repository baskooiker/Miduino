#pragma once

#include <algorithm>

#include "defs.hpp"
#include "harmony_struct.hpp"
#include "modulators.hpp"
#include "parameters.hpp"
#include "tanzbar_lo.hpp"

#include "tanzbar_sd.hpp"
#include "tanzbar_cp.hpp"

#include "tanzbar_tom.hpp"
#include "tanzbar_cb.hpp"
#include "tanzbar_cl.hpp"

#include "tanzbar_ma.hpp"
#include "tanzbar_hats.hpp"
#include "tanzbar_cy.hpp"

#include "mfb_503.hpp"

#include "rocket_bass.hpp"
#include "acid_bass.hpp"
#include "bass_dub_settings.hpp"

#include "vermona_mono.hpp"
#include "vermona_mono_dub.hpp"
#include "poly.hpp"
#include "lead.hpp"
#include "drone.hpp"
#include "fugue_player.hpp"

namespace Vleerhond
{
    class ApplicationData
    {
    public:
        TimeStruct time;
        HarmonyStruct harmony;

        Fugue fugue;
        Modulators modulators;

        TanzbarLo tanzbar_lo;

        TanzbarMid tanzbar_mid;
        TanzbarCp tanzbar_cp;

        TanzbarTom tanzbar_tom;
        TanzbarCb tanzbar_cb;
        TanzbarCl tanzbar_cl;

        TanzbarHats tanzbar_hats;
        TanzbarCy tanzbar_cy;
        TanzbarMa tanzbar_ma;

        Mfb503Kick mfb_503_kick;
        Mfb503Snare mfb_503_snare;
        Mfb503Toms mfb_503_toms;
        Mfb503Hats mfb_503_hats;
        Mfb503Cymbal mfb_503_cymbal;

        RocketBass rocket_bass;
        AcidBass acid_bass;
        VermonaMono mono;
        VermonaMonoDub mono_dub;
        Drone drone;

        PolySettings poly_settings;
        LeadSettings lead_settings;

        FuguePlayer fugue_vermona_1;
        FuguePlayer fugue_vermona_2;
        FuguePlayer fugue_vermona_3;
        FuguePlayer fugue_vermona_4;

        UiState ui_state;

        ApplicationData() :
            tanzbar_lo(modulators, time),

            tanzbar_mid(modulators, time),
            tanzbar_cp(modulators, time),

            tanzbar_tom(modulators, time),
            tanzbar_cb(modulators, time),
            tanzbar_cl(modulators, time),

            tanzbar_ma(modulators, time),
            tanzbar_cy(modulators, time),
            tanzbar_hats(modulators, time),

            mfb_503_kick(modulators, harmony, time),
            mfb_503_hats(modulators, time),
            mfb_503_snare(modulators, time),
            mfb_503_toms(modulators, harmony, time),
            mfb_503_cymbal(modulators, time),

            rocket_bass(modulators, harmony, time),
            acid_bass(harmony, time),
            mono(harmony, modulators, time),
            mono_dub(mono, harmony, time, modulators),
            poly_settings(harmony, time),
            lead_settings(harmony, time),
            drone(harmony, time),
            fugue_vermona_1(harmony, time, fugue),
            fugue_vermona_2(harmony, time, fugue),
            fugue_vermona_3(harmony, time, fugue),
            fugue_vermona_4(harmony, time, fugue)
        {
            //mono_dub.midi_channel.set_channel(MIDI_CHANNEL_MONO_2);
            drone.midi_channel.set_channel(MIDI_CHANNEL_BASS_DUB);

            this->poly_settings.midi_channel.set_channel(MIDI_CHANNEL_ROCKET);

            fugue_vermona_1.midi_channel.set_channel(MIDI_CHANNEL_BASS);
            fugue_vermona_2.midi_channel.set_channel(MIDI_CHANNEL_BASS_DUB);
            fugue_vermona_3.midi_channel.set_channel(MIDI_CHANNEL_MONO);
            fugue_vermona_4.midi_channel.set_channel(MIDI_CHANNEL_MONO_2);

            lead_settings.set_active(false);
            poly_settings.set_active(false);
            mfb_503_kick.set_active(false);
            mfb_503_snare.set_active(false);
            mfb_503_toms.set_active(false);
            mfb_503_cymbal.set_active(false);
            mfb_503_hats.set_active(false);

            this->randomize_all();
            set_regular();
        }

        void probability_randomize()
        {
            if (!time.add(TICKS_PER_STEP / 2).interval_hit(TimeDivision::Four))
            {
                return;
            }

            std::vector<InstrumentBase*> ptrs = get_randomizable_instruments();
            InstrumentBase* latest_randomize_ptr = ptrs.at(0);
            for (auto instrument : ptrs)
            {
                if (instrument->randomized_time() < latest_randomize_ptr->randomized_time())
                {
                    latest_randomize_ptr = instrument;
                }
            }

            uint64_t diff = millis() - latest_randomize_ptr->randomized_time();
            if (diff > 60000)
            {
                if (Rand::distribution(diff / 1000, 240) == 0)
                {
                    latest_randomize_ptr->randomize();
                }
            }

        }

        void play_all()
        {
            for (auto instrument : get_active_instrument())
            {
                instrument->play();
            }

            probability_randomize();
        }

        void process_active_notes()
        {
            for (auto instrument : get_instrument_ptrs())
            {
                instrument->midi_channel.process_active_notes();
            }
        }

        void randomize_all()
        {
            auto inst_ptrs = get_instrument_ptrs();
            std::random_shuffle(inst_ptrs.begin(), inst_ptrs.end());
            for (auto& value : inst_ptrs)
            {
                value->randomize();
            }

            this->harmony.randomize();
            this->fugue.randomize_fugue();
        }

        void process_events()
        {
            for (auto instrument : get_instrument_ptrs())
            {
                instrument->process_events();
            }
        }

        std::vector<InstrumentBase*> get_instrument_ptrs()
        {
            std::vector<InstrumentBase*> ptrs;

            ptrs.push_back(&this->tanzbar_lo);
            ptrs.push_back(&this->tanzbar_mid);
            ptrs.push_back(&this->tanzbar_cp);
            ptrs.push_back(&this->tanzbar_tom);
            ptrs.push_back(&this->tanzbar_cb);
            ptrs.push_back(&this->tanzbar_cl);

            ptrs.push_back(&this->tanzbar_ma);
            ptrs.push_back(&this->tanzbar_hats);
            ptrs.push_back(&this->tanzbar_cy);

            //ptrs.push_back(&this->mfb_503_kick);
            //ptrs.push_back(&this->mfb_503_toms);
            //ptrs.push_back(&this->mfb_503_hats);
            //ptrs.push_back(&this->mfb_503_snare);
            //ptrs.push_back(&this->mfb_503_cymbal);

            ptrs.push_back(&this->rocket_bass);
            ptrs.push_back(&this->acid_bass);

            ptrs.push_back(&this->mono);
            ptrs.push_back(&this->mono_dub);

            //ptrs.push_back(&this->poly_settings);
            //ptrs.push_back(&this->lead_settings);

            ptrs.push_back(&this->drone);

            ptrs.push_back(&this->fugue_vermona_1);
            ptrs.push_back(&this->fugue_vermona_2);
            ptrs.push_back(&this->fugue_vermona_3);
            ptrs.push_back(&this->fugue_vermona_4);

            return ptrs;
        }

        std::vector<InstrumentBase*> get_active_instrument()
        {
            std::vector<InstrumentBase*> instruments = get_instrument_ptrs();
            std::vector<InstrumentBase*> ptrs;
            std::copy_if(instruments.begin(), instruments.end(), std::back_inserter(ptrs), 
                [](const InstrumentBase* i) {return i->is_active(); });
            return ptrs;
        }

        std::vector<InstrumentBase*> get_randomizable_instruments()
        {
            std::vector<InstrumentBase*> ptrs;
            std::vector<InstrumentBase*> all_ptrs = get_active_instrument();
            for (auto ptr : all_ptrs)
            {
                if (ptr->is_randomizable())
                {
                    ptrs.push_back(ptr);
                }
            }
            return ptrs;
        }

        void stop_all()
        {
            for (auto instrument : get_instrument_ptrs())
            {
                instrument->stop_notes();
            }
        }

        void set_regular()
        {
            acid_bass.set_active(true);
            drone.set_active(true);
            mono.set_active(true);
            mono_dub.set_active(true);

            fugue_vermona_1.set_active(false);
            fugue_vermona_2.set_active(false);
            fugue_vermona_3.set_active(false);
            fugue_vermona_4.set_active(false);
        }

        void set_fugue()
        {
            fugue_vermona_1.set_active(true);
            fugue_vermona_2.set_active(true);
            fugue_vermona_3.set_active(true);
            fugue_vermona_4.set_active(true);

            acid_bass.set_active(false);
            drone.set_active(false);
            mono.set_active(false);
            mono_dub.set_active(false);
        }
    };
}