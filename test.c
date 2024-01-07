#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lib.h"

Maze test_maze(char *contents)
{
    Vector2 dims = vector2(0, 0);
    while (1)
    {
        // Expected at least one newline before the end of the string.
        assert(contents[dims.x] != '\0');
        if (contents[dims.x] == '\n')
        {
            dims.y += 1;
            break;
        };
        dims.x += 1;
    }

    while (contents[dims.y * (dims.x + 1)] != '\0')
    {
        for (int x = 1; x < dims.x; x++)
            assert(contents[dims.y * (dims.x + 1) + x] != '\0');

        // Lines should end with a newline.
        assert(contents[(dims.y + 1) * (dims.x + 1) - 1] == '\n');
        dims.y += 1;
    }

    Maze maze = alloc_maze(dims);

    for (int y = 0; y < dims.y; y++)
    {
        for (int x = 0; x < dims.x; x++)
        {
            char c = contents[y * (dims.x + 1) + x];
            if (c == 'w')
            {
                *maze_cell_ptr(&maze, vector2(x, y)) = CELL_WALL;
            }
            else if (c == 'p')
            {
                *maze_cell_ptr(&maze, vector2(x, y)) = CELL_PATH;
            }
            else
            {
                // Unknown character type.
                assert(0);
            }
        }
    }

    return maze;
}

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
    Maze maze = test_maze(
        "wwwwwww\n"
        "wwwwwww\n"
        "wwwwwww\n"
        "wwwpwww\n"
        "wwwwwww\n"
        "wwwwwww\n"
        "wwwwwww\n");

    int cand_idx = idx_2to1(vector2(3, 3), maze.dims);
    assert(prim_is_candidate(&maze, cand_idx));

    maze = test_maze(
        "wwwwwww\n"
        "wwwwwww\n"
        "wwwwwww\n"
        "wpppwww\n"
        "wwwwwww\n"
        "wwwwwww\n"
        "wwwwwww\n");
    assert(prim_is_candidate(&maze, cand_idx));

    maze.cells[cand_idx + 2] = CELL_PATH;
    assert(prim_is_candidate(&maze, cand_idx));
    maze.cells[cand_idx - 2 * maze.dims.x] = CELL_PATH;
    assert(prim_is_candidate(&maze, cand_idx));
    maze.cells[cand_idx + 2 * maze.dims.x] = CELL_PATH;
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
