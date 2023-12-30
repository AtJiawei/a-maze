#include <stdio.h>

const char *PATH = "🌿";
const char *GOAL = "🚩";
const char *PLAYER = "🚶";
const char *WALL = "🧱";

const int MAZE_ROWS = 7;
const int MAZE_COLS = 7;

void printMaze(const char *maze[MAZE_ROWS][MAZE_COLS])
{
    printf("\n");
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            printf("%s ", maze[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    const char *maze[MAZE_ROWS][MAZE_COLS] =
        {
            {WALL, WALL, WALL, WALL, WALL, WALL, WALL},
            {WALL, PLAYER, PATH, WALL, PATH, WALL, WALL},
            {WALL, WALL, PATH, WALL, PATH, PATH, WALL},
            {WALL, PATH, PATH, PATH, PATH, WALL, WALL},
            {WALL, PATH, WALL, WALL, PATH, PATH, WALL},
            {WALL, PATH, PATH, WALL, WALL, PATH, WALL},
            {WALL, WALL, WALL, WALL, WALL, GOAL, WALL}};

    // Print the maze
    printMaze(maze);
}