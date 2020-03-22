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

    void Mono::randomizeArp()
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

    void Mono::randomizeRhythm()
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

        this->gate_pattern.setEuclid(length, euclid_steps1);
        this->gate_pattern.patterns[1].setEuclid(length, euclid_steps2);
        this->gate_pattern.length = length;

        this->gate_pattern.time_division = TimeDivision::Sixteenth;

        PatternUtils::randomizeSlides(this->slide_pattern);

        PatternUtils::randomizeAccents(this->accent_pattern);

        // Randomize Lead
        this->lead_pattern.randomizeIntervalLead();
    }

    void Mono::randomizeChaos()
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
            randomizeArp();
            break;
        case 1:
            randomizeRhythm();
            break;
        case 2:
            randomizeChaos();
            break;
        }
    }

    void Mono::totalRandomize()
    {
        TonalInstrumentBase::randomize();

        randomizeArp();
        randomizeRhythm();
        randomizeChaos();

        switch (Rand::distribution(
            settings.p_arp,
            settings.p_euclid))
        {
        case 0: this->style = MonoStyle::MonoSixteenths; break;
        case 1: this->style = MonoStyle::MonoPolyRhythm; break;
        }
    }

    bool Mono::getHit() const
    {
        switch (this->style)
        {
        case MonoStyle::MonoSixteenths:
            return Utils::intervalHit(TimeDivision::Sixteenth, time);
        case MonoStyle::MonoPolyRhythm:
            return this->gate_pattern.gate(time);
        }
        return false;
    }

    uint8_t Mono::getSequencePitch() const
    {
        uint8_t pitch = 0;

        // TODO: Fix this some other way!
        // This is done to make the const pattern follow chords
        if (pitch_pattern.length == 1 && pitch_pattern.value(0) == 0)
        {
            int note_nr = harmony.getChordStep(time).root;

            pitch = harmony.scale.applyScaleOffset(
                0,
                this->getVariablePitchOffset(),
                harmony.getChordStep(time).root
            );
        }
        else
        {
            pitch = harmony.scale.getPenta(this->pitch_pattern.value(time));
            pitch = Utils::clipPitch(pitch, this->getVariablePitchOffset());
        }

        uint8_t octave = Utils::rerange(octave_pattern.value(time), this->octave_range);
        pitch += octave * 12;
        return pitch;
    }

    uint8_t Mono::getNextMonoPitch()
    {
        uint8_t pitch = 0;
        if (pitch_mode == MonoPitchMode::SEQUENCE)
        {
            pitch = getSequencePitch();
        }
        else if (pitch_mode == MonoPitchMode::ARP)
        {
            uint8_t range = 48;
            uint8_t offset = 36;
            uint8_t pitch_offset_value = Utils::rerange(this->_variable_pitch_offset, range, offset);
            this->arp_data.min = pitch_offset_value - arp_data.range / 3;
            pitch = this->arp_data.getNextArpPitch(harmony.scale, harmony.getChordStep(time));
        }
        return pitch;
    }

    uint8_t Mono::getMonoPitch() const
    {
        if (pitch_mode == MonoPitchMode::SEQUENCE)
        {
            return getSequencePitch();
        }
        // Else MonoPitchMode::ARP
        return this->arp_data.getArpPitch();
    }

    NoteInfo Mono::getNoteEvent()
    {
        if (time.tick != note_event.tick)
        {
            note_event.tick = time.tick;
            note_event.hit = getHit();
            if (note_event.hit)
            {
                note_event.note = NoteStruct(
                    this->getNextMonoPitch(), 
                    getVelocity(), 
                    12, 
                    NoteType::Tie
                );
            }
        }
        return note_event;
    }

    bool Mono::play()
    {
        this->checkArpReset();

        if (this->isKilled())
        {
            return false;
        }

        NoteInfo new_note_event = getNoteEvent();
        if (new_note_event.hit)
        {
            this->midi_channel->noteOn(new_note_event.note, time.getShuffleDelay());
            return true;
        }
        return false;
    }

    void Mono::checkArpReset()
    {
        if (Utils::intervalHit(this->arp_reset_interval, time))
        {
            arp_data.counter = 0;
        }
    }

    void Mono::setArpType(ArpType arp_type)
    {
        arp_data.type = arp_type;
    }

    void Mono::setStyle(MonoStyle mono_style)
    {
        style = mono_style;
    }

    void Mono::setPitchMode(const MonoPitchMode pitch_mode)
    {
        this->pitch_mode = pitch_mode;
    }

    void Mono::setArpRange(const int range)
    {

        arp_data.range = std::max(12, range);
    }

    void Mono::setConstSequence()
    {
        this->pitch_pattern.setAll(0);
        this->octave_pattern.setAll(0);
        pitch_pattern.length = 1;
    }

    void Mono::setSlowRhythm()
    {
        this->gate_pattern.setAll(0);
        this->gate_pattern.set(0, true);
        this->gate_pattern.time_division = TimeDivision::Eighth;
        switch (Rand::distribution(16, 16))
        {
        case 0: gate_pattern.length = 8; break;
        case 1: gate_pattern.length = 16; break;
        }
    }

    uint8_t Mono::getVelocity()
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
        this->slide_pattern.setAll(0);
    }
    bool Mono::getPedal()
    {
        return slide_pattern.gate(this->time);
    }
}
