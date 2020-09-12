#include "scene_controller.h"
#include "root.h"
#include "utils.h"
#include <curses.h>
#include <string.h>

void scene_controller_init(struct scene_controller_t *c, struct world_t *world,
                           struct app_t *app, struct tetris_t *tetris)
{
    memset(c, 0, sizeof(*c));

    c->current = SCENE_START;
    c->lut[SCENE_START] = &c->start.base;
    c->lut[SCENE_GAMEOVER] = &c->gameover.base;
    c->lut[SCENE_PAUSE] = &c->pause.base;
    c->lut[SCENE_INGAME] = &c->player.base;

    outline_view_init(&c->outline);
    game_view_init(&c->game, &c->outline, app, tetris);
    title_scene_init(&c->start, &world->input, c, &c->outline);
    pause_scene_init(&c->pause, &world->input, &c->game, c, app);
    player_scene_init(&c->player, &world->input, &c->game, c, app, tetris);
    gameover_scene_init(&c->gameover, &world->input, c, &c->game);
}

void scene_controller_transit(scene_controller_handle h, enum scene_types to)
{
    enum scene_types from = h->current;
    h->current = to;
    struct scene_t *next = h->lut[to];

    if (next->enter != NULL) {
        next->enter(next, TRANSITION_REPLACE, from);
    }
}

void scene_controller_update(struct scene_controller_t *s, int delta)
{
    struct scene_t *scene = s->lut[s->current];

    if (scene->update != NULL) {
        scene->update(scene);
    }
}
