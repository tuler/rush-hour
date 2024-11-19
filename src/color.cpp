#include "color.h"

void color_setup()
{
    // https://coolors.co/gradient-palette/000000-ffffff?number=8
    riv->palette[RUSH_COLOR_BLACK] = 0x000000;
    riv->palette[RUSH_COLOR_WHITE] = 0xFFFFFF;

    riv->palette[RUSH_COLOR_GREY_0] = 0xDBDBDB;
    riv->palette[RUSH_COLOR_GREY_1] = 0xB6B6B6;
    riv->palette[RUSH_COLOR_GREY_2] = 0x929292;
    riv->palette[RUSH_COLOR_GREY_3] = 0x6D6D6D;
    riv->palette[RUSH_COLOR_GREY_4] = 0x494949;
    riv->palette[RUSH_COLOR_GREY_5] = 0x242424;

    // https://coolors.co/gradient-palette/f2eacd-ffffff?number=7
    riv->palette[RUSH_COLOR_BEIGE_0] = 0xF7FCFD;
    riv->palette[RUSH_COLOR_BEIGE_1] = 0xEEF8FB;
    riv->palette[RUSH_COLOR_BEIGE_2] = 0xE6F5F9;
    riv->palette[RUSH_COLOR_BEIGE_3] = 0xDEF1F6;
    riv->palette[RUSH_COLOR_BEIGE_4] = 0xD5EEF4;
    riv->palette[RUSH_COLOR_BEIGE_5] = 0xCDEAF2;

    // https://coolors.co/gradient-palette/338899-ffffff?number=7 (to white)
    // https://coolors.co/gradient-palette/338899-f2eacd?number=7 (to beige)
    riv->palette[RUSH_COLOR_TEAL_0] = 0xC4DAD2; // 0xEEEBDD;
    riv->palette[RUSH_COLOR_TEAL_1] = 0xBCC9B2; // 0xDDD7BB;
    riv->palette[RUSH_COLOR_TEAL_2] = 0xB3B993; // 0xCCC499;
    riv->palette[RUSH_COLOR_TEAL_3] = 0xAAA973; // 0xBBB077;
    riv->palette[RUSH_COLOR_TEAL_4] = 0xA29853; // 0xAA9C55;
    riv->palette[RUSH_COLOR_TEAL_5] = 0x998833;

    // https://coolors.co/gradient-palette/cc3333-ffffff?number=7
    riv->palette[RUSH_COLOR_RED_0] = 0xDDDDF7;
    riv->palette[RUSH_COLOR_RED_1] = 0xBBBBEE;
    riv->palette[RUSH_COLOR_RED_2] = 0x9999E6;
    riv->palette[RUSH_COLOR_RED_3] = 0x7777DD;
    riv->palette[RUSH_COLOR_RED_4] = 0x5555D5;
    riv->palette[RUSH_COLOR_RED_5] = 0x3333CC;

    // https://coolors.co/gradient-palette/ffd800-ffffff?number=7
    riv->palette[RUSH_COLOR_YELLOW_0] = 0xD5F9FF;
    riv->palette[RUSH_COLOR_YELLOW_1] = 0xAAF2FF;
    riv->palette[RUSH_COLOR_YELLOW_2] = 0x80ECFF;
    riv->palette[RUSH_COLOR_YELLOW_3] = 0x55E5FF;
    riv->palette[RUSH_COLOR_YELLOW_4] = 0x2ADFFF;
    riv->palette[RUSH_COLOR_YELLOW_5] = 0x00D8FF;
}

uint32_t color_alpha(uint32_t color, uint32_t base_color, float alpha)
{
    uint32_t base = color - 6;
    uint32_t c = alpha * 6 + base;
    return c == base ? base_color : c;
}

uint32_t color_bw_alpha(float alpha)
{
    uint32_t base = RUSH_COLOR_WHITE;
    uint32_t c = alpha * 8 + base;
    return c == 8 ? RUSH_COLOR_BLACK : c;
}
