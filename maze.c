// this version is to add ncurses.h library. reordered the headers. reordered the global variables
#include <assert.h>
#include <locale.h>
#include <ncurses.h>
#include <wchar.h>

// define the global variables
const char *GOAL = "🚩";
const char *START = "📌";
const char *PATH = "  ";
const char *PLAYER = "🚶";
const char *WALL = "🌲";
const char *VICTORY = "😃";

#define MAZE_ROWS 7
#define MAZE_COLS 7

#define MAZE_START_ROW 4
#define MAZE_START_COL 3
#define MAZE_COL_WIDTH 2 // Width of each maze cell when printed

#define MSG_COL 15
#define MSG_ROW 11
#define ESC_KEY 27

typedef struct
{
    int x;
    int y;
} Player;

bool reach_goal(Player player, const char *maze[MAZE_ROWS][MAZE_COLS])
{
    return maze[player.y][player.x] == GOAL; // same as if true return true else return false; 
}

bool updatePlayerPos(Player *player, int c, const char *maze[MAZE_ROWS][MAZE_COLS])
{
    Player new_player = *player;

    // Process input.
    if (c == KEY_UP)
        new_player.y--;
    if (c == KEY_DOWN)
        new_player.y++;
    if (c == KEY_LEFT)
        new_player.x--;
    if (c == KEY_RIGHT)
        new_player.x++;

    if (new_player.x >= 0 && new_player.x < MAZE_ROWS && new_player.y >= 0 && new_player.y < MAZE_COLS && maze[new_player.y][new_player.x] != WALL)
    {
        *player = new_player;

        if (reach_goal(*player, maze) == true)
        {
            return true; // Player reached goal
        }
    }
    return false;
}

void printMaze(const char *maze[MAZE_ROWS][MAZE_COLS])
{
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            mvaddstr(MAZE_START_ROW + i, MAZE_START_COL + j * MAZE_COL_WIDTH, maze[i][j]);
        }
    }
}

void printPlayer(Player player)
{
    mvprintw(MAZE_START_ROW + player.y, MAZE_START_COL + player.x * MAZE_COL_WIDTH, PLAYER);
}

void play(const char *maze[MAZE_ROWS][MAZE_COLS])
{
    // Player position.
    Player player = {
        .x = 1,
        .y = 1,
    };

    int c;
    while (1)
    {
        clear();
        printMaze(maze);
        printPlayer(player);

        c = getch();

        if (c == ESC_KEY) // stop the game if player chooses to quit
        {
            mvprintw(MSG_COL, MSG_ROW, "|| Game Ended. You have chosen to quit the game by pressing Esc. Press any key to close the window ");
            break;
        }
        else if (c == KEY_UP || c == KEY_DOWN || c == KEY_LEFT || c == KEY_RIGHT)
        {
            if (updatePlayerPos(&player, c, maze))
            {
                clear();
                mvprintw(MAZE_START_ROW + player.y, MAZE_START_COL + player.x * MAZE_COL_WIDTH, VICTORY);
                mvprintw(MSG_COL, MSG_ROW, "|| Congratulations! You have WON! Press any key to close the window");
                break;
            }
        }
    }
}

int main()
{
    // Initialize the maze
    setlocale(LC_ALL, "");

    // Ensure that all characters we use to print the maze are indeed of printing width 2.
    assert(wcwidth(U'🔘') == MAZE_COL_WIDTH);
    assert(wcwidth(U'🚩') == MAZE_COL_WIDTH);
    assert(wcwidth(U'🚶') == MAZE_COL_WIDTH);
    assert(wcwidth(U'🌲') == MAZE_COL_WIDTH);
    assert(wcwidth(U'📌') == MAZE_COL_WIDTH);

    // Initialize the screen buffer for ncurses.
    if (initscr() == NULL)
    {
        fprintf(stderr, "Error initializing ncurses.\n");
        return 1;
    }
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
    play(maze);
    refresh();
    getch();
    endwin();
    return 0;
}
