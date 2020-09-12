#pragma once
#include "view.h"

struct title_presenter_t {
    int selected_menu;
    struct {
        void *context;
        void (*transit)(void *context);
    } delegate;
};

struct title_scene_t {
    struct scene_t base;
    struct title_presenter_t presenter;
    struct outline_view_t *outline;
    struct input_t *input;
    struct scene_controller_t *scenes;
};

void title_scene_init(struct title_scene_t *scene, struct input_t *input,
                      struct scene_controller_t *scenes,
                      struct outline_view_t *outline);
