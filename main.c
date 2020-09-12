#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "list.h"
#include "master.h"
#include "tetris.h"
#include "utils.h"
#include "view.h"

#include "root.h"

void init(void)
{
    srand(time(NULL));
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    for (int i = 0; i < MINO_KINDS; ++i) {
        init_pair(i + 1, MINO_COLORS[i], COLOR_BLACK);
    }
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    noecho();

    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

void run(void)
{
    struct root_t root;
    root_init(&root);

    while (true) {
        usleep(16600);

        int ch = getch();

        if (ch == 'q') {
            break;
        }

        root.world.input.ch = ch;

        erase();
        scene_controller_update(&root.scenes, 1);
        refresh();
    }
}

void finalize(void)
{
    clear();
    endwin();
}

int main(void)
{
    init();
    run();
    finalize();
}
