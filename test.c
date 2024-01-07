#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lib.h"

void test_vector2()
{
    Vector2 v = vector2(1, 2);
    assert(v.x == 1);
    assert(v.y == 2);
}

// TASK: Think if you can change the implementation of vector2_eq so that it does NOT do the right thing anymore, but do it in such a way that the test below still passes (without modification).
void test_vector2_eq()
{
    assert(vector2_eq(
        vector2(1, 2),
        vector2(1, 2)));

    assert(!vector2_eq(
        vector2(1, 2),
        vector2(1, 3)));

    assert(!vector2_eq(
        vector2(1, 2),
        vector2(2, 2)));
}

void test_vector2_add()
{

    assert(vector2_eq((vector2_add(
                          vector2(1, 2),
                          vector2(1, 2))),
                      vector2(2, 4)));

    assert(!vector2_eq((vector2_add(
                           vector2(2, 2),
                           vector2(1, 5))),
                       vector2(5, 7)));

    assert(!vector2_eq((vector2_add(
                           vector2(3, 2),
                           vector2(6, 7))),
                       vector2(9, 2)));
}

void test_idx_2to1()
{
    Vector2 index2 = {
        .x = 2,
        .y = 1,
    };

    Vector2 dimensions = {
        .x = 3,
        .y = 3,
    };

    int correct_idx = 5;

    assert(idx_2to1(index2, dimensions) == correct_idx);
}

void test_idx_1to2()
{
    Vector2 dimensions = {
        .x = 5,
        .y = 5,
    };

    int index1 = 12;

    Vector2 correct_result = {
        .x = 2,
        .y = 2,
    };

    Vector2 result = idx_1to2(index1, dimensions);

    assert(vector2_eq(result, correct_result));
}

// TASK: Write a function vector2_add which adds two vectors together and returns the result (also a vector), and write a test function for it. Make sure to also call the test function from main!

void test_fill_cell_with()
{
    Maze maze = alloc_maze(vector2(2, 3));
    fill_maze_with(&maze, CELL_WALL);
    assert(maze.cells[0] == CELL_WALL);
    assert(maze.cells[1] == CELL_WALL);
    assert(maze.cells[2] == CELL_WALL);
    assert(maze.cells[3] == CELL_WALL);
    assert(maze.cells[4] == CELL_WALL);
    assert(maze.cells[5] == CELL_WALL);
}

void test_prim_is_candidate()
{
    Maze maze = alloc_maze(vector2(7, 7));
    fill_maze_with(&maze, CELL_WALL);
    int cand_idx = idx_2to1(vector2(3, 3), maze.dims);
    maze.cells[cand_idx] = CELL_PATH;
    /*
    w w w w w w w
    w w w w w w w
    w w w w w w w
    w w w p w w w
    w w w w w w w
    w w w w w w w
    w w w w w w w
    */
    assert(prim_is_candidate(&maze, cand_idx));
    maze.cells[cand_idx - 2] = CELL_PATH;
    assert(prim_is_candidate(&maze, cand_idx));
    maze.cells[cand_idx + 2] = CELL_PATH;
    assert(prim_is_candidate(&maze, cand_idx));
    maze.cells[cand_idx - maze.dims.x] = CELL_PATH;
    assert(prim_is_candidate(&maze, cand_idx));
    maze.cells[cand_idx + maze.dims.x] = CELL_PATH;
    assert(!prim_is_candidate(&maze, cand_idx));
}

int main()
{
    test_vector2();
    test_vector2_eq();
    test_vector2_add();
    test_idx_2to1();
    test_idx_1to2();
    test_fill_cell_with();
    test_prim_is_candidate();

    printf("\nTests Passed!\n");
}
