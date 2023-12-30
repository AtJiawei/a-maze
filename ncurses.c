#include <ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>
// #include <stdio.h>
#include <time.h>

const char *PATH = "  ";
const char *GOAL = "🚩";
const char *PLAYER = "🚶";
const char *PLAYER2 = "🏃";

const char *WALL = "🌲";
const char *START = "📌";

#define MAZE_ROWS 7
#define MAZE_COLS 7

void printMaze(const char *maze[MAZE_ROWS][MAZE_COLS])
{
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            mvaddstr(4 + i, 3 + j * 2, maze[i][j]);
        }
    }
}
// https://stackoverflow.com/questions/61690230/chinese-character-too-large-for-enclosing-character-literal-type
// https://man7.org/linux/man-pages/man3/wcwidth.3.html

typedef struct Point2I
{
    int x;
    int y;
} Point2I;

typedef struct Player {
    Point2I pos;
    int anim;
} Player;

void render(const char *maze[MAZE_ROWS][MAZE_COLS], Player player)
{
    clear();

    // Print time.
    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);
    mvprintw(1, 1, "%s", asctime(timeinfo));

    printMaze(maze);
    const char * graphic = player.anim < 30 ? PLAYER : PLAYER2;
    mvaddstr(4 + player.pos.y, 3 + player.pos.x * 2, graphic);

    return 0;
}

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
    Player player = {
        .pos = {
            .x = 1,
            .y = 1,
        },
        .anim = 0,
    };

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
        render(maze, player);

        // Process input.
        timeout(16);
        int c = getch();

        if (c == 27)
            break;
        if (c == KEY_UP)
            player.pos.y--;
        if (c == KEY_DOWN)
            player.pos.y++;
        if (c == KEY_LEFT)
            player.pos.x--;
        if (c == KEY_RIGHT)
            player.pos.x++;

        player.anim = (player.anim + 1) % 60;
    }

    endwin();

    return 0;
}
