#include <ncurses.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "");

    int c;
    int y = 11, x = 11; // @coordinates
    char i;             // our symbol

    initscr(); // initial curses
    keypad(stdscr, 1);

    noecho();    // do not show the user input
    curs_set(0); // hide blinking cursor

    mvprintw(0, 6, "Chose any numeric or character key as your avatar by clicking them once. Then use arrows to move the avatar around\n");
    mvprintw(1, 11, "Press Esc to exit the game\n");
    i = getch(); // get symbol

    do
    {
        clear();
        mvprintw(0, 6, "Chose any numeric or character key as your avatar by clicking them once. Then use arrows to move the avatar\n");
        mvprintw(1, 11, "Press Esc to exit the game\n");
        if (c == KEY_UP)
            y--;
        if (c == KEY_DOWN)
            y++;
        if (c == KEY_LEFT)
            x--;
        if (c == KEY_RIGHT)
            x++;

        mvprintw(y, x, "🚶");
        // mvaddch(y, x, i); // be careful that this starts with y. put symbol at location (y,x)
    } while ((c = getch()) != 27); // 27 'Esc'

    // refresh();// needed when there is not a while loop. refresh the location
    // getch(); // needed when there is not a while loop so that the window will not refresh and immediately close -> you can then see the change
    endwin();

    return 0;
}
