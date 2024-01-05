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
    // if ((b.x == 1 && b.y == 3) || (b.x == 2 && b.y == 2))
    // {
    //     return false;
    // }
    // else
    // {
    //     return true;
    // }
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
