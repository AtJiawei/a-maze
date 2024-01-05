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

void fill_maze(Maze *maze)
{
    Vector2 dims = maze->dims;
    assert(dims.x > 2);
    assert(dims.y > 2);

    // Prim's algorithm
    // initiate arrays and allocate room to store cells in a maze: done at alloc_malloc. cells is a pointer potining to an array with dims.x * dims.y number items
    // Store the length of array in a variable for easy reference:
    fill_maze_with(maze, CELL_WALL);

    Vector2 sub_dims = (Vector2){
        .x = (dims.x - 1) / 2,
        .y = (dims.y - 1) / 2,
    };

    int candidate_count = sub_dims.x * sub_dims.y; // got the result 12 in this case (X == 7 and Y == 9)

    int candidate_indices[candidate_count]; // to store the possible valid cell_path candidates and later adjacent cells
    int candidate_indices_len = 0;          // it functions as a counter to index the array

    for (int y = 1; y < dims.y; y += 2)
    {
        for (int x = 1; x < dims.x; x += 2)
        {
            int idx = idx_2to1(vector2(x, y), maze->dims);
            candidate_indices[candidate_indices_len++] = idx; // candidate_indices_len will increase by 1 after this function is called.
            // more: int x = 5; int y = x++; will result in y = 5, x = 6. However, int x = 5; int y = ++x; will result in x = 6 and y = 6 because the increment happens before returning the value.
        }
    }

    int rdm_num = rand() % candidate_count;
    int rdm_idx = candidate_indices[rdm_num];
    maze->cells[rdm_idx] = CELL_PATH;
    // till this line, the starting cell is selected and changed to CELL_PATH. Codes compiled.
    // from there, we start to check the adjacent cells

    int adj_indices[candidate_count]; // to store the adjacent cells;
    for (int i = 0; i < candidate_count; i++)
    {
        adj_indices[i] = 0; // to initialize all the value to 0
    }

    int adj_indices_len = 0; // to count the valid adjacent cells. to be used to generaze the randoem number

    // TODO: I think here starts the loop where the loop should start. the loop stops when adj_counter = 12. Write this loop condition later. Maybe write an extra function for this step?

    while (adj_indices_len < candidate_indices_len)
    {
        for (int i = 0; i < candidate_indices_len; i++) // a function can be written for each condition below.
        {
            if ((candidate_indices[i] == rdm_idx - 2) && (maze->cells[rdm_idx - 2] != CELL_PATH))
            {
                adj_indices[adj_indices_len++] = rdm_idx - 2;
                break;
            }
            if ((candidate_indices[i] == rdm_idx + 2) && (maze->cells[rdm_idx + 2] != CELL_PATH))
            {
                adj_indices[adj_indices_len++] = rdm_idx + 2;
                break;
            }
            if ((candidate_indices[i] == rdm_idx + 2 * sub_dims.x) && (maze->cells[rdm_idx + 2 * sub_dims.x] != CELL_PATH))
            {
                adj_indices[adj_indices_len++] = rdm_idx + 2 * sub_dims.x;
                break;
            }
            if ((candidate_indices[i] == rdm_idx - 2 * sub_dims.x) && (maze->cells[rdm_idx - 2 * sub_dims.x] != CELL_PATH))
            {
                adj_indices[adj_indices_len++] = rdm_idx - 2 * sub_dims.x;
                break;
            }
        }

        int rdm_idx_new;
        do
        {
            rdm_idx_new = adj_indices[rand() % adj_indices_len];
        } while (rdm_idx_new != 0);

        maze->cells[rdm_idx_new] = CELL_PATH;

        // also change the cell between the newly selected cell and previous cell as path to connect two cells
        int d_idx = rdm_idx_new - rdm_idx;
        if (d_idx == 2)
        {
            maze->cells[rdm_idx + 1] = CELL_PATH;
        }
        if (d_idx == -2)
        {
            maze->cells[rdm_idx - 1] = CELL_PATH;
        }
        if (d_idx == 2 * sub_dims.x)
        {
            maze->cells[rdm_idx + sub_dims.x] = CELL_PATH;
        }
        if (d_idx == -(2 * sub_dims.x))
        {
            maze->cells[rdm_idx - sub_dims.x] = CELL_PATH;
        }

        adj_indices[rdm_idx_new] = 0;
        rdm_idx = rdm_idx_new; // update rdm_idx to restart the loop
    }

    // Set the start and goal.
    maze->cells[1 * maze->dims.x + 1] = CELL_START;
    maze->cells[(maze->dims.y - 2) * maze->dims.x + (maze->dims.x - 2)] = CELL_GOAL;
}

void play()
{
    Vector2 dims = {
        .x = 7,
        .y = 9,
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

    // srand((unsigned int)time(NULL)); TODO: enable this when maze generation is done.

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
