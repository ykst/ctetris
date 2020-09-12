#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

enum mino_definition {
    MINO_I = 0,
    MINO_J,
    MINO_L,
    MINO_O,
    MINO_S,
    MINO_T,
    MINO_Z,
    MINO_KINDS
};

static const int MINO_SPAWN_X = 3;
static const int MINO_SPAWN_Y = 19;

static const int MAX_NEXT = 7;

static const int MINO[MINO_KINDS][4] = {
    [MINO_I] = {0x0F00, 0x2222, 0x00F0, 0x4444},
    [MINO_J] = {0x8E00, 0x6440, 0x0E20, 0x44c0},
    [MINO_L] = {0x2E00, 0x4460, 0x0E80, 0xc440},
    [MINO_O] = {0x6600, 0x6600, 0x6600, 0x6600},
    [MINO_S] = {0x6c00, 0x4620, 0x06c0, 0x8c40},
    [MINO_T] = {0x4e00, 0x4640, 0x0e40, 0x4c40},
    [MINO_Z] = {0xc600, 0x2640, 0x0c60, 0x4c80},
};

#define AREA_WIDTH 10
#define AREA_HEIGHT 40
#define AREA_VISIBLE_HEIGHT 21

#define iter_minos(mino, rot, i, j, action)                                    \
    int _mino = MINO[mino][rot];                                               \
    for (i = 0; i < 4; ++i) {                                                  \
        for (j = 0; j < 4; ++j) {                                              \
            if ((_mino >> ((3 - j) + (3 - i) * 4)) & 1) {                      \
                action;                                                        \
            }                                                                  \
        }                                                                      \
    }

struct controller_t {
    int x;
    int y;
    int rot;
    int touched_rot;
    int mino;
    int nexts[MAX_NEXT];
    int next_idx;
    int tick;
    int hold;
    int fix_delay;
    int drop_delay;
    bool soft;
    bool hold_lock;
};

struct score_t {
    uint64_t total;
    uint64_t combo;
    uint64_t level;
    uint64_t lines;

    struct {
        uint64_t delta;
        bool tspin;
        int lines;
        bool b2b;
    } current;
};

struct tetris_t {
    struct controller_t controller;
    struct score_t score;
    bool gameover;
    uint8_t field[AREA_HEIGHT][AREA_WIDTH];
};

void tetris_update_score(struct tetris_t *tetris, int lines);

void tetris_init(struct tetris_t *tetris);
void tetris_rotate(struct tetris_t *tetris, int clockwise);
void tetris_hold(struct tetris_t *tetris);
void tetris_soft_drop(struct tetris_t *tetris);
void tetris_hard_drop(struct tetris_t *tetris);
void tetris_move_x(struct tetris_t *tetris, int x);
void tetris_tick(struct tetris_t *tetris);
bool tetris_check(struct tetris_t *tetris, int offset_x, int offset_y);
