#include <assert.h>
#include <locale.h>
#include <ncurses.h>
#include <wchar.h>
#include "lib.h"

// define the global variables
const char *PLAYER = "🚶";
const char *VICTORY = "😃";

#define ESC_KEY 27
#define MAZE_COLS 7
#define MAZE_ROWS 7
#define MSG_COL 15
#define MSG_ROW 11
#define STP_COL 15
#define STP_ROW 14

#define MAZE_COL_WIDTH 2 // Width of each maze cell when printed

// A maze is defined as a grid of cells.
typedef enum Cell
{
    CELL_PATH = 0,
    CELL_WALL = 1,
    CELL_START = 2,
    CELL_GOAL = 3,
} Cell;

const char *cell_to_str(Cell cell)
{
    switch (cell)
    {
    case CELL_PATH:
        return "  ";
    case CELL_WALL:
        return "🌲";
    case CELL_START:
        return "📌";
    case CELL_GOAL:
        return "🚩";
    default:
        assert(false);
    }
}

typedef struct Player
{
    int x;
    int y;
} Player;

int calc_start_row(int win_rows)
{
    return (win_rows - MAZE_ROWS) / 2;
}

int calc_start_col(int win_cols)
{
    return (win_cols - MAZE_COLS * MAZE_COL_WIDTH) / 2;
}

bool reach_goal(Player player, Cell maze[MAZE_ROWS][MAZE_COLS])
{
    return maze[player.y][player.x] == CELL_GOAL;
}

void update_player_pos(Player *player, int c, Cell maze[MAZE_ROWS][MAZE_COLS])
{
    int dx = (c == KEY_LEFT ? -1 : 0) + (c == KEY_RIGHT ? 1 : 0); // dx = delta x, the changing x
    int dy = (c == KEY_UP ? -1 : 0) + (c == KEY_DOWN ? 1 : 0);

    // check if the new x/y is within the grid and return the clamp value
    int x = clamp(player->x + dx, 0, MAZE_COLS - 1); // between (): x + the change of x
    int y = clamp(player->y + dy, 0, MAZE_ROWS - 1); // between (): y + the change of y

    if (maze[y][x] != CELL_WALL)
    {
        player->x = x;
        player->y = y;
    }
}

void print_maze(int start_row, int start_col, Cell maze[MAZE_ROWS][MAZE_COLS])
{
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            const char *str = cell_to_str(maze[i][j]);
            mvaddstr(start_row + i, start_col + j * MAZE_COL_WIDTH, str);
        }
    }
}

void print_player(int start_row, int start_col, Player player)
{
    mvaddstr(start_row + player.y, start_col + player.x * MAZE_COL_WIDTH, PLAYER);
}

void play()
{
    Cell maze[MAZE_ROWS][MAZE_COLS] = {
        {CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL},
        {CELL_WALL, CELL_START, CELL_PATH, CELL_WALL, CELL_PATH, CELL_WALL, CELL_WALL},
        {CELL_WALL, CELL_WALL, CELL_PATH, CELL_WALL, CELL_PATH, CELL_PATH, CELL_WALL},
        {CELL_WALL, CELL_PATH, CELL_PATH, CELL_PATH, CELL_PATH, CELL_WALL, CELL_WALL},
        {CELL_WALL, CELL_PATH, CELL_WALL, CELL_WALL, CELL_PATH, CELL_PATH, CELL_WALL},
        {CELL_WALL, CELL_PATH, CELL_PATH, CELL_WALL, CELL_WALL, CELL_PATH, CELL_WALL},
        {CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_GOAL, CELL_WALL}};

    // Player position.
    Player player = {
        .x = 1,
        .y = 1,
    };

    int counter = 0;

    while (1)
    {
        // Get the terminal window size
        int win_rows, win_cols;
        getmaxyx(stdscr, win_rows, win_cols);

        // Calculate starting row and column for centering the maze
        int start_row = calc_start_row(win_rows);
        int start_col = calc_start_col(win_cols);

        // Render.
        clear();
        print_maze(start_row, start_col, maze);
        print_player(start_row, start_col, player);
        mvprintw(STP_COL, STP_COL, "Total Steps Taken: %i", counter);

        // Update state.
        int c = getch();
        if (c == 'q') // stop the game if player chooses to quit
        {
            clear();
            mvprintw(MSG_COL, MSG_ROW, "Game Ended. You have chosen to quit the game by pressing Esc. This window will be closed automatically in 5 seconds. To quit immediately, press any key to close the window ");
            break;
        }
        else
        {
            counter++;
            update_player_pos(&player, c, maze);
        }

        // Check for victory.
        if (reach_goal(player, maze) == true)
        {
            clear();
            mvaddstr(start_row + player.y, start_col + player.x * MAZE_COL_WIDTH, VICTORY);
            mvprintw(MSG_COL, MSG_ROW, "Congratulations! You have WON! Game Ended! Total Steps Taken: %i. This window will be closed automatically in 5 seconds. To quit immediately,press any key to close the window.", counter);
            break;
        }
    }

    // Initial unskippable delay.
    refresh();   // Ensure that the message is displayed before the delay
    napms(1000); // Sleep for 1 second (1000 milliseconds).
    flushinp();  // Flush any input received during the sleep.

    // Second skippable delay (by pressing a character).
    timeout(4000);
    getch();
    timeout(-1);
}

int main()
{
    // Initialize the maze
    setlocale(LC_ALL, "");

    // Ensure that all characters we use to print the maze are indeed of printing width 2.
    assert(wcwidth(U'😃') == MAZE_COL_WIDTH);
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
