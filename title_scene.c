#include "title_scene.h"
#include "scene_controller.h"
#include "utils.h"

void title_presenter_transit(struct title_presenter_t *p)
{
    p->delegate.transit(p->delegate.context);
}

static void title_update(struct scene_t *scene)
{
    struct title_scene_t *s = container_of(typeof(*s), scene, base);

    if (s->input->ch != ERR) {
        title_presenter_transit(&s->presenter);
    }

    s->outline->base.update(&s->outline->base);

    mvprintw(AREA_OFFSET_Y + AREA_VISIBLE_HEIGHT / 2, AREA_OFFSET_X + 5,
             "PRESS ANY KEY");
}

void title_presenter_init(struct title_presenter_t *p)
{
    *p = (struct title_presenter_t){.selected_menu = 0};
}

void title_on_transit(void *context)
{
    struct title_scene_t *s = context;

    scene_controller_transit(s->scenes, SCENE_INGAME);
}

void title_scene_init(struct title_scene_t *scene, struct input_t *input,
                      struct scene_controller_t *scenes,
                      struct outline_view_t *outline)
{
    *scene = (struct title_scene_t){
        .base =
            {
                .update = title_update,
            },
        .scenes = scenes,
        .outline = outline,
        .input = input,
        .presenter = {
            .delegate = {.context = scene, .transit = title_on_transit}}};
}
