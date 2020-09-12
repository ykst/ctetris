#include "player_scene.h"
#include "input.h"
#include "scene_controller.h"
#include "utils.h"

enum tetris_move {
    MOVE_LEFT = 1,
    MOVE_RIGHT,
    MOVE_DROP_SOFT,
    MOVE_DROP_HARD,
};

void player_presenter_reset(struct player_presenter_t *p)
{
    tetris_init(p->tetris);
}

void player_presenter_rotate(struct player_presenter_t *p, int clockwise)
{
    tetris_rotate(p->tetris, clockwise);
}

void player_presenter_hold(struct player_presenter_t *p)
{
    tetris_hold(p->tetris);
}

void player_presenter_move(struct player_presenter_t *p, enum tetris_move move)
{
    switch (move) {
    case MOVE_LEFT:
        tetris_move_x(p->tetris, -1);
        break;
    case MOVE_RIGHT:
        tetris_move_x(p->tetris, 1);
        break;
    case MOVE_DROP_SOFT:
        tetris_soft_drop(p->tetris);
        break;
    case MOVE_DROP_HARD:
        tetris_hard_drop(p->tetris);
        break;
    }
}

void player_presenter_drop(struct player_presenter_t *p, int is_hard)
{
    if (is_hard) {
        tetris_hard_drop(p->tetris);
    } else {
        tetris_soft_drop(p->tetris);
    }
}

void player_presenter_tick(struct player_presenter_t *p)
{
    tetris_tick(p->tetris);

    if (p->tetris->gameover) {
        p->delegate.gameover(p->delegate.context);
    }
}

static void player_enter(struct scene_t *scene,
                         enum transition_types transition,
                         enum scene_types from)
{
    struct player_scene_t *s = container_of(typeof(*s), scene, base);

    if (!s->app->playing) {
        player_presenter_reset(&s->presenter);
        game_view_reset(s->game);
        s->app->playing = true;
    }
}

static void player_update(struct scene_t *scene)
{
    struct player_scene_t *s = container_of(typeof(*s), scene, base);

    if (s->app->playing) {
        switch (s->input->ch) {
        case 'p':
            scene_controller_transit(s->scenes, SCENE_PAUSE);
            break;
        case 'z':
            player_presenter_rotate(&s->presenter, 0);
            break;
        case 'x':
            player_presenter_rotate(&s->presenter, 1);
            break;
        case 'c':
            player_presenter_hold(&s->presenter);
            break;
        case KEY_UP:
            player_presenter_move(&s->presenter, MOVE_DROP_HARD);
            break;
        case KEY_DOWN:
            player_presenter_move(&s->presenter, MOVE_DROP_SOFT);
            break;
        case KEY_LEFT:
            player_presenter_move(&s->presenter, MOVE_LEFT);
            break;
        case KEY_RIGHT:
            player_presenter_move(&s->presenter, MOVE_RIGHT);
            break;
        }
    }

    if (s->app->playing && !s->app->paused) {
        player_presenter_tick(&s->presenter);
    }

    s->game->base.update(&s->game->base);
}

static void player_scene_on_gameover(void *context)
{
    struct player_scene_t *s = context;

    s->app->playing = false;

    scene_controller_transit(s->scenes, SCENE_GAMEOVER);
}

void player_scene_init(struct player_scene_t *scene, struct input_t *input,
                       struct game_view_t *game,
                       struct scene_controller_t *scenes, struct app_t *app,
                       struct tetris_t *tetris)
{
    *scene = (struct player_scene_t){
        .base =
            {
                .update = player_update,
                .enter = player_enter,
            },
        .presenter = {.tetris = tetris,
                      .delegate = {.context = scene,
                                   .gameover = player_scene_on_gameover}},
        .scenes = scenes,
        .app = app,
        .input = input,
        .game = game};
}
