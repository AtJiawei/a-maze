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

#define MAZE_COLS 7
#define MAZE_ROWS 7

#define MAZE_COL_WIDTH 2 // Width of each maze cell when printed

#define ESC_KEY 27
#define MSG_COL 15
#define MSG_ROW 11

typedef struct
{
    int x;
    int y;
} Player;

int calculateStartRow(int windowHeight)
{
    return (windowHeight - MAZE_ROWS) / 2;
}

int calculateStartCol(int windowWidth)
{
    return (windowWidth - MAZE_COLS * MAZE_COL_WIDTH) / 2;
}

bool reach_goal(Player player, const char *maze[MAZE_ROWS][MAZE_COLS])
{
    return maze[player.y][player.x] == GOAL; // same as if true return true else return false;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

// Returns min if value < min, max if value > max, and otherwise the value itself.
int clamp(int value, int min_, int max_)
{
    return min(max(value, min_), max_);
}

void updatePlayerPos(Player *player, int c, const char *maze[MAZE_ROWS][MAZE_COLS])
{
    int dx = (c == KEY_LEFT ? -1 : 0) + (c == KEY_RIGHT ? 1 : 0); // dx = delta x, the changing x
    int dy = (c == KEY_UP ? -1 : 0) + (c == KEY_DOWN ? 1 : 0);

    // check if the new x/y is within the grid and return the clamp value
    int x = clamp(player->x + dx, 0, MAZE_COLS - 1); // between (): x + the change of x
    int y = clamp(player->y + dy, 0, MAZE_ROWS - 1); // between (): y + the change of y

    if (maze[y][x] != WALL)
    {
        player->x = x;
        player->y = y;
    }
}

void printMaze(int startRow, int startCol, const char *maze[MAZE_ROWS][MAZE_COLS])
{
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            mvaddstr(startRow + i, startCol + j * MAZE_COL_WIDTH, maze[i][j]);
        }
    }
}

void printPlayer(int startRow, int startCol, Player player)
{
    mvprintw(startRow + player.y, startCol + player.x * MAZE_COL_WIDTH, PLAYER);
}

void play()
{
    const char *maze[MAZE_ROWS][MAZE_COLS] = {
        {WALL, WALL, WALL, WALL, WALL, WALL, WALL},
        {WALL, START, PATH, WALL, PATH, WALL, WALL},
        {WALL, WALL, PATH, WALL, PATH, PATH, WALL},
        {WALL, PATH, PATH, PATH, PATH, WALL, WALL},
        {WALL, PATH, WALL, WALL, PATH, PATH, WALL},
        {WALL, PATH, PATH, WALL, WALL, PATH, WALL},
        {WALL, WALL, WALL, WALL, WALL, GOAL, WALL}};

    // Player position.
    Player player = {
        .x = 1,
        .y = 1,
    };

    while (1)
    {
        // Get the terminal window size
        int windowHeight, windowWidth;
        getmaxyx(stdscr, windowHeight, windowWidth);

        // Calculate starting row and column for centering the maze
        int startRow = calculateStartRow(windowHeight);
        int startCol = calculateStartCol(windowWidth);
        
        // Render.
        clear();
        printMaze(startRow, startCol, maze);
        printPlayer(startRow, startCol, player);

        // Update state.
        int c = getch();
        if (c == ESC_KEY) // stop the game if player chooses to quit
        {
            mvprintw(MSG_COL, MSG_ROW, "Game Ended. You have chosen to quit the game by pressing Esc. Press any key to close the window ");
            break;
        }
        updatePlayerPos(&player, c, maze);

        // Check for victory.
        if (reach_goal(player, maze) == true)
        {
            clear();
            mvprintw(startRow + player.y, startCol + player.x * MAZE_COL_WIDTH, VICTORY);
            mvprintw(MSG_COL, MSG_ROW, "Congratulations! You have WON! Press any key to close the window");
            break;
        }
    }

    getch();
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

    play();

    endwin();

    return 0;
}
