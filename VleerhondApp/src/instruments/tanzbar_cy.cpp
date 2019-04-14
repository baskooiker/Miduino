#include "tanzbar_cy.h"

#include "tanzbar_settings.h"

const RandomParam tanzbar_cy_params[] = {
    {TB_CY_DECAY,  64, 127},
    {TB_CY_TONE ,   0, 127},
    {TB_CY_TUNE ,   0, 127}
};
const uint8_t nr_of_tanzbar_cy_params = sizeof(tanzbar_cy_params) / sizeof(*tanzbar_cy_params);

void TanzbarCy::randomize()
{
    ofLogNotice("tanzbar_hi", "randomize()");
    last_randomized_time = millis();

    Tanzbar::randomize_parameters(tanzbar_cy_params, nr_of_tanzbar_cy_params);

    randomize_hi_seq();

    // Modulators
    uint8_t range = Rand::randui8(128);
    this->cy_tune.randomize(range, 127 - range, .3);

    range = Rand::randui8(16, 64);
    this->cy_vel.randomize(range, 127 - range);

    timing.randomize();
}