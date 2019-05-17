#pragma once

#include <algorithm>

#include "defs.h"
#include "harmony_struct.h"
#include "modulators.h"
#include "parameters.h"
#include "tanzbar_lo.h"

#include "tanzbar_sd.h"
#include "tanzbar_cp.h"

#include "tanzbar_tom.h"
#include "tanzbar_cb.h"
#include "tanzbar_cl.h"

#include "tanzbar_ma.h"
#include "tanzbar_hats.h"
#include "tanzbar_cy.h"

#include "mfb_503.h"

#include "rocket_bass.h"
#include "acid_bass.h"
#include "bass_dub_settings.h"
#include "mono_settings.h"
#include "mono_dub_settings.h"
#include "poly.h"
#include "lead.h"
#include "drone.h"

namespace Vleerhond
{
    class ApplicationData
    {
    public:
        TimeStruct time;
        HarmonyStruct harmony;

        FugueSettings fugue_settings;
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
        MonoSettings mono_settings;
        MonoDubSettings mono_dub_settings;
        Drone drone;

        PolySettings poly_settings;
        LeadSettings lead_settings;

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

            rocket_bass(harmony, time),
            acid_bass(harmony, time),
            mono_settings(harmony, time),
            mono_dub_settings(mono_settings, harmony, time),
            poly_settings(harmony, time),
            lead_settings(harmony, time),
            drone(harmony, time)
        {
            mono_settings.storage.set_channel(MIDI_CHANNEL_MONO);
            mono_dub_settings.storage.set_channel(MIDI_CHANNEL_MONO_2);

            drone.storage.set_channel(MIDI_CHANNEL_BASS_DUB);

            this->poly_settings.storage.set_channel(MIDI_CHANNEL_POLY);
            this->lead_settings.storage.set_channel(MIDI_CHANNEL_LEAD);

            this->randomize_all();
        }

        void probability_randomize()
        {
            if (!Utils::interval_hit(TimeDivision::Four, time.add(TICKS_PER_STEP / 2)))
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
            for (auto instrument : get_instrument_ptrs())
            {
                instrument->play();
            }

            probability_randomize();
        }

        void process_active_notes()
        {
            for (auto instrument : get_instrument_ptrs())
            {
                instrument->storage.process_active_notes();
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
            this->fugue_settings.randomize_fugue();
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

            ptrs.push_back(&this->mfb_503_kick);
            ptrs.push_back(&this->mfb_503_toms);
            ptrs.push_back(&this->mfb_503_hats);
            ptrs.push_back(&this->mfb_503_snare);
            ptrs.push_back(&this->mfb_503_cymbal);

            ptrs.push_back(&this->rocket_bass);
            ptrs.push_back(&this->acid_bass);
            //ptrs.push_back(&this->bass_dub_settings);
            ptrs.push_back(&this->mono_settings);
            ptrs.push_back(&this->mono_dub_settings);
            ptrs.push_back(&this->poly_settings);
            ptrs.push_back(&this->lead_settings);
            ptrs.push_back(&this->drone);

            return ptrs;
        }

        std::vector<InstrumentBase*> get_randomizable_instruments()
        {
            std::vector<InstrumentBase*> ptrs;
            std::vector<InstrumentBase*> all_ptrs = get_instrument_ptrs();
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
    };
}