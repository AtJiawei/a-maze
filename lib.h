#pragma once

#include <stdbool.h>

// ^ only include this file once when compiling the program. Otherwise we might end up with multiple declarations with the same name, which the compiler does not know what to do with.

int max(int a, int b);
int min(int a, int b);

// Returns min if value < min, max if value > max, and otherwise the value itself.
int clamp(int value, int min_, int max_);

typedef struct Vector2
{
    int x;
    int y;
} Vector2;

Vector2 vector2(int x, int y);

bool vector2_eq(Vector2 a, Vector2 b);

Vector2 vector2_add(Vector2 a, Vector2 b);

int idx_2to1(Vector2 index, Vector2 dimensions);
Vector2 idx_1to2(int index, Vector2 dimensions);

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

Maze alloc_maze(Vector2 dims);

void fill_maze_with(Maze *maze, Cell cell);

// The function returns whether a cell is a candidate for expansion.
bool prim_is_candidate(Maze *maze, int idx);

// The function takes a 2D index in a bigger matrix (maze) and convert it to a 2D index in sub_maze
Vector2 to_sub_idx(Vector2 idx);

// The function takes a 2D index in sub_maze and convert it into a 2D index in a bigger matrix (maze)
Vector2 from_sub_idx(Vector2 sub_idx);

// The function takes a 2D index in maze and returns the cell value (use to compare value)
Cell maze_cell(Maze *maze, Vector2 idx);

// The function takes a 2D index in maze and returns the pointer to the cell value (use to modify)
Cell *maze_cell_ptr(Maze *maze, Vector2 idx);

// The sub maze is composed of the entries at the odd indices in the full maze.
// This function returns the cell by value at the sub index.
Cell sub_maze_cell(Maze *maze, Vector2 sub_idx);

Cell *sub_maze_cell_ptr(Maze *maze, Vector2 sub_idx);

bool idx_in_bound(Vector2 idx, Vector2 dims);

