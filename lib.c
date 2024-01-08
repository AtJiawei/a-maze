#include "lib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

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
    int n = maze_cell_count(maze);
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

// The function takes the candidate index and return the target count
int prim_count_candidate_targets(Maze *maze, int idx)
{
    Vector2 sub_dims = to_sub_idx(maze->dims);
    Vector2 sub_idx = to_sub_idx(idx_1to2(idx, maze->dims));

    Vector2 deltas[4] = {
        vector2(0, -1),
        vector2(-1, 0),
        vector2(0, 1),
        vector2(1, 0),
    };

    int counter = 0;
    for (int i = 0; i < 4; i++)
    {
        Vector2 adj_sub_idx = vector2_add(sub_idx, deltas[i]);
        if (idx_in_bound(adj_sub_idx, sub_dims) && sub_maze_cell(maze, adj_sub_idx) == CELL_WALL)
        {
            counter++;
        }
    }
    return counter;
}

bool prim_is_candidate(Maze *maze, int idx)
{
    return maze->cells[idx] == CELL_PATH && prim_count_candidate_targets(maze, idx) > 0;
}

int prim_count_candidates(Maze *maze)
{
    int counter = 0;
    Vector2 sub_dims = to_sub_idx(maze->dims);
    for (int y = 0; y < sub_dims.y; y++)
    {
        for (int x = 0; x < sub_dims.x; x++)
        {
            int cand_idx = idx_2to1(from_sub_idx(vector2(x, y)), maze->dims);
            if (prim_is_candidate(maze, cand_idx))
            {
                counter++;
            }
        }
    }
    return counter;
}

int maze_cell_count(Maze *maze)
{
    return maze->dims.x * maze->dims.y;
}

// The function takes a rdm number ( from 0 to candidate count) and returns the 1D candidate index in a maze
int prim_pick_random_candidate(Maze *maze, int rdm)
{
    int counter = 0;
    Vector2 sub_dims = to_sub_idx(maze->dims);
    for (int y = 0; y < sub_dims.y; y++)
    {
        for (int x = 0; x < sub_dims.x; x++)
        {
            int cand_idx = idx_2to1(from_sub_idx(vector2(x, y)), maze->dims);
            if (prim_is_candidate(maze, cand_idx))
            {
                if (counter == rdm)
                {
                    return cand_idx;
                }
                counter++;
            }
        }
    }
    // if we reach here, the candidate count was incorrect and the randome number picked was larger than the actual candidate count
    assert(false);
}

// TO BE TESTED
void choose_random_target(Maze *maze, int rdm_num, int candidate_index) // int target_count was deleted for testing purpose
{
    int counter = 0;

    Vector2 sub_dims = to_sub_idx(maze->dims);
    Vector2 sub_idx = to_sub_idx(idx_1to2(candidate_index, maze->dims));

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
            
            if (counter == rdm_num)
            {
                int new_path_idx =
                    idx_2to1(from_sub_idx(vector2_add(deltas[i], to_sub_idx(idx_1to2(candidate_index, maze->dims)))), maze->dims);

                 maze->cells[new_path_idx] = CELL_PATH;

                if (i == 0)
                {
                    maze->cells[candidate_index - maze->dims.x] = CELL_PATH;
                }
                else if (i == 1)
                {
                    maze->cells[candidate_index - 1] = CELL_PATH;
                }
                else if (i == 2)
                {
                    maze->cells[candidate_index + maze->dims.x] = CELL_PATH;
                }
                else if (i == 3)
                {
                    maze->cells[candidate_index + 1] = CELL_PATH;
                }
                
                break;
            }
            counter++;
        }
    }
}
