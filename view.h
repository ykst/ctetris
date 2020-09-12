#pragma once
#include "tetris.h"
#include <ncurses.h>

#define HOLD_OFFSET_X 3
#define AREA_OFFSET_X 12
#define AREA_OFFSET_Y 5
#define SCORE_TEXT_OFFSET_X 3
#define SCORE_TEXT_OFFSET_Y 8

static const int MINO_COLORS[MINO_KINDS] = {
    COLOR_CYAN,  COLOR_BLUE,    COLOR_WHITE, COLOR_YELLOW,
    COLOR_GREEN, COLOR_MAGENTA, COLOR_RED};

enum scene_types {
    SCENE_START = 0,
    SCENE_INGAME,
    SCENE_PAUSE,
    SCENE_GAMEOVER,
    SCENE_KINDS,
};

enum transition_types {
    TRANSITION_REPLACE = 1,
    TRANSITION_PUSH,
    TRANSITION_POP,
    TRANSITION_OVERLAY,
};

static const int MAX_SCORE_DELAY = 300;

struct scene_t {
    void (*update)(struct scene_t *);
    void (*enter)(struct scene_t *, enum transition_types transition,
                  enum scene_types from);
};
