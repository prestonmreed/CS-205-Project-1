# CS-205-Project-1
Eight-Puzzle

To run the code, compile using the command
```
g++ eightPuzzle_final.cpp
```
This command will create the executable file "a.out". To execute the program, type the executable's path as follows:
```
./a.out
```
At this point, you will be prompted to enter the initial eight-puzzle that you want to be solved. Enter each row separately. For example, if the puzzle you want to solve is:
```
1 2 3
4 5 6
7 0 8
```
you would write the first row as "1 2 3" (space-separated) then press enter to move on and enter the next row. Once three rows have been entered, you will be prompted to choose a heuristic to sovle the puzzle. enter a single 1, 2, or 3 based on your choice and press enter. The algorithm will run. At the end it will print out a few statistics (depth of solution, number of nodes expanded, and maximum size of the queue). It will also provide the sequence of states from the intitial state to get to the solution.
