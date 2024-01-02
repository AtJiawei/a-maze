#include <assert.h>
#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>
#include <wchar.h>
#include <stdio.h>
#include <time.h>

#include "lib.h"

// define the global variables
const char *PLAYER = "🚶";
const char *VICTORY = "😃";

#define ESC_KEY 27
#define MSG_COL 15
#define MSG_ROW 11

#define MAZE_COL_WIDTH 2 // Width of each maze cell when printed

typedef struct Vector2
{
    int x;
    int y;
} Vector2;

Vector2 vector2(int x, int y)
{
    return (Vector2){.x = x, .y = y};
}

Vector2 vector2_sub(Vector2 a, Vector2 b)
{
    return vector2(a.x - b.x, a.y - b.y);
}

bool vector2_eq(Vector2 a, Vector2 b)
{
    return a.x == b.x && a.y == b.y;
}

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

// Generate a random number between 0 (inclusive) and high (exclusive).
int r1(int high)
{
    int r;
    do
    {
        r = rand();
    } while (r > (RAND_MAX - (RAND_MAX % high)));
    return r % high;
}

// Generate a random number between low (inclusive) and high (exclusive).
int r2(int low, int high)
{
    assert(low < high);
    return low + r1(high - low);
}

// Remaps Cell
typedef enum CellWilsons
{
    CELL_WILSONS_PATH = CELL_PATH,
    CELL_WILSONS_WALL = CELL_WALL,
    CELL_WILSONS_UNDEFINED = CELL_START,
    CELL_WILSONS_RANDOM_WALK = CELL_GOAL,
} CellWilsons;

int sub_idx(Vector2 dims, Vector2 idx)
{
    return (idx.y * 2 + 1) * dims.x + (idx.x * 2 + 1);
}

void print_maze_wilsons(Maze *maze, Vector2 *path, int path_len)
{
    // Get the terminal window size
    Vector2 win_dims;
    getmaxyx(stdscr, win_dims.y, win_dims.x);

    // Calculate starting row and column for centering the maze
    Vector2 start = calc_start(win_dims, maze->dims);

    for (int y = 0; y < maze->dims.y; y++)
    {
        for (int x = 0; x < maze->dims.x; x++)
        {
            const char *str;
            switch (maze->cells[y * maze->dims.x + x])
            {
            case CELL_WILSONS_PATH:
            case CELL_WILSONS_RANDOM_WALK:
                str = "  ";
                break;
            case CELL_WILSONS_WALL:
            case CELL_WILSONS_UNDEFINED:
                str = "🌲";
                break;
            default:
                assert(false);
            }
            mvaddstr(start.y + y, start.x + x * MAZE_COL_WIDTH, str);
        }
    }

    if (path_len > 0)
    {
        Vector2 last = path[0];
        mvaddstr(start.y + (last.y * 2 + 1), start.x + (last.x * 2 + 1) * MAZE_COL_WIDTH, "📌");

        // Delete walls between random walk path.
        for (int i = 1; i < path_len; i++)
        {
            Vector2 curr = path[i];
            Vector2 d = vector2_sub(curr, last);
            mvaddstr(start.y + (last.y * 2 + 1 + d.y), start.x + (last.x * 2 + 1 + d.x) * MAZE_COL_WIDTH, "  ");
            last = curr;
        }

        if (path_len > 1)
        {
            mvaddstr(start.y + (last.y * 2 + 1), start.x + (last.x * 2 + 1) * MAZE_COL_WIDTH, "🚩");
        }
    }
}

int non_blocking_getch()
{
    timeout(0);
    int c = getch();
    timeout(-1);
    return c;
}

void animate(bool *should_anim, int *anim_delay, int anim_mult, Maze *maze, Vector2 *path, int path_len)
{
    if (!*should_anim)
        return;

    clear();

    mvprintw(0, 0, "Animating maze generation at %2.1f steps per second", 1000.0 / (float) *anim_delay);
    mvaddstr(1, 0, "  Press q to skip animation");
    mvaddstr(2, 0, "  Press ⬆️ to speed up animation");
    mvaddstr(3, 0, "  Press ⬇️ to slow down animation");

    print_maze_wilsons(maze, path, path_len);
    refresh();
    napms(*anim_delay * anim_mult);

    int c;
    while ((c = non_blocking_getch()) != ERR)
    {
        switch (c)
        {
        case 'q':
            *should_anim = false;
            break;
        case KEY_UP:
            *anim_delay = max(1, *anim_delay / 2);
            break;
        case KEY_DOWN:
            *anim_delay = min(512, *anim_delay * 2);
            break;
        }
    }
}

void generate_maze_wilsons(Maze *maze)
{
    bool should_anim = true;
    int anim_delay = 128;

    Vector2 dims = maze->dims;

    assert(dims.x >= 3);
    assert(dims.y >= 3);
    assert(dims.x % 2 == 1);
    assert(dims.y % 2 == 1);

    Vector2 sub_dims = {
        .x = (dims.x - 1) / 2,
        .y = (dims.y - 1) / 2,
    };

    // Safety: CellWilsons should be a subset of Cell.
    CellWilsons *cells = (CellWilsons *)maze->cells;
    Vector2 *path = malloc(sizeof(Vector2) * sub_dims.x * sub_dims.y);

    // Initialize the cells with the following pattern:
    // # # # # #
    // # U # U #
    // # # # # #
    for (int y = 0; y < dims.y; y++)
    {
        for (int x = 0; x < dims.x; x++)
        {
            cells[y * dims.x + x] = (y % 2 == 1 && x % 2 == 1) ? CELL_WILSONS_UNDEFINED : CELL_WILSONS_WALL;
        }
    }
    // Mark one cell as part of the maze.
    cells[sub_idx(dims, vector2(sub_dims.x/2, sub_dims.y/2))] = CELL_WILSONS_PATH;

    while (true)
    {
        // Compute the number of undefined cells.
        int count = 0;
        for (int y = 0; y < sub_dims.y; y += 1)
        {
            for (int x = 0; x < sub_dims.x; x += 1)
            {
                if (cells[sub_idx(dims, vector2(x, y))] == CELL_WILSONS_UNDEFINED)
                {
                    count += 1;
                }
            }
        }
        if (count == 0)
        {
            break;
        }

        // Pick a starting point for the random walk.
        Vector2 head;
        {
            int undefined_cell_count = r1(count);
            bool found = false;
            for (int y = 0; !found && y < sub_dims.y; y += 1)
            {
                for (int x = 0; !found && x < sub_dims.x; x += 1)
                {
                    if (cells[sub_idx(dims, vector2(x, y))] == CELL_WILSONS_UNDEFINED)
                    {
                        if (undefined_cell_count == 0)
                        {
                            head = vector2(x, y);
                            found = true;
                        }
                        else
                        {
                            undefined_cell_count -= 1;
                        }
                    }
                }
            }
            assert(found);
        }

        cells[sub_idx(dims, head)] = CELL_WILSONS_RANDOM_WALK;
        path[0] = head;
        int path_len = 1;
        animate(&should_anim, &anim_delay, 4, maze, path, path_len);

        while (true)
        {
            // Compute the number of possible directions.
            int direction = r1(4);
            Vector2 next = head;
            switch (direction)
            {
            case 0:
                if (head.y == 0)
                    continue;
                next.y -= 1;
                break;
            case 1:
                if (head.y == sub_dims.y - 1)
                    continue;
                next.y += 1;
                break;
            case 2:
                if (head.x == 0)
                    continue;
                next.x -= 1;
                break;
            default:
                if (head.x == sub_dims.x - 1)
                    continue;
                next.x += 1;
                break;
            }

            // Prevent going in reverse.
            if (path_len >= 2)
            {
                Vector2 last = path[path_len - 2];
                if (vector2_eq(last, next))
                    continue;
            }

            CellWilsons next_cell = cells[sub_idx(dims, next)];

            if (next_cell == CELL_WILSONS_PATH)
            {
                // Animate the connection.
                {
                    path[path_len] = next;
                    path_len += 1;
                    animate(&should_anim, &anim_delay, 4, maze, path, path_len);
                    path_len -= 1;
                }

                // Finish the path.
                Vector2 last = next;

                for (int path_idx = path_len - 1; path_idx >= 0; path_idx--)
                {
                    Vector2 curr = path[path_idx];
                    Vector2 d = vector2_sub(curr, last);
                    cells[(last.y * 2 + 1 + d.y) * dims.x + (last.x * 2 + 1 + d.x)] = CELL_WILSONS_PATH;
                    cells[sub_idx(dims, curr)] = CELL_WILSONS_PATH;
                    last = curr;
                }
                break;
            }
            else if (next_cell == CELL_WILSONS_RANDOM_WALK)
            {
                // Animate the self intersection.
                {
                    path[path_len] = next;
                    path_len += 1;
                    animate(&should_anim, &anim_delay, 4, maze, path, path_len);
                    path_len -= 1;
                }

                // Erase loop.
                for (; path_len >= 1; path_len--)
                {
                    head = path[path_len - 1];
                    if (vector2_eq(next, head))
                        break;
                    cells[sub_idx(dims, head)] = CELL_WILSONS_UNDEFINED;

                    animate(&should_anim, &anim_delay, 1, maze, path, path_len);
                }
                // Assert that we did find an overlapping cell in the path.
                assert(path_len >= 1);

                animate(&should_anim, &anim_delay, 2, maze, path, path_len);
            }
            else if (next_cell == CELL_WILSONS_UNDEFINED)
            {
                // Mark as part of path.
                head = next;
                path[path_len] = head;
                path_len += 1;
                cells[sub_idx(dims, head)] = CELL_WILSONS_RANDOM_WALK;

                animate(&should_anim, &anim_delay, 2, maze, path, path_len);
            }
            else
            {
                // Unreachable.
                assert(false);
            }
        }
    }

    cells[sub_idx(dims, vector2(0, 0))] = (CellWilsons) CELL_START;
    cells[sub_idx(dims, vector2(sub_dims.x - 1, sub_dims.y - 1))] = (CellWilsons) CELL_GOAL;

    free(path);
}

void play(Vector2 dims)
{
    Maze maze = alloc_maze(dims);

    generate_maze_wilsons(&maze);

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
        mvprintw(0, 0, "Total Steps Taken: %i", counter);

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

int main(int argc, char * argv[])
{
    // Initialize the maze
    setlocale(LC_ALL, "");

    // This will use the same seed every second, but that's okay for our application.
    srand((unsigned int)time(NULL));

    // Ensure that all characters we use to print the maze are indeed of printing width 2.
    assert(wcwidth(U'😃') == MAZE_COL_WIDTH);
    assert(wcwidth(U'🚩') == MAZE_COL_WIDTH);
    assert(wcwidth(U'🚶') == MAZE_COL_WIDTH);
    assert(wcwidth(U'🌲') == MAZE_COL_WIDTH);
    assert(wcwidth(U'📌') == MAZE_COL_WIDTH);

    Vector2 dims = vector2(15, 15);
    if (argc >= 2) {
        dims.x = atoi(argv[1]) * 2 + 1;
    }
    if (argc >= 3) {
        dims.y = atoi(argv[2]) * 2 + 1;
    }

    if (dims.x < 3 || dims.y < 3)
    {
        fprintf(stderr, "Invoke the application as maze <x> <y> where <x> and <y> are non-zero integers.");   
        return 1;
    }
    
    // Initialize the screen buffer for ncurses.
    if (initscr() == NULL)
    {
        fprintf(stderr, "Error initializing ncurses.\n");
        return 1;
    }
    keypad(stdscr, 1);
    noecho();    // do not show the user input
    curs_set(0); // hide blinking cursor

    play(dims);

    endwin();

    return 0;
}
