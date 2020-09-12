#pragma once

#include "app.h"
#include "game_view.h"
#include "tetris.h"
#include "view.h"

struct pause_scene_t {
    struct scene_t base;
    struct game_view_t *game;
    struct app_t *app;
    struct input_t *input;
    struct scene_controller_t *scenes;
};

void pause_scene_init(struct pause_scene_t *scene, struct input_t *input,
                      struct game_view_t *game,
                      struct scene_controller_t *scenes, struct app_t *app);
