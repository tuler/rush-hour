extern "C"
{
#include "riv.h"
}

#define RUSH_COLOR_BLACK 0
#define RUSH_COLOR_WHITE 1

#define RUSH_COLOR_GREY_0 2
#define RUSH_COLOR_GREY_1 3
#define RUSH_COLOR_GREY_2 4
#define RUSH_COLOR_GREY_3 5
#define RUSH_COLOR_GREY_4 6
#define RUSH_COLOR_GREY_5 7

#define RUSH_COLOR_BEIGE_0 8
#define RUSH_COLOR_BEIGE_1 9
#define RUSH_COLOR_BEIGE_2 10
#define RUSH_COLOR_BEIGE_3 11
#define RUSH_COLOR_BEIGE_4 12
#define RUSH_COLOR_BEIGE_5 13

#define RUSH_COLOR_TEAL_0 14
#define RUSH_COLOR_TEAL_1 15
#define RUSH_COLOR_TEAL_2 16
#define RUSH_COLOR_TEAL_3 17
#define RUSH_COLOR_TEAL_4 18
#define RUSH_COLOR_TEAL_5 19

#define RUSH_COLOR_RED_0 20
#define RUSH_COLOR_RED_1 21
#define RUSH_COLOR_RED_2 22
#define RUSH_COLOR_RED_3 23
#define RUSH_COLOR_RED_4 24
#define RUSH_COLOR_RED_5 25

#define RUSH_COLOR_YELLOW_0 26
#define RUSH_COLOR_YELLOW_1 27
#define RUSH_COLOR_YELLOW_2 28
#define RUSH_COLOR_YELLOW_3 29
#define RUSH_COLOR_YELLOW_4 30
#define RUSH_COLOR_YELLOW_5 31

#define RUSH_COLOR_BACKGROUND 1
#define RUSH_COLOR_BOARD 13
#define RUSH_COLOR_GRID_LINE 0
#define RUSH_COLOR_PRIMARY_PIECE 25
#define RUSH_COLOR_PIECE 19
#define RUSH_COLOR_PIECE_OUTLINE 0
#define RUSH_COLOR_LABEL 0
#define RUSH_COLOR_WALL 6

/**
 * Setup the color palette in RIV.
 */
void color_setup();

/**
 * Get the color with the given alpha.
 */
uint32_t color_alpha(uint32_t color, uint32_t base_color, float alpha);

/**
 * Get the black or white color with the given alpha.
 */
uint32_t color_bw_alpha(float alpha);