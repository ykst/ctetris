#include "pause_scene.h"
#include "scene_controller.h"
#include "utils.h"

static void pause_enter(struct scene_t *scene, enum transition_types transition,
                        enum scene_types from)
{
    struct pause_scene_t *s = container_of(typeof(*s), scene, base);
    s->app->paused = true;
}

static void pause_update(struct scene_t *scene)
{
    struct pause_scene_t *s = container_of(typeof(*s), scene, base);

    if (s->input->ch == 'p') {
        s->app->paused = false;
        scene_controller_transit(s->scenes, SCENE_INGAME);
    }

    s->game->base.update(&s->game->base);
    mvprintw(AREA_OFFSET_Y - 1, AREA_OFFSET_X + 8, "PAUSED");
}

void pause_scene_init(struct pause_scene_t *scene, struct input_t *input,
                      struct game_view_t *game,
                      struct scene_controller_t *scenes, struct app_t *app)
{
    *scene = (struct pause_scene_t){.base =
                                        {
                                            .update = pause_update,
                                            .enter = pause_enter,
                                        },

                                    .game = game,
                                    .app = app,
                                    .scenes = scenes,
                                    .input = input};
}
