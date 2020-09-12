#pragma once
#include <stdbool.h>

struct app_t {
    bool playing;
    bool paused;
};

void app_init(struct app_t *app);
