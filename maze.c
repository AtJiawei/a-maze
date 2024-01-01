#include <assert.h>
#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>
#include <wchar.h>
#include "lib.h"

// define the global variables
const char *PLAYER = "🚶";
const char *VICTORY = "😃";

#define ESC_KEY 27
#define MSG_COL 15
#define MSG_ROW 11
#define STP_COL 15
#define STP_ROW 14

#define MAZE_COL_WIDTH 2 // Width of each maze cell when printed

typedef struct Vector2
{
    int x;
    int y;
} Vector2;

// A maze is defined as a grid of cells.
typedef enum Cell
{
    CELL_PATH = 0,
    CELL_WALL = 1,
    CELL_START = 2,
    CELL_GOAL = 3,
} Cell;

typedef struct Maze
{
    Cell *cells;
    Vector2 dims;
} Maze;

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

Vector2 calc_start(Vector2 win_dims, Vector2 maze_dims)
{
    Vector2 offset = {
        .x = (win_dims.x - maze_dims.x * MAZE_COL_WIDTH) / 2,
        .y = (win_dims.y - maze_dims.y) / 2,
    };
    return offset;
}

bool reach_goal(Player player, Maze maze)
{
    return maze.cells[player.y * maze.dims.x + player.x] == CELL_GOAL;
}

void update_player_pos(Player *player, int c, Maze maze)
{
    int dx = (c == KEY_LEFT ? -1 : 0) + (c == KEY_RIGHT ? 1 : 0); // dx = delta x, the changing x
    int dy = (c == KEY_UP ? -1 : 0) + (c == KEY_DOWN ? 1 : 0);

    // check if the new x/y is within the grid and return the clamp value
    int x = clamp(player->x + dx, 0, maze.dims.x - 1); // between (): x + the change of x
    int y = clamp(player->y + dy, 0, maze.dims.y - 1); // between (): y + the change of y

    if (maze.cells[y * maze.dims.x + x] != CELL_WALL)
    {
        player->x = x;
        player->y = y;
    }
}

void print_maze(Vector2 start, Maze maze)
{
    for (int y = 0; y < maze.dims.y; y++)
    {
        for (int x = 0; x < maze.dims.x; x++)
        {
            const char *str = cell_to_str(maze.cells[y * maze.dims.x + x]);
            mvaddstr(start.y + y, start.x + x * MAZE_COL_WIDTH, str);
        }
    }
}

void print_player(Vector2 start, Player player)
{
    mvaddstr(start.y + player.y, start.x + player.x * MAZE_COL_WIDTH, PLAYER);
}

// Cell

Maze alloc_maze(Vector2 dims)
{
    Cell *cells = malloc(sizeof(Cell) * dims.x * dims.y);
    Maze maze = {
        .cells = cells,
        .dims = dims,
    };
    return maze;
}

void fill_maze(Maze *maze)
{
    assert(maze->dims.x > 2);
    assert(maze->dims.y > 2);

    for (int y = 0; y < maze->dims.y; y++)
    {
        for (int x = 0; x < maze->dims.x; x++)
        {
            Cell cell = (y == 0 || y == maze->dims.y - 1 || x == 0 || x == maze->dims.x - 1) ? CELL_WALL : CELL_PATH;
            maze->cells[y * maze->dims.x + x] = cell;
        }
    }

    maze->cells[1 * maze->dims.x + 1] = CELL_START;
    maze->cells[(maze->dims.y - 2) * maze->dims.x + (maze->dims.x - 2)] = CELL_GOAL;
}

void play()
{
    Vector2 dims = {
        .x = 10,
        .y = 6,
    };

    Maze maze = alloc_maze(dims);

    fill_maze(&maze);

    // Player position.
    Player player = {
        .x = 1,
        .y = 1,
    };

    int counter = 0;

    while (1)
    {
        // Get the terminal window size
        Vector2 win_dims;
        getmaxyx(stdscr, win_dims.y, win_dims.x);

        // Calculate starting row and column for centering the maze
        Vector2 start = calc_start(win_dims, maze.dims);

        // Render.
        clear();
        print_maze(start, maze);
        print_player(start, player);
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
            mvaddstr(start.y + player.y, start.x + player.x * MAZE_COL_WIDTH, VICTORY);
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
