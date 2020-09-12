#include "game_view.h"
#include "utils.h"

void prediction_view_update(struct scene_t *scene)
{
    struct prediction_view_t *s = container_of(typeof(*s), scene, base);
    struct tetris_t *tetris = s->tetris;

    for (int y = tetris->controller.y + 1; y < AREA_HEIGHT; ++y) {
        if (!tetris_check(tetris, 0, y - tetris->controller.y)) {
            int i, j;
            iter_minos(tetris->controller.mino, tetris->controller.rot, i, j, {
                mvprintw(AREA_OFFSET_Y + y -
                             (AREA_HEIGHT - AREA_VISIBLE_HEIGHT) + i - 1,
                         AREA_OFFSET_X + 1 + (tetris->controller.x + j) * 2,
                         "・");
            });
            break;
        }
    }
}

void prediction_view_init(struct prediction_view_t *view,
                          struct tetris_t *tetris)
{
    *view = (struct prediction_view_t){
        .base = {.update = prediction_view_update}, .tetris = tetris};
}

void user_view_update(struct scene_t *scene)
{
    struct user_view_t *s = container_of(typeof(*s), scene, base);
    struct controller_t *controller = s->controller;

    attron(COLOR_PAIR(controller->mino + 1) | A_BOLD);
    int i, j;
    iter_minos(controller->mino, controller->rot, i, j, {
        mvprintw(AREA_OFFSET_Y + controller->y -
                     (AREA_HEIGHT - AREA_VISIBLE_HEIGHT) + i,
                 AREA_OFFSET_X + 1 + (controller->x + j) * 2, "口");
    });
    attroff(COLOR_PAIR(controller->mino + 1) | A_BOLD);
}

void user_view_init(struct user_view_t *view, struct controller_t *controller)
{
    *view = (struct user_view_t){.base = {.update = user_view_update},
                                 .controller = controller};
}

void score_view_update(struct scene_t *scene)
{
    struct score_view_t *s = container_of(typeof(*s), scene, base);
    struct score_t *score = s->score;

    mvprintw(AREA_OFFSET_Y + AREA_VISIBLE_HEIGHT + 1, AREA_OFFSET_X,
             "SCORE %16llu", score->total);

    if (!s->app->paused) {
        if (s->last_seen_lines != score->lines) {
            s->score_delay = 0;
            s->last_seen_lines = score->lines;
        } else {
            ++s->score_delay;
        }
    }

    if ((score->current.delta > 0) && (s->score_delay < MAX_SCORE_DELAY)) {
        mvprintw(AREA_OFFSET_Y + AREA_VISIBLE_HEIGHT + 2, AREA_OFFSET_X + 5,
                 "%16llu+", score->current.delta);

        const char *notes[5];
        int num_notes = 0;
        if (score->current.tspin) {
            notes[num_notes++] = "T-Spin";
        }
        switch (score->current.lines) {
        case 1:
            notes[num_notes++] = "Single";
            break;
        case 2:
            notes[num_notes++] = "Double";
            break;
        case 3:
            notes[num_notes++] = "Triple";
            break;
        case 4:
            notes[num_notes++] = "Tetris";
            break;
        }

        if (score->current.b2b) {
            notes[num_notes++] = "B2B";
        }

        for (int i = 0; i < num_notes; ++i) {
            mvprintw(AREA_OFFSET_Y + SCORE_TEXT_OFFSET_Y + i,
                     SCORE_TEXT_OFFSET_X, notes[i]);
        }

        if (score->combo > 1) {
            mvprintw(AREA_OFFSET_Y + SCORE_TEXT_OFFSET_Y + num_notes,
                     SCORE_TEXT_OFFSET_X, "REN %lld", score->combo);
        }
    }
}

void score_view_init(struct score_view_t *view, struct app_t *app,
                     struct score_t *score)
{
    *view = (struct score_view_t){
        .base = {.update = score_view_update}, .app = app, .score = score};
}

void score_view_reset(struct score_view_t *score)
{
    score->last_seen_lines = 0;
    score->score_delay = 0;
}

void hold_view_update(struct scene_t *scene)
{
    struct hold_view_t *s = container_of(typeof(*s), scene, base);
    struct controller_t *controller = s->controller;

    mvprintw(AREA_OFFSET_Y, HOLD_OFFSET_X, "HOLD");

    if (controller->hold >= 0) {
        attron(COLOR_PAIR(controller->hold + 1) | A_BOLD);
        int i, j;
        iter_minos(controller->hold, 0, i, j, {
            mvprintw(AREA_OFFSET_Y + i + 2, HOLD_OFFSET_X + j * 2, "口");
        });
        attroff(COLOR_PAIR(controller->hold + 1) | A_BOLD);
    }
}

void hold_view_init(struct hold_view_t *view, struct controller_t *controller)
{
    *view = (struct hold_view_t){.base = {.update = hold_view_update},
                                 .controller = controller};
}

void nexts_view_update(struct scene_t *scene)
{
    struct nexts_view_t *s = container_of(typeof(*s), scene, base);
    struct controller_t *controller = s->controller;

    mvprintw(AREA_OFFSET_Y, AREA_OFFSET_X + AREA_WIDTH * 2 + 4, "NEXT");
    for (int ridx = 0; ridx < MAX_NEXT - 1; ++ridx) {
        int idx = (controller->next_idx + ridx) % MAX_NEXT;
        int next_mino = controller->nexts[idx];
        int i, j;

        attron(COLOR_PAIR(next_mino + 1) | A_BOLD);
        iter_minos(next_mino, 0, i, j, {
            mvprintw(AREA_OFFSET_Y + ridx * 3 + i + 2,
                     AREA_OFFSET_X + AREA_WIDTH * 2 + 4 + j * 2, "口");
        });
        attroff(COLOR_PAIR(next_mino + 1) | A_BOLD);
    }
}

void nexts_view_init(struct nexts_view_t *view, struct controller_t *controller)
{
    *view = (struct nexts_view_t){.base = {.update = nexts_view_update},
                                  .controller = controller};
}

void fixed_view_update(struct scene_t *scene)
{
    struct fixed_view_t *s = container_of(typeof(*s), scene, base);
    struct tetris_t *tetris = s->tetris;

    for (int i = 0; i < AREA_VISIBLE_HEIGHT; ++i) {
        for (int j = 0; j < AREA_WIDTH; ++j) {
            int color = tetris->field[i + AREA_HEIGHT - AREA_VISIBLE_HEIGHT][j];
            attron(COLOR_PAIR(color) | A_BOLD);
            if (color != 0) {
                mvprintw(AREA_OFFSET_Y + i, AREA_OFFSET_X + j * 2 + 1, "回");
            }
            attroff(COLOR_PAIR(color) | A_BOLD);
        }
    }
}

void fixed_view_init(struct fixed_view_t *view, struct tetris_t *tetris)
{
    *view = (struct fixed_view_t){.base = {.update = fixed_view_update},
                                  .tetris = tetris};
}

void game_view_update(struct scene_t *scene)
{
    struct game_view_t *view = container_of(typeof(*view), scene, base);

    view->outline->base.update(&view->outline->base);
    view->hold.base.update(&view->hold.base);
    view->fixed.base.update(&view->fixed.base);
    view->prediction.base.update(&view->prediction.base);
    view->user.base.update(&view->user.base);
    view->nexts.base.update(&view->nexts.base);
    view->score.base.update(&view->score.base);
}

// XXX: マウントした人にしかこれを使えないようにしたい
void game_view_reset(struct game_view_t *game)
{
    score_view_reset(&game->score);
}

void game_view_init(struct game_view_t *game, struct outline_view_t *outline,
                    struct app_t *app, struct tetris_t *tetris)
{
    *game = (struct game_view_t){.base = {.update = game_view_update},
                                 .presenter = {.tetris = tetris},
                                 .outline = outline};

    hold_view_init(&game->hold, &tetris->controller);
    nexts_view_init(&game->nexts, &tetris->controller);
    user_view_init(&game->user, &tetris->controller);
    prediction_view_init(&game->prediction, tetris);
    fixed_view_init(&game->fixed, tetris);
    score_view_init(&game->score, app, &tetris->score);
}
