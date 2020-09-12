#pragma once
#include "app.h"
#include "outline_view.h"
#include "tetris.h"
#include "view.h"

struct score_view_t {
    struct scene_t base;
    struct score_t *score;
    struct app_t *app;
    uint64_t last_seen_lines;
    int score_delay;
};

struct fixed_view_t {
    struct scene_t base;
    struct tetris_t *tetris;
};

struct user_view_t {
    struct scene_t base;
    struct controller_t *controller;
};

struct nexts_view_t {
    struct scene_t base;
    struct controller_t *controller;
};

struct hold_view_t {
    struct scene_t base;
    struct controller_t *controller;
};

struct prediction_view_t {
    struct scene_t base;
    struct tetris_t *tetris;
};

struct game_presenter_t {
    struct tetris_t *tetris;
};

struct game_view_t {
    struct scene_t base;
    struct game_presenter_t presenter;

    struct outline_view_t *outline;
    struct hold_view_t hold;
    struct fixed_view_t fixed;
    struct prediction_view_t prediction;
    struct user_view_t user;
    struct nexts_view_t nexts;
    struct score_view_t score;
};

void game_view_reset(struct game_view_t *game);
void game_view_init(struct game_view_t *game, struct outline_view_t *outline,
                    struct app_t *app, struct tetris_t *tetris);
