#pragma once
#include "input.h"
#include "view.h"

struct player_presenter_t {
    struct tetris_t *tetris;
    struct {
        void *context;
        void (*gameover)(void *context);
    } delegate;
};

struct player_scene_t {
    struct scene_t base;
    struct player_presenter_t presenter;
    struct game_view_t *game;
    struct app_t *app;
    struct input_t *input;
    struct scene_controller_t *scenes;
};

void player_scene_init(struct player_scene_t *scene, struct input_t *input,
                       struct game_view_t *game,
                       struct scene_controller_t *scenes, struct app_t *app,
                       struct tetris_t *tetris);
