#pragma once
#include "app.h"
#include "input.h"
#include "scene_controller.h"
#include "tetris.h"
#include "world.h"

struct root_t {
    struct world_t world;
    struct app_t app;
    struct tetris_t tetris;
    struct scene_controller_t scenes;
};

static inline void root_init(struct root_t *root)
{
    tetris_init(&root->tetris);
    app_init(&root->app);
    scene_controller_init(&root->scenes, &root->world, &root->app,
                          &root->tetris);
}
