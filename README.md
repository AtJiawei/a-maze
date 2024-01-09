# Maze Generator in C
#### This project is an implementation of an automatically generated maze written in C.

It initially started as a basic version, utilizing only <stdio.h>, and has since evolved into a game featuring automatic maze generation using [Prim's Algorithm](https://en.wikipedia.org/wiki/Prim%27s_algorithm#:~:text=In%20computer%20science%2C%20Prim's%20algorithm,in%20the%20tree%20is%20minimized.).

## Usage
To compile and run the program, ensure you have the <ncurses.h> library installed. Adjust the building command to link the ncurses library appropriately.

## Building with Bazel
For a streamlined experience, it is recommended to use [Bazel](https://skia.googlesource.com/buildbot/+/refs/heads/main/BAZEL_CHEATSHEET.md) to build and run the program.

## Testing
As the program has become more complex, an emphasis has been placed on testing major functions for pre-checking and debugging. Automatic tests have been implemented to verify the functionality of key functions.

To facilitate automatic testing, the testing functions and corresponding headers are separated into different files. In more recent versions, aside from the main file <maze.c>, the header file <lib.h> and the source file <lib.c> must also be downloaded to ensure proper execution of the program.
