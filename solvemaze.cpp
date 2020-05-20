/* Andrew J Wood
 * COP4531 - Data Structures Algorithm Analysis
 * Project 3 - Mazes on the Web
 * June 13, 2019
 *
 * Uses existing utilities in maze_util.h to load the maze file generated in ranmaze2* programs, represents
 * the maze as a graph object, then uses DFS or BFS to solve the maze.
 *
 * Note that the code is self-documenting.
 */

#include <iostream>
#include <maze_util.h>
#include <graph.h>
#include <graph_util.h>
#include <xstring.h>
#include <fstream>

int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "** command line arguments required:\n"
                  << "   1: mazefile\n"
                  << "** try again\n\n";
        return 0; //exit program
    }

    //argument read successfully, now load the maze
    //create an ALD (Ajacency List Directed) graph

    fsu::String filename(argv[1]);
    fsu::ALDGraph<size_t> mazeGraph;
    size_t start;
    size_t goal;
    fsu::List<size_t> SolutionPath;

    //will populate the graph based on maze read in mazeFile
    LoadMaze(filename.Cstr(),mazeGraph,start,goal);

    //checks symmetry, outputs error if there is a problem
    CheckSymmetry(mazeGraph,1);

    //use the utility to find the path
    bool solution = Path_DFS(mazeGraph,start,goal,SolutionPath);

    if (!solution)
    {
        std::cout << " no solution";
        exit(1);
    }

    //compute number of cells in the solution path
    size_t cells = SolutionPath.Size();

    //append dfs to file name
    fsu::String outFileName = filename + ".dfs";

    //output solution to file
    std::ofstream outFile(outFileName.Cstr(),std::ios_base::out); //open new file for output
    std::ifstream inFile(filename.Cstr(),std::ios_base::in); //open existing file to read input

    outFile << inFile.rdbuf();

    outFile << "\n";

    //iterate through solution vector
    fsu::ConstListIterator<size_t> ci = SolutionPath.Begin();
    for (; ci != SolutionPath.End(); ++ci)
    {
        outFile << " " << *ci;
    }

    outFile << "\n";

    std::cout << "Solution found has " << cells << " cells\n";
    std::cout << "Maze and solution written to file \"" << outFileName << "\"\n";

    inFile.close();
    outFile.close();

    return 0; //exit success
}

#include <xstring.cpp> //in lieu of makefile mod