#include "sound.h"

riv_waveform_desc move_sfx = {
    .type = RIV_WAVEFORM_TRIANGLE,
    .delay = 0.0f,
    .attack = 0.0f,
    .decay = 0.05f,
    .sustain = 0.025f,
    .release = 0.025f,
    .start_frequency = RIV_NOTE_G2,
    .end_frequency = RIV_NOTE_G2,
    .amplitude = 0.25f,
    .sustain_level = 1.0f,
    .duty_cycle = 0.2f,
    .pan = 0.0f,
};

riv_waveform_desc coin_sfx = {
    .type = RIV_WAVEFORM_TRIANGLE,
    .delay = 0.0f,
    .attack = 0.025f,
    .decay = 0.075f,
    .sustain = 0.0f,
    .release = 0.12f,
    .start_frequency = RIV_NOTE_A6,
    .end_frequency = RIV_NOTE_A6,
    .amplitude = 0.25f,
    .sustain_level = 0.3f,
    .duty_cycle = 0.5f,
    .pan = 0,
};

riv_waveform_desc end_sfx = {
    .type = RIV_WAVEFORM_PULSE,
    .delay = 0.0f,
    .attack = 0.01f,
    .decay = 0.15f,
    .sustain = 0.1f,
    .release = 0.1f,
    .start_frequency = RIV_NOTE_A2,
    .end_frequency = RIV_NOTE_Gb0,
    .amplitude = 0.2f,
    .sustain_level = 0.5f,
    .duty_cycle = 0.5f,
    .pan = 0,
};

void play_start()
{
    riv_waveform(&coin_sfx);
}

void play_move()
{
    riv_waveform(&move_sfx);
}

void play_score()
{
    riv_waveform(&coin_sfx);
}

void play_game_over()
{
    riv_waveform(&end_sfx);
}
