#include "outline_view.h"
#include "tetris.h"

void outline_view_update(struct scene_t *scene)
{
    for (int i = 0; i < (AREA_WIDTH + 1) * 2; ++i) {
        mvprintw(AREA_OFFSET_Y - 1, AREA_OFFSET_X + i, ".");
    }

    for (int i = 0; i < AREA_VISIBLE_HEIGHT + 1; ++i) {
        const char chr = (i == AREA_VISIBLE_HEIGHT) ? '+' : '|';
        mvaddch(AREA_OFFSET_Y + i, AREA_OFFSET_X, chr);
        mvaddch(AREA_OFFSET_Y + i, AREA_OFFSET_X + AREA_WIDTH * 2 + 1, chr);
    }

    for (int i = 0; i < AREA_WIDTH; ++i) {
        mvprintw(AREA_OFFSET_Y + AREA_VISIBLE_HEIGHT, AREA_OFFSET_X + i + 1,
                 "ー");
    }
}

void outline_view_init(struct outline_view_t *outline)
{
    *outline = (struct outline_view_t){.base = {
                                           .update = outline_view_update,
                                       }};
}
