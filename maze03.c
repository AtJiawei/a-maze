// this version is the best version so far. the Move function simplied the movement check.
#include <stdio.h>
//define the global variables
#define PATH '-'
#define GOAL '!'
#define PLAYER 'V'
#define WALL '#'

#define MAZE_ROWS 7
#define MAZE_COLS 7

typedef struct 
{
    int x;
    int y;
}Position;


Position findPlayer(char maze[MAZE_ROWS][MAZE_COLS]) 
{
    Position playerPosition = {0, 0};
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            if (maze[i][j] == PLAYER)
            {
                playerPosition.x = i;
                playerPosition.y = j;
                return playerPosition;
            }
        }
    }
    return playerPosition;
}

int updateMaze(char Move, char maze[MAZE_ROWS][MAZE_COLS])
{
    Position playerPosition = findPlayer(maze);
    int newX = playerPosition.x;
    int newY = playerPosition.y;

    if(Move == 'w')
    {
        newX = newX - 1;
    }
    if(Move == 's')
    {
        newX = newX + 1;
    }
    if(Move == 'a')
    {
        newY = newY - 1;
    }
    if(Move == 'd')
    {
        newY = newY + 1;
    }

    if (newX >= 0 && newX < MAZE_ROWS && newY >= 0 && newY < MAZE_COLS && maze[newX][newY] != WALL) {
        if (maze[newX][newY] == GOAL) {
            maze[playerPosition.x][playerPosition.y] = PATH;
            maze[newX][newY] = PLAYER;
            return 1;  // Player reached goal
        } else {
            maze[playerPosition.x][playerPosition.y] = PATH;
            maze[newX][newY] = PLAYER;
            return 0;  // Normal move
        }
    }
    return 0;
}

void printMaze(char maze[MAZE_ROWS][MAZE_COLS])
{
    printf("\n");
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
}

void play(char maze[MAZE_ROWS][MAZE_COLS])
{
    char Move;
    while (1)
    {
        printf("\n____ Enter w/s/a/d to control the avatar, enter q to exit the game (please only use lower case): ");
        scanf(" %c", &Move);
        
        if (Move == 'q')
        {
            printf("\n|| Game Ended.\n\n|| You have chosen to quit the game by pressing q.\n\n");
            break; // end the loop if 'q' is pressed
        }
        else if (Move == 'w'|| Move == 's'|| Move == 'a'|| Move == 'd')
        {
            if(updateMaze(Move, maze))
            {
                printMaze(maze);
                printf("\n|| Congratulations! \n\n|| You have WON!\n\n");
                break;            
            }
            printMaze(maze); 
        }       
        else
        {
            printf("Invalid key entered.\n");
        }
    }
}

int main()
{
    // Initialize the maze
    char maze[MAZE_ROWS][MAZE_COLS] =
        {
            {WALL, WALL, WALL, WALL, WALL, WALL, WALL},
            {WALL, PLAYER, PATH, WALL, PATH, WALL, WALL},
            {WALL, WALL, PATH, WALL, PATH, PATH, WALL},
            {WALL, PATH, PATH, PATH, PATH, WALL, WALL},
            {WALL, PATH, WALL, WALL, PATH, PATH, WALL},
            {WALL, PATH, PATH, WALL, WALL, PATH, WALL},
            {WALL, WALL, WALL, WALL, WALL, GOAL, WALL}
        };

    // Print the maze
    printMaze(maze);
    play(maze);
    return 0;
}
