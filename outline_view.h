#pragma once
#include "view.h"

struct outline_view_t {
    struct scene_t base;
};

void outline_view_update(struct scene_t *scene);
void outline_view_init(struct outline_view_t *outline);
