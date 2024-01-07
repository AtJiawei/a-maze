#include "lib.h"
#include <stdlib.h>

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

Vector2 vector2(int x, int y)
{
    return (Vector2){.x = x, .y = y};
}

bool vector2_eq(Vector2 a, Vector2 b)
{
    return a.x == b.x && a.y == b.y;
}

Vector2 vector2_add(Vector2 a, Vector2 b)
{
    return (Vector2){.x = a.x + b.x, .y = a.y + b.y};
}

int idx_2to1(Vector2 index2, Vector2 dimensions)
{
    return dimensions.x * index2.y + index2.x;
}

Vector2 idx_1to2(int index1, Vector2 dimensions)
{
    Vector2 index;
    index.x = index1 % dimensions.x;
    index.y = index1 / dimensions.x;
    return index;
}

Maze alloc_maze(Vector2 dims)
{
    Cell *cells = malloc(sizeof(Cell) * dims.x * dims.y);
    Maze maze = {
        .cells = cells,
        .dims = dims,
    };
    return maze;
}

void fill_maze_with(Maze *maze, Cell cell)
{
    int n = maze->dims.x * maze->dims.y;
    for (int i = 0; i < n; i++)
    {
        maze->cells[i] = cell;
    }
}

Vector2 to_sub_idx(Vector2 idx)
{
    return (Vector2){
        .x = (idx.x - 1) / 2,
        .y = (idx.y - 1) / 2,
    };
}

Vector2 from_sub_idx(Vector2 sub_idx)
{
    return (Vector2){
        .x = sub_idx.x * 2 + 1,
        .y = sub_idx.y * 2 + 1,
    };
}

Cell maze_cell(Maze *maze, Vector2 idx)
{
    return maze->cells[idx_2to1(idx, maze->dims)];
}

Cell *maze_cell_ptr(Maze *maze, Vector2 idx)
{
    return &(maze->cells[idx_2to1(idx, maze->dims)]);
}

// The sub maze is composed of the entries at the odd indices in the full maze.
// This function returns the cell by value at the sub index.
Cell sub_maze_cell(Maze *maze, Vector2 sub_idx)
{
    return maze_cell(maze, from_sub_idx(sub_idx));
}

Cell *sub_maze_cell_ptr(Maze *maze, Vector2 sub_idx)
{
    return maze_cell_ptr(maze, from_sub_idx(sub_idx));
}

bool idx_in_bound(Vector2 idx, Vector2 dims)
{
    return idx.x >= 0 && idx.x < dims.x && idx.y >= 0 && idx.y < dims.y;
}

bool prim_is_candidate(Maze *maze, int idx)
{
    Vector2 sub_dims = to_sub_idx(maze->dims);
    Vector2 sub_idx = to_sub_idx(idx_1to2(idx, maze->dims));

    if (sub_maze_cell(maze, sub_idx) != CELL_PATH)
    {
        return false;
    }

    Vector2 deltas[4] = {
        vector2(0, -1),
        vector2(-1, 0),
        vector2(0, 1),
        vector2(1, 0),
    };

    for (int i = 0; i < 4; i++)
    {
        Vector2 adj_sub_idx = vector2_add(sub_idx, deltas[i]);
        if (idx_in_bound(adj_sub_idx, sub_dims) && sub_maze_cell(maze, adj_sub_idx) == CELL_WALL)
        {
            return true;
        }
    }

    return false;
}
