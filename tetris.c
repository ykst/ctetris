#include "tetris.h"
#include "master.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static void fix(struct tetris_t *tetris);
static bool check(struct tetris_t *tetris, int offset_x, int offset_y);

static bool is_tspin(struct tetris_t *tetris)
{
    if (tetris->controller.mino != MINO_T ||
        tetris->controller.touched_rot == tetris->controller.rot) {
        return false;
    }

    int count = 0;

    count += tetris->field[tetris->controller.y][tetris->controller.x] != 0;
    count += tetris->field[tetris->controller.y][tetris->controller.x + 2] != 0;
    count += tetris->field[tetris->controller.y + 2][tetris->controller.x] != 0;
    count +=
        tetris->field[tetris->controller.y + 2][tetris->controller.x + 2] != 0;

    return count >= 3;
}

static void normalize(struct tetris_t *tetris)
{
    int lines[4] = {};
    int line_idx = 0;

    for (int i = 0; i < AREA_HEIGHT; ++i) {
        bool line_fill = true;
        for (int j = 0; j < AREA_WIDTH; ++j) {
            if (!tetris->field[i][j]) {
                line_fill = false;
                break;
            }
        }
        if (line_fill) {
            lines[line_idx++] = i;
        }
    }

    if (line_idx > 0) {
        int accum = 0;

        tetris_update_score(tetris, line_idx);

        while (line_idx > 0) {
            int line = lines[--line_idx] + accum;

            for (int i = line; i > 0; --i) {
                for (int j = 0; j < AREA_WIDTH; ++j) {
                    tetris->field[i][j] = tetris->field[i - 1][j];
                }
            }

            for (int j = 0; j < AREA_WIDTH; ++j) {
                tetris->field[0][j] = 0;
            }

            ++accum;
        }
    } else {
        tetris->score.combo = 0;
    }
}

void reset_mino_position(struct controller_t *c)
{
    c->tick = 0;
    c->rot = 0;
    c->touched_rot = 0;
    c->y = MINO_SPAWN_Y;
    c->x = MINO_SPAWN_X;
    c->soft = false;
    c->fix_delay = 0;
    c->drop_delay = 0;
}

void tetris_move_x(struct tetris_t *tetris, int x)
{
    if (check(tetris, x, 0)) {
        tetris->controller.x += x;
    }
}

void tetris_hard_drop(struct tetris_t *t)
{
    struct controller_t *c = &t->controller;
    c->touched_rot = c->rot;
    while (check(t, 0, 1)) {
        ++c->y;
        t->score.total += 2;
    }
    fix(t);
}

void tetris_soft_drop(struct tetris_t *tetris)
{
    tetris->controller.soft = true;
}

void tetris_tick(struct tetris_t *tetris)
{
    struct controller_t *c = &tetris->controller;
    if (c->soft || (++c->drop_delay > MAX_DROP_DELAY)) {
        if (++c->tick % (c->soft ? SOFT_DROP_INTERVAL : NORMAL_DROP_INTERVAL) ==
            0) {
            if (check(tetris, 0, 1)) {
                c->y += 1;
                if (c->soft) {
                    tetris->score.total += 1;
                }
            }
        }

        if (!check(tetris, 0, 1)) {
            if (c->fix_delay == 0) {
                c->touched_rot = c->rot;
            }
            if (++c->fix_delay > MAX_FIX_DELAY) {
                fix(tetris);
            }
        } else {
            c->fix_delay = 0;
        }
    }
}

static void next_mino(struct controller_t *c)
{
    c->mino = c->nexts[c->next_idx];
    c->nexts[c->next_idx] = rand() % MINO_KINDS;
    c->next_idx = (c->next_idx + 1) % MAX_NEXT;
    c->hold_lock = false;
    reset_mino_position(c);
}

static void fix(struct tetris_t *tetris)
{
    int i, j;
    iter_minos(tetris->controller.mino, tetris->controller.rot, i, j, {
        tetris->field[tetris->controller.y + i][tetris->controller.x + j] =
            tetris->controller.mino + 1;
    });

    normalize(tetris);

    next_mino(&tetris->controller);

    if (!check(tetris, 0, 0)) {
        tetris->gameover = true;
    }
}

static bool check(struct tetris_t *tetris, int offset_x, int offset_y)
{
    struct controller_t *c = &tetris->controller;

    int mino = MINO[c->mino][c->rot];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if ((mino >> ((3 - j) + (3 - i) * 4)) & 1) {
                if ((c->y + offset_y) + i >= AREA_HEIGHT) {
                    return false;
                }

                if ((c->x + offset_x) + j < 0) {
                    return false;
                }

                if ((c->x + offset_x) + j > AREA_WIDTH - 1) {
                    return false;
                }

                if (tetris->field[c->y + offset_y + i][c->x + offset_x + j]) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool tetris_check(struct tetris_t *tetris, int offset_x, int offset_y)
{
    return check(tetris, offset_x, offset_y);
}

void tetris_update_score(struct tetris_t *tetris, int lines)
{
    struct score_t *score = &tetris->score;

    bool tspin = is_tspin(tetris);
    bool b2b = ((score->current.lines == 4) || score->current.tspin) &&
               ((lines == 4) || tspin);
    score->current.tspin = tspin;
    score->current.lines = lines;
    score->current.b2b = b2b;
    score->lines += lines;

    uint64_t delta = 0;

    if (tspin) {
        switch (lines) {
        case 1:
            delta = 800;
            break;
        case 2:
            delta = 1200;
            break;
        case 3:
            delta = 1600;
            break;
        }
    } else {
        switch (lines) {
        case 1:
            delta = 100;
            break;
        case 2:
            delta = 300;
            break;
        case 3:
            delta = 500;
            break;
        case 4:
            delta = 800;
            break;
        }
    }

    if (b2b) {
        delta = (delta * 3) / 2;
    }

    score->combo += 1;
    delta += (score->combo - 1) * ((lines > 1) ? 50 : 20);
    delta *= score->level + 1;

    score->current.delta = delta;
    score->total += delta;
}

void kick(struct tetris_t *tetris, int rule[4][2][5][2], int clockwise,
          int prev_rot)
{
    struct controller_t *c = &tetris->controller;

    bool possible = false;

    for (int i = 0; i < 5; ++i) {
        int *offset = rule[c->rot][clockwise][i];
        if (check(tetris, offset[0], -offset[1])) {
            c->x += offset[0];
            c->y -= offset[1];
            possible = true;
            break;
        }
    }

    if (!possible) {
        c->rot = prev_rot;
    }
}

void tetris_hold(struct tetris_t *tetris)
{
    struct controller_t *c = &tetris->controller;

    if (!c->hold_lock) {
        c->hold_lock = true;
        if (c->hold >= 0) {
            swap(c->mino, c->hold);
            reset_mino_position(c);
        } else {
            c->hold = c->mino;
            next_mino(c);
        }
    }
}

void tetris_rotate(struct tetris_t *tetris, int clockwise)
{
    struct controller_t *c = &tetris->controller;

    // refs: https://tetris.fandom.com/wiki/SRS
    int prev_rot = c->rot;
    c->rot = (c->rot + (clockwise ? 1 : 3)) % 4;
    switch (c->mino) {
    case MINO_O:
        break;
    case MINO_I:
        kick(tetris,
             (int[4][2][5][2]){
                 {{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {1, 2}},    // 1 >> 0
                  {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}}, // 3 >> 0

                 {{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}},  // 2 >> 1
                  {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}}, // 0 >> 1

                 {{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}},  // 3 >> 2
                  {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}}, // 1 >> 2

                 {{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}},  // 0 >> 3
                  {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}}, // 2 >> 3
             },
             clockwise, prev_rot);
        break;
    default:
        kick(tetris,
             (int[4][2][5][2]){
                 {{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},     // 1 >> 0
                  {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}}, // 3 >> 0

                 {{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},  // 2 >> 1
                  {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}}, // 0 >> 1

                 {{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}, // 3 >> 2
                  {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}},   // 1 >> 2

                 {{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},  // 0 >> 3
                  {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}}, // 2 >> 3
             },
             clockwise, prev_rot);
        break;
    }
}

void tetris_init(struct tetris_t *tetris)
{
    memset(tetris, 0, sizeof(struct tetris_t));
    tetris->controller.x = MINO_SPAWN_X;
    tetris->controller.y = MINO_SPAWN_Y;
    tetris->controller.hold = -1;

    // user_score_delay = 0;
    for (int i = 0; i < MAX_NEXT; ++i) {
        tetris->controller.nexts[i] = rand() % MINO_KINDS;
    }
    tetris->controller.mino = tetris->controller.nexts[0];
}
