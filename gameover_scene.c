#include "gameover_scene.h"
#include "input.h"
#include "scene_controller.h"
#include "utils.h"

static void gameover_update(struct scene_t *scene)
{
    struct gameover_scene_t *s = container_of(typeof(*s), scene, base);

    if (s->input->ch != ERR) {
        scene_controller_transit(s->scenes, SCENE_START);
    }

    s->game->base.update(&s->game->base);
    mvprintw(AREA_OFFSET_Y - 1, AREA_OFFSET_X + 7, "GAMEOVER");
}

void gameover_scene_init(struct gameover_scene_t *scene, struct input_t *input,
                         struct scene_controller_t *scenes,
                         struct game_view_t *game)
{
    *scene = (struct gameover_scene_t){.base =
                                           {
                                               .update = gameover_update,
                                           },
                                       .scenes = scenes,
                                       .game = game,
                                       .input = input};
}
