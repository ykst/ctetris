#pragma once
#include "view.h"

struct gameover_scene_t {
    struct scene_t base;
    struct scene_controller_t *scenes;
    struct game_view_t *game;
    struct input_t *input;
};

void gameover_scene_init(struct gameover_scene_t *scene, struct input_t *input,
                         struct scene_controller_t *scenes,
                         struct game_view_t *game);
