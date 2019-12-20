#include "bass.h"

namespace Vleerhond
{

Bass::Bass(
    HarmonyStruct& harmony_ref,
    TimeStruct& time_ref,
    const uint8_t midi_channel,
    const int8_t offset
) :
    TonalInstrumentBase(harmony_ref, time_ref, true, midi_channel, offset),
    octave_sh(TimeDivision::Sixteenth)
{
    style = BassStyle::BassEuclid;
    note_range_value = 0;
    density = 0;
    octave_sh.prob = 16;

    total_randomize();
}

void Bass::randomize_octaves()
{
    ofLogNotice("", "randomize_octaves");
    this->octaves.randomize();
    //this->octaves.patterns[0].set(0, 0);
    switch (Rand::distribution(0, 16, 16, 16))
    {
    case 0: this->octaves.length = 2; break;
    case 1: this->octaves.length = 4; break;
    case 2: this->octaves.length = 8; break;
    case 3: this->octaves.length = 16; break;
    }
    this->variable_octaves.randomize();
    switch (Rand::distribution(16, 16, 16, 16))
    {
    case 0: this->variable_octaves.length = 2; break;
    case 1: this->variable_octaves.length = 4; break;
    case 2: this->variable_octaves.length = 8; break;
    case 3: this->variable_octaves.length = 16; break;
    }
}

void Bass::randomize_pitches()
{
    ofLogNotice("", "randomize_pitches");
    // Randomize pitches
    this->pitches.randomize();
    for (int i = 0; i < 3; i++)
    {
        this->pitches.patterns[i].set(0, 0);
    }

    uint8_t prob_2 = this->pitches.abPattern.isConstant() ? 0 : 32;
    switch (Rand::distribution(prob_2, 16, 16))
    {
    case 0: this->pitches.length = 2; break;
    case 1: this->pitches.length = 4; break;
    case 2: this->pitches.length = 8; break;
    }

    this->note_range_prob.randomize();
    switch (Rand::distribution(0, 0, 16, 0))
    {
    case 0: this->note_range_prob.length = 2; break;
    case 1: this->note_range_prob.length = 4; break;
    case 2: this->note_range_prob.length = 8; break;
    case 3: this->note_range_prob.length = 16; break;
    }
}

void Bass::randomize_gates()
{
    ofLogNotice("", "randomize_gates");
    // Randomize gates
    this->probs.randomize();
    switch (Rand::distribution(32, 16, 16))
    {
    case 0: this->probs.length = 4; break;
    case 1: this->probs.length = 8; break;
    case 2: this->probs.length = 16; break;
    }

    switch (Rand::distribution(
        settings.p_euclid_16,
        settings.p_euclid_8,
        settings.p_interval,
        settings.p_diddles
    ))
    {
    case 0:
    {
        // Euclid length 16
        uint8_t steps = 3;
        switch (Rand::distribution(
            settings.euclid_16.p_5,
            settings.euclid_16.p_6,
            settings.euclid_16.p_7,
            settings.euclid_16.p_9,
            settings.euclid_16.p_11))
        {
        case 0: steps = 5;  break;
        case 1: steps = 6;  break;
        case 2: steps = 7;  break;
        case 3: steps = 9;  break;
        case 4: steps = 11;  break;
        }
        this->euclid_pattern.set_euclid(16, steps);
        this->euclid_pattern.length = 16;
        style = BassStyle::BassEuclid;
        break;
    }
    case 1:
    {
        // Euclid length 8
        uint8_t steps = 3;
        switch (Rand::distribution(
            settings.euclid_8.p_3,
            settings.euclid_8.p_5,
            settings.euclid_8.p_7))
        {
        case 0: steps = 3;  break;
        case 1: steps = 5;  break;
        case 2: steps = 7;  break;
        }
        this->euclid_pattern.set_euclid(8, steps);
        this->euclid_pattern.length = 8;
        style = BassStyle::BassEuclid;
        break;
    }
    case 2:
    {
        // Settings interval pattern
        this->int_pattern.randomize_interval(arp_interval_probs);
        style = BassStyle::BassArpInterval;
        break;
    }
    case 3:
    {
        // Setting diddles
        uint8_t length = 8;
        switch (Rand::distribution(16, 16))
        {
        case 0:
            length = 8;
            break;
        case 1:
            length = 16;
            break;
        }
        euclid_pattern.set_diddles(
            Rand::randf(
                settings.diddles.p_min,
                settings.diddles.p_max
            ),
            true,
            length
        );
        if (length > 8)
        {
            euclid_pattern.abPattern.set_ab_pattern_const(0);
        }
        style = BassStyle::BassEuclid;
        break;
    }
    }

}

void Bass::randomize_accents()
{
    ofLogNotice("", "randomize_accents");
    this->slides.randomize(Rand::randf(.50f, .80f));
    slides.length = 8;
    slides.set(0, false);

    this->accents.randomize(Rand::randf(.15f, .4f));
    switch (Rand::distribution(0, 16))
    {
    case 0:
        this->accents.length = 4;
        break;
    case 1:
        break;
        this->accents.length = 8;
    }
}

void Bass::randomize()
{
    TonalInstrumentBase::randomize();

    octave_sh.prob = Rand::randui8(settings.p_octave_sh);

    switch (Rand::distribution(16, 16, 0, 16))
    {
    case 0: randomize_octaves(); break;
    case 1: randomize_pitches(); break;
    case 2: randomize_gates(); break;
    case 3: randomize_accents(); break;
    }

}

void Bass::total_randomize()
{
    TonalInstrumentBase::randomize();

    octave_sh.prob = Rand::randui8(32);

    randomize_octaves();
    randomize_pitches();
    randomize_gates();
    randomize_accents();
}

bool Bass::get_hit(const uint8_t density, const TimeStruct& time)
{
    bool hit = false;
    switch (this->style)
    {
    case BassStyle::BassEuclid:
        hit = this->euclid_pattern.gate(time);
        break;
    case BassStyle::BassArpInterval:
        hit = this->int_pattern.hit(time);
        break;
    }

    uint8_t prob = this->probs.value(time);
    bool prob_step = (prob < density) && (prob > 0) && Utils::interval_hit(TimeDivision::Sixteenth, time);
    return hit || prob_step;
}

uint8_t Bass::get_pitch()
{
    // TODO: Hier klopt dus niks van...
    uint8_t note_nr = 0;
    uint8_t note_range_p = this->note_range_prob.value(time);

    // TODO: Deze geeft veel te vaak false.
    if (note_range_p < this->note_range_value)
    {
        uint8_t pitch_cv = harmony.scale.get_note(this->pitches.value(time));

        if (this->note_range_value < 64)
        {
            note_nr = Utils::to_chord_order(pitch_cv);
        }
        else
        {
            if (note_range_p % 64 < this->note_range_value % 64)
            {
                note_nr = pitch_cv;
            }
            else
            {
                note_nr = Utils::to_chord_order(pitch_cv);
            }
        }
    }

    uint8_t pitch = harmony.scale.apply_scale_offset(
        note_nr,
        this->pitch_offset,
        this->follow_harmony ? harmony.get_chord_step(time) : 0
    );

    uint8_t octave = this->octaves.value(time);
    std::vector<int> octave_opts;
    if (octave < 32)
        octave_opts = { 0, 0,  0, 0 , 1, 1};
    else if (octave < 64)
        octave_opts = { 0, 0, -1, 1 , 1, 1};
    else if (octave < 96)
        octave_opts = { 0, 1, -1, 1 , 2, 2};
    else
        octave_opts = { 0, 1, -1, 1 , 2, 3};
    if (octave < this->variable_octave)
    {
        pitch += octave_opts[octave % 6] * 12;
    }

    return pitch;
    //return Utils::clip_pitch(
    //    pitch, 
    //    this->pitch_offset,
    //    Utils::rerange(variable_octave, this->pitch_offset + 12, 36)
    //);
}

bool Bass::play()
{
    if (this->kill)
    {
        return false;
    }

    // Get hit

    if (this->get_hit(this->density, time))
    {
        uint8_t pitch = get_pitch();

        // Note length
        uint8_t length = this->accents.gate(time) ? 6 : 2;
        NoteType note_type = NoteType::Tie;
        if (this->slides.gate(time))
        {
            length = time.ticks_left_in_bar();
            length = TICKS_PER_STEP * 4;
            note_type = NoteType::Slide;
        }
        else
        {
            if (!this->get_hit(this->density, time.add(TICKS_PER_STEP)))
            {
                length = TICKS_PER_STEP * 2;
            }
        }

        length = TICKS_PER_STEP * 4;

        // Sample and hold on random octave jumps
        if (octave_sh.gate(time))
        {
            pitch += 12;
        }

        // Play it!
        this->midi_channel.note_on(
            NoteStruct(pitch, this->get_velocity(), length, note_type),
            time.get_shuffle_delay()
        );
        return true;
    }
    return false;
}

std::string Bass::toString()
{
    std::stringstream ss;
    ss << "gates\n";
    ss << this->euclid_pattern.toString() << "\n\n";
    ss << "pitches\n";
    ss << this->pitches.toString() << "\n";
    ss << "octaves\n";
    ss << this->octaves.toString() << "\n";
    return ss.str();
}

}
