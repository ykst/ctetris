#pragma once

#include "game_view.h"
#include "gameover_scene.h"
#include "list.h"
#include "pause_scene.h"
#include "player_scene.h"
#include "title_scene.h"
#include "view.h"
#include "world.h"

struct scene_controller_t {
    enum scene_types current;
    struct scene_t *lut[SCENE_KINDS];
    struct gameover_scene_t gameover;
    struct pause_scene_t pause;
    struct title_scene_t start;
    struct player_scene_t player;
    struct outline_view_t outline;
    struct game_view_t game;
};

typedef struct scene_controller_t *scene_controller_handle;

void scene_controller_init(scene_controller_handle h, struct world_t *world,
                           struct app_t *app, struct tetris_t *tetris);

void scene_controller_transit(scene_controller_handle h, enum scene_types to);
void scene_controller_update(struct scene_controller_t *s, int delta);
