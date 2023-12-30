//1st working version as backup
#include <stdio.h>
//define the global variables
const int ROWS = 5;
const int COLS = 5;
const char PATH = '-';
const char GOAL = '!';
const char PLAYER = 'V';
const char WALL = '#';

int updateMaze(char Move, char maze[ROWS][COLS]) 
{
    // check the location of the player(V)
    int x, y;
    for (int m = 0; m < ROWS; m++)
    {
        for (int n = 0; n < COLS; n++)
        {
            if (maze[m][n] == PLAYER)
            {
                x = m;
                y = n;
                goto outer;
            }
        }
    }

outer:
    if (Move == 'd' && y != COLS - 1 && maze[x][y + 1] != WALL)
    {
        if(maze[x][y + 1] == GOAL)
        {
            maze[x][y] = PATH;
            maze[x][y + 1] = PLAYER;
            return 1;
        }
        else
        {
            maze[x][y] = PATH;
            maze[x][y + 1] = PLAYER;
            return 0;
        }
    }

    if (Move == 's' && x != ROWS - 1 && maze[x + 1][y] != WALL)
    {
        if(maze[x + 1][y] == GOAL)
        {
            maze[x][y] = PATH;
            maze[x + 1][y] = PLAYER;
            return 1;
        }
        else
        {
            maze[x][y] = PATH;
            maze[x + 1][y] = PLAYER;
            return 0;
        }
    }

    if (Move == 'a' && y != 0 && maze[x][y - 1] != WALL)
    {
        if(maze[x][y - 1] == GOAL)
        {
            maze[x][y] = PATH;
            maze[x][y - 1] = PLAYER;
            return 1;
        }
        else
        {
            maze[x][y] = PATH;
            maze[x][y - 1] = PLAYER;
            return 0;
        }
    }

    if (Move == 'w' && x != 0 && maze[x - 1][y] != WALL)
    {
        if(maze[x - 1][y]== GOAL)
        {
            maze[x][y] = PATH;
            maze[x - 1][y] = PLAYER;
            return 1;
        }
        else
        {
            maze[x][y] = PATH;
            maze[x - 1][y] = PLAYER;
            return 0;
        }
    }
    return 0;
}

// Function to print the maze
void printMaze(char maze[ROWS][COLS])
{
    printf("\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
}

void play(char maze[ROWS][COLS])
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
    char maze[ROWS][COLS] =
        {
            {PLAYER, PATH, WALL, PATH, WALL},
            {WALL, PATH, WALL, PATH, PATH},
            {PATH, PATH, PATH, PATH, WALL},
            {PATH, WALL, WALL, PATH, PATH},
            {PATH, PATH, WALL, WALL, GOAL}
        };

    // Print the maze
    printMaze(maze);
    play(maze);
    return 0;
}
