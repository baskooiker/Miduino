#include "mono.h"

#include "patterns/pattern_utils.h"

#include "rand.h"

namespace Vleerhond
{
    Mono::Mono(
        HarmonyStruct& harmony,
        Modulators& modulators,
        TimeStruct& time) :
        TonalInstrumentBase(harmony, time)
    {
        style = MonoStyle::MonoSixteenths;
        arp_reset_interval = TimeDivision::Whole;
    }

    void Mono::randomize_arp()
    {
        this->arp_data.range = Rand::randui8(12, 36);

        switch (Rand::randui8(4))
        {
        case 0: this->arp_data.type = ArpType::UP; break;
        case 1: this->arp_data.type = ArpType::DOWN; break;
        case 2: this->arp_data.type = ArpType::UPDOWN; break;
        case 3: this->arp_data.type = ArpType::PICKING_IN; break;
        }

        // Pitch patterns
        pitch_pattern.randomize();
        switch (Rand::distribution(16, 16))
        {
        case 0: pitch_pattern.length = 8; break;
        case 1: pitch_pattern.length = 16; break;
        }
        octave_pattern.randomize();
        switch (Rand::distribution(16, 16))
        {
        case 0: octave_pattern.length = 8; break;
        case 1: octave_pattern.length = 16; break;
        }

        octave_range = Rand::randi8(1, 3);
    }

    void Mono::randomize_rhythm()
    {
        // Set Euclid
        uint8_t length = 16;
        uint8_t euclid_steps1 = 4;
        uint8_t euclid_steps2 = 8;
        switch (Rand::distribution(16, 16))
        {
        case 0:
            length = 8;
            euclid_steps1 = Rand::randui8(2, 6);
            euclid_steps2 = Rand::randui8(2, 6);
            break;
        case 1:
            length = 16;
            euclid_steps1 = Rand::randui8(4, 12);
            euclid_steps2 = Rand::randui8(4, 12);
            break;
        }

        this->gate_pattern.set_euclid(length, euclid_steps1);
        this->gate_pattern.patterns[1].set_euclid(length, euclid_steps2);
        this->gate_pattern.length = length;

        this->gate_pattern.time_division = TimeDivision::Sixteenth;

        PatternUtils::randomize_slides(this->slide_pattern);

        PatternUtils::randomize_accents(this->accent_pattern);

        // Randomize Lead
        this->lead_pattern.randomize_interval_lead();
    }

    void Mono::randomize_chaos()
    {
        switch (Rand::distribution(16, 16, 16, 16))
        {
        case 0: arp_reset_interval = TimeDivision::Whole; break;
        case 1: arp_reset_interval = TimeDivision::Two; break;
        case 2: arp_reset_interval = TimeDivision::Four; break;
        case 3: arp_reset_interval = TimeDivision::Eight; break;
        }
    }

    void Mono::randomize()
    {
        ofLogVerbose("mono", "randomize()");
        TonalInstrumentBase::randomize();

        switch (Rand::distribution(16, 16, 16))
        {
        case 0:
            randomize_arp();
            break;
        case 1:
            randomize_rhythm();
            break;
        case 2:
            randomize_chaos();
            break;
        }
    }

    void Mono::total_randomize()
    {
        TonalInstrumentBase::randomize();

        randomize_arp();
        randomize_rhythm();
        randomize_chaos();

        switch (Rand::distribution(
            settings.p_arp,
            settings.p_euclid))
        {
        case 0: this->style = MonoStyle::MonoSixteenths; break;
        case 1: this->style = MonoStyle::MonoPolyRhythm; break;
        }
    }

    bool Mono::get_hit() const
    {
        switch (this->style)
        {
        case MonoStyle::MonoSixteenths:
            return Utils::interval_hit(TimeDivision::Sixteenth, time);
        case MonoStyle::MonoPolyRhythm:
            return this->gate_pattern.gate(time);
        }
        return false;
    }

    uint8_t Mono::get_sequence_pitch() const
    {
        uint8_t pitch = 0;

        // TODO: Fix this some other way!
        // This is done to make the const pattern follow chords
        if (pitch_pattern.length == 1 && pitch_pattern.value(0) == 0)
        {
            int note_nr = harmony.getChordStep(time).root;

            pitch = harmony.scale.apply_scale_offset(
                0,
                this->getVariablePitchOffset(),
                harmony.getChordStep(time).root
            );
        }
        else
        {
            pitch = harmony.scale.getPenta(this->pitch_pattern.value(time));
            pitch = Utils::clip_pitch(pitch, this->getVariablePitchOffset());
        }

        uint8_t octave = Utils::rerange(octave_pattern.value(time), this->octave_range);
        pitch += octave * 12;
        return pitch;
    }

    uint8_t Mono::get_next_mono_pitch()
    {
        uint8_t pitch = 0;
        if (pitch_mode == MonoPitchMode::SEQUENCE)
        {
            pitch = get_sequence_pitch();
        }
        else if (pitch_mode == MonoPitchMode::ARP)
        {
            this->arp_data.min = Utils::rerange(this->_variable_pitch_offset, 48, 36);
            pitch = this->arp_data.getNextArpPitch(harmony.scale, harmony.getChordStep(time));
        }
        return pitch;
    }

    uint8_t Mono::get_mono_pitch() const
    {
        if (pitch_mode == MonoPitchMode::SEQUENCE)
        {
            return get_sequence_pitch();
        }
        // Else MonoPitchMode::ARP
        return this->arp_data.getArpPitch();
    }

    NoteInfo Mono::get_note_event()
    {
        if (time.tick != note_event.tick)
        {
            note_event.tick = time.tick;
            note_event.hit = get_hit();
            if (note_event.hit)
            {
                note_event.note = NoteStruct(
                    this->get_next_mono_pitch(), 
                    get_velocity(), 
                    12, 
                    NoteType::Tie
                );
            }
        }
        return note_event;
    }

    bool Mono::play()
    {
        this->check_arp_reset();

        if (this->isKilled())
        {
            return false;
        }

        NoteInfo new_note_event = get_note_event();
        if (new_note_event.hit)
        {
            this->midi_channel->note_on(new_note_event.note, time.get_shuffle_delay());
            return true;
        }
        return false;
    }

    void Mono::check_arp_reset()
    {
        if (Utils::interval_hit(this->arp_reset_interval, time))
        {
            arp_data.counter = 0;
        }
    }

    void Mono::set_arp_type(ArpType arp_type)
    {
        arp_data.type = arp_type;
    }

    void Mono::set_style(MonoStyle mono_style)
    {
        style = mono_style;
    }

    void Mono::set_pitch_mode(const MonoPitchMode pitch_mode)
    {
        this->pitch_mode = pitch_mode;
    }

    void Mono::set_arp_range(const int range)
    {

        arp_data.range = std::max(12, range);
    }

    void Mono::set_const_sequence()
    {
        this->pitch_pattern.set_all(0);
        this->octave_pattern.set_all(0);
        pitch_pattern.length = 1;
    }

    void Mono::set_slow_rhythm()
    {
        this->gate_pattern.set_all(0);
        this->gate_pattern.set(0, true);
        this->gate_pattern.time_division = TimeDivision::Eighth;
        switch (Rand::distribution(16, 16))
        {
        case 0: gate_pattern.length = 8; break;
        case 1: gate_pattern.length = 16; break;
        }
    }

    uint8_t Mono::get_velocity()
    {
        // TODO: variation on min/max velocity
        if (accent_pattern.gate(time))
        {
            return settings.max_velocity;
        }
        return settings.min_velocity;
    }
    void Mono::disableSlides()
    {
        this->slide_pattern.set_all(0);
    }
    bool Mono::getPedal()
    {
        return slide_pattern.gate(this->time);
    }
}
