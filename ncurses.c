#include <ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>

const char *PATH = "   "; // "🔘";
const char *GOAL = "🚩";
const char *PLAYER = "🚶";
const char *WALL = "🌲";
const char *START = "📌";

const int MAZE_ROWS = 7;
const int MAZE_COLS = 7;

void printMaze(const char *maze[MAZE_ROWS][MAZE_COLS])
{
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            // mvprintw((i + 4), (j + 3), "%s ", maze[i][j]);
            mvaddstr(4 + i, 3 + j * 2, maze[i][j]);
        }
    }
}
// https://stackoverflow.com/questions/61690230/chinese-character-too-large-for-enclosing-character-literal-type
// https://man7.org/linux/man-pages/man3/wcwidth.3.html

int main()
{
    // Set the locale to the user preferred locale. The default is a minimal locale.
    setlocale(LC_ALL, "");

    // Ensure that all characters we use to print the maze are indeed of printing width 2.
    assert(wcwidth(U'🔘') == 2);
    assert(wcwidth(U'🚩') == 2);
    assert(wcwidth(U'🚶') == 2);
    assert(wcwidth(U'🌲') == 2);
    assert(wcwidth(U'📌') == 2);

    // Player position.
    int y = 1, x = 1;

    // Initialize the screen buffer for ncurses.
    initscr();
    keypad(stdscr, 1);

    noecho();    // do not show the user input
    curs_set(0); // hide blinking cursor

    const char *maze[MAZE_ROWS][MAZE_COLS] =
        {
            {WALL, WALL, WALL, WALL, WALL, WALL, WALL},
            {WALL, START, PATH, WALL, PATH, WALL, WALL},
            {WALL, WALL, PATH, WALL, PATH, PATH, WALL},
            {WALL, PATH, PATH, PATH, PATH, WALL, WALL},
            {WALL, PATH, WALL, WALL, PATH, PATH, WALL},
            {WALL, PATH, PATH, WALL, WALL, PATH, WALL},
            {WALL, WALL, WALL, WALL, WALL, GOAL, WALL}};

    while (true)
    {
        // Draw.
        clear();
        printMaze(maze);
        mvprintw(4 + y, 3 + x * 2, "🚶");

        // Process input.
        int c = getch();

        if (c == 27)
            break;
        if (c == KEY_UP)
            y--;
        if (c == KEY_DOWN)
            y++;
        if (c == KEY_LEFT)
            x--;
        if (c == KEY_RIGHT)
            x++;
    }

    endwin();

    return 0;
}
