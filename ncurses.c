#include <ncurses.h>

int main()
{
    initscr();
    printw("hello, world");
    getchar();
    refresh();
    endwin();

    return 0;
}
