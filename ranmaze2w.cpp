/* Andrew J Wood
 * COP4531 - Data Structures Algorithm Analysis
 * Project 3 - Mazes on the Web
 * June 13, 2019
 *
 * Implements a random maze generator as a client of fsu::Partition<>.  The objective is to output a file
 * per the specifications indicated on the command line arguments (number of rows, number of columns, output file
 * name) and then to generate a file with a numeric representation of the maze.
 *
 * This version (2w) generates only 1 file:
 *      -File containing only 1 partition where all cells are in the
 *      same partition
 *
 * Note that the code is self-documenting.
 */

#include <iostream>
#include <vector.h>
#include <partition2.h>
//#include <string> not allowed as per Prof. Lacher
#include <xran.h>
//#include <xran.cpp>
#include <sstream>
#include <fstream>
#include <ctype.h>

//function prototype - implemented after main
void Connect(
        size_t, size_t,         // start and goal cells
        size_t, size_t,         // maze dimensions
        fsu::Vector<uint8_t>& ,   // walls codes for cells - passed by reference
        fsu::Partition<size_t>&   // tracks component structure - passed by reference
                );

//function prototype - write the maze to the output file specified
void WriteMaze(
        std::ofstream &,      //file to write output to
        size_t, size_t,       //start cell and goal cell
        size_t, size_t,       //maze dimensions
        fsu::Vector<uint8_t> &  //the vector representing the maze
        );

const uint8_t NORTH = 0x01;
const uint8_t EAST  = 0x02;
const uint8_t SOUTH = 0x04;
const uint8_t WEST  = 0x08;

int main (int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "** command line arguments required:\n"
                  << "   1: number of rows\n"
                  << "   2: number of cols\n"
                  << "   3: file name for maze\n"
                  << "   4: 1 = trace [optional] *not implemented*\n";
        return 0; //exit program
    }

    size_t numrows = atoi(argv[1]);
    size_t numcols = atoi(argv[2]);

    //std::string fileName(argv[3]); not allowed

    const char * fileName(argv[3]);

    //std::stringstream ss(fileName, std::ios_base::app | std::ios_base::out);

    size_t numcells = numrows * numcols;
    size_t midrow = numrows / 2; //integer division

    //the starting cell, leftmost cell of the middle row
    size_t start = numcols * midrow;
    //the ending cell, rightmost cell of the middle row
    size_t goal = numcols * midrow + (numcols - 1);

    //output introduction
    std::cout << "Ranmaze: \n";
    std::cout << " numrows = " << numrows << ",";
    std::cout << " numcols = " << numcols << ",";
    std::cout << " numcells = " << numcells << ",";
    std::cout << " start = " << start << ",";
    std::cout << " goal = " << goal << ",";
    std::cout << " trace = 0\n";

    //establish variables to hold the maze representation
    fsu::Vector<uint8_t> maze (numcells, 15);   // all closed boxes
    fsu::Partition<size_t> p (numcells);        // all singletons

    // ensure goal is reachable from start:
    std::cout << " components after 0 passes:   " << p.Components() << '\n';
    Connect (start, goal, numrows, numcols, maze, p); //connect start and goal cells in the maze
    size_t components = p.Components();
    std::cout << " components after 1 pass:     " << components << '\n';

    // continue until all cells are reachable - skip if random cell called is already part of solution partition
    // which was already completed in the previous step.
    if (components > 1)
    {
        for (size_t cell = 1; cell < numcells; ++cell)
        {
            if (p.Components() == 1)
            {
                break; //don't bother continuing if there is already 1 component
            }
            Connect (0, cell, numrows, numcols, maze, p);
        }
    }

    char outFileName2[100] = {'\0'};

    char a = fileName[0];
    size_t initCount = 0;
    while (a != '\0')
    {
        outFileName2[initCount] = a;
        ++initCount;
        a = fileName[initCount];
    }

    //now, append the period
    outFileName2[initCount] = '.';
    ++initCount;

    //now append the 1
    outFileName2[initCount] = '1';
    
    std::cout << " components after all passes: " << p.Components();  //should always be 1

    //output new file
    std::ofstream out2(outFileName2, std::ofstream::out);
    WriteMaze(out2, start, goal, numrows, numcols, maze);
    out2.close();


} //end main

//connects the cells specified "beg" and "end" in the maze.  The partition structure representing the maze is looped through
//continuously where cells are randomly chosen to be unioned.  After the random choice, the algoirthm checks to see if the cells
//beg and end are in the partition.  This process continues until the this is true.  Additionally, the algorithm updates the vector
//maze to correctly represent the walls in the maze as the partition updates.
void Connect(size_t beg , size_t end , size_t numrows, size_t numcols, fsu::Vector<uint8_t>& maze , fsu::Partition<size_t>& p)
{
    //randomly choose a wall to knock down
    static fsu::Random_unsigned_int ran;

    //determine if the two target cells are connected (beg and end are the targets)
    bool connected = p.Find(beg, end);

    while (!connected) //while beg and end are NOT connected
    {
        //Step 1 - choose a cell randomly
        size_t randomCell = ran(0, numrows * numcols); //a random cell in the maze

        //skip knocking down a wall here if the cell selected is already part of the solution partition
        //bool isAlreadyInSolution = p.Find(randomCell,end);
        //if (isAlreadyInSolution)
        //    continue; //skip knocking down a wall here as cell is already part of solution, go to next iteration.

        //constants representing respective corners of the maze in terms of Vector index - will be preserved between calls (possible minor optimization)
        static const size_t NWcorner = 0;
        static const size_t NEcorner = numcols - 1;
        static const size_t SWcorner = numcols * numrows - numcols;
        static const size_t SEcorner = numcols * numrows - 1;


        //means of determining if the random cell selected is a non-corner wall (checked after the corner case is detected)
        char wall = 'X'; //default, set to X

        if (randomCell / numcols == 0) //correct
            wall = 'N';
        if (randomCell % numcols == (numcols - 1)) //correct
            wall = 'E';
        if (randomCell >= numrows * numcols - numcols) //correct
            wall = 'S';
        if (randomCell % numcols == 0) //correct
            wall = 'W';

        //Step 1: Is it a corner?
        if (randomCell == NWcorner || randomCell == NEcorner || randomCell == SWcorner || randomCell == SEcorner)
        {
            //if cell is the north west corner
            if (randomCell == NWcorner)
            {
                //Note: random generation will include 4 options to ensure the maze is 'truly random'; if an ineligible wall face is selected, the
                //step is simply skipped.
                size_t ranWall = ran(0, 4);
                switch (ranWall)
                {
                    case 0: //east wall should be knocked down - cell to its east
                        //note - break out if the cell to the east is already part of this cell's partition
                        if (p.Find(randomCell,randomCell+1))
                        {
                            break; //don't bother knocking down another wall since we want a circuitous path
                        }
                        p.Union(randomCell, randomCell + 1);
                        maze[randomCell] &= ~EAST;
                        maze[randomCell + 1] &= ~WEST;
                        break;
                    case 1: //south wall should be knocked down - cell to its south
                        if (p.Find(randomCell,randomCell+numcols))
                        {
                            break; //don't bother knocking down another wall since we want a circuitous path
                        }
                        p.Union(randomCell, randomCell + numcols);
                        maze[randomCell] &= ~SOUTH;
                        maze[randomCell + numcols] &= ~NORTH;
                        break;
                    default: //cases 3 and 4, do nothing
                        break;
                }
            } //end NW corner case

            //if cell is the north east corner
            if (randomCell == NEcorner)
            {
                size_t ranWall = ran(0, 4);
                switch (ranWall)
                {
                    case 0: //south wall should be knocked down
                        if (p.Find(randomCell,randomCell+numcols))
                        {
                            break; //don't bother knocking down another wall since we want a circuitous path
                        }
                        p.Union(randomCell, randomCell + numcols);
                        maze[randomCell] &= ~SOUTH;
                        maze[randomCell + numcols] &= ~NORTH;
                        break;
                    case 1: //west wall should be knocked down
                        if (p.Find(randomCell,randomCell-1))
                        {
                            break; //don't bother knocking down another wall since we want a circuitous path
                        }
                        p.Union(randomCell, randomCell - 1);
                        maze[randomCell] &= ~WEST;
                        maze[randomCell - 1] &= ~EAST;
                        break;
                    default: //cases 3 and 4, do nothing
                        break;
                }
            } //end NE corner case

            //if cell is the south west corner
            if (randomCell == SWcorner)
            {
                size_t ranWall = ran(0, 4);
                switch (ranWall)
                {
                    case 0: //north wall should be knocked down
                        if (p.Find(randomCell,randomCell-numcols))
                        {
                            break; //don't bother knocking down another wall since we want a circuitous path
                        }
                        p.Union(randomCell, randomCell - numcols);
                        maze[randomCell] &= ~NORTH;
                        maze[randomCell - numcols] &= ~SOUTH;
                        break;
                    case 1: //east wall should be knocked down
                        if (p.Find(randomCell,randomCell+1))
                        {
                            break; //don't bother knocking down another wall since we want a circuitous path
                        }
                        p.Union(randomCell, randomCell + 1);
                        maze[randomCell] &= ~EAST;
                        maze[randomCell + 1] &= ~WEST;
                        break;
                    default: //cases 3 and 4, do nothing
                        break;
                }
            } //end SW corner case

            //if cell is the south east corner
            if (randomCell == SEcorner)
            {
                size_t ranWall = ran(0, 4);
                switch (ranWall)
                {
                    case 0: //north wall should be knocked down
                        if (p.Find(randomCell,randomCell-numcols))
                        {
                            break; //don't bother knocking down another wall since we want a circuitous path
                        }
                        p.Union(randomCell, randomCell - numcols);
                        maze[randomCell] &= ~NORTH;
                        maze[randomCell - numcols] &= ~SOUTH;
                        break;
                    case 1: //west wall should be knocked down
                        if (p.Find(randomCell,randomCell-1))
                        {
                            break; //don't bother knocking down another wall since we want a circuitous path
                        }
                        p.Union(randomCell, randomCell - 1);
                        maze[randomCell] &= ~WEST;
                        maze[randomCell - 1] &= ~EAST;
                        break;
                    default: //cases 3 and 4, do nothing
                        break;
                }
            } //end SE corner case
        } //corner cell edge case

        else if (wall == 'N' || wall == 'E' || wall == 'S' || wall == 'W') //Step 2: not a corner, now check to see if it is an outer wall
        {
            size_t ranWall = ran(0, 4);
            switch (wall)
            {
                case 'N': //north wall cell is chosen
                    switch (ranWall)
                    {
                        case 0: //east wall should be knocked down
                            if (p.Find(randomCell,randomCell+1))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell + 1);
                            maze[randomCell] &= ~EAST;
                            maze[randomCell + 1] &= ~WEST;
                            break;
                        case 1: //south wall should be knocked down
                            if (p.Find(randomCell,randomCell+numcols))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell + numcols);
                            maze[randomCell] &= ~SOUTH;
                            maze[randomCell + numcols] &= ~NORTH;
                            break;
                        case 2: //west wall should be knocked down
                            if (p.Find(randomCell,randomCell-1))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell - 1);
                            maze[randomCell] &= ~WEST;
                            maze[randomCell - 1] &= ~EAST;
                            break;
                        default: //includes possibility of north wall being chosen, do nothing
                            break;
                    }
                    break;

                case 'E': //east wall cell is chosen
                    switch (ranWall)
                    {
                        case 0: //south wall should be knocked down
                            if (p.Find(randomCell,randomCell+numcols))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell + numcols);
                            maze[randomCell] &= ~SOUTH;
                            maze[randomCell + numcols] &= ~NORTH;
                            break;
                        case 1: //west wall should be knocked down
                            if (p.Find(randomCell,randomCell-1))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell - 1);
                            maze[randomCell] &= ~WEST;
                            maze[randomCell - 1] &= ~EAST;
                            break;
                        case 2: //north wall should be knocked down
                            if (p.Find(randomCell,randomCell-numcols))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell - numcols);
                            maze[randomCell] &= ~NORTH;
                            maze[randomCell - numcols] &= ~SOUTH;
                            break;
                        default: //includes possibility of east wall being chosen, do nothing
                            break;
                    }
                    break;

                case 'S': //south wall cell is chosen
                    switch (ranWall)
                    {
                        case 0: //west wall should be knocked down
                            if (p.Find(randomCell,randomCell-1))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell - 1);
                            maze[randomCell] &= ~WEST;
                            maze[randomCell - 1] &= ~EAST;
                            break;
                        case 1: //north wall should be knocked down
                            if (p.Find(randomCell,randomCell-numcols))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell - numcols);
                            maze[randomCell] &= ~NORTH;
                            maze[randomCell - numcols] &= ~SOUTH;
                            break;
                        case 2: //east wall should be knocked down
                            if (p.Find(randomCell,randomCell+1))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell + 1);
                            maze[randomCell] &= ~EAST;
                            maze[randomCell + 1] &= ~WEST;
                            break;
                        default: //includes possibility of south wall being chosen, do nothing
                            break;
                    }
                    break;

                case 'W': //west wall cell is chosen
                    switch (ranWall)
                    {
                        case 0: //north wall should be knocked down
                            if (p.Find(randomCell,randomCell-numcols))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell - numcols);
                            maze[randomCell] &= ~NORTH;
                            maze[randomCell - numcols] &= ~SOUTH;
                            break;
                        case 1: //east wall should be knocked down
                            if (p.Find(randomCell,randomCell+1))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell + 1);
                            maze[randomCell] &= ~EAST;
                            maze[randomCell + 1] &= ~WEST;
                            break;
                        case 2: //south wall should be knocked down
                            if (p.Find(randomCell,randomCell+numcols))
                            {
                                break; //don't bother knocking down another wall since we want a circuitous path
                            }
                            p.Union(randomCell, randomCell + numcols);
                            maze[randomCell] &= ~SOUTH;
                            maze[randomCell + numcols] &= ~NORTH;
                            break;
                        default: //includes possibility of west wall being chosen, do nothing
                            break;
                    }
                    break;

                default:
                    break;
            }
        } //edge wall cell case

        else //Step 3: neither a corner nor an outer wall, so it's a normal internal cell
        {
            //generate random number 0 to 3
            size_t ranWall = ran(0, 4);
            switch (ranWall)
            {
                case 0: //north wall should be knocked down
                    if (p.Find(randomCell,randomCell-numcols))
                    {
                        break; //don't bother knocking down another wall since we want a circuitous path
                    }
                    p.Union(randomCell, randomCell - numcols);
                    maze[randomCell] &= ~NORTH;
                    maze[randomCell - numcols] &= ~SOUTH;
                    break;
                case 1: //east wall should be knocked down
                    if (p.Find(randomCell,randomCell+1))
                    {
                        break; //don't bother knocking down another wall since we want a circuitous path
                    }
                    p.Union(randomCell, randomCell + 1);
                    maze[randomCell] &= ~EAST;
                    maze[randomCell + 1] &= ~WEST;
                    break;
                case 2: //south wall should be knocked down
                    if (p.Find(randomCell,randomCell+numcols))
                    {
                        break; //don't bother knocking down another wall since we want a circuitous path
                    }
                    p.Union(randomCell, randomCell + numcols);
                    maze[randomCell] &= ~SOUTH;
                    maze[randomCell + numcols] &= ~NORTH;
                    break;
                case 3: //west wall should be knocked down
                    if (p.Find(randomCell,randomCell-1))
                    {
                        break; //don't bother knocking down another wall since we want a circuitous path
                    }
                    p.Union(randomCell, randomCell - 1);
                    maze[randomCell] &= ~WEST;
                    maze[randomCell - 1] &= ~EAST;
                    break;
                default: //Note - should never get here
                    std::cerr << "Some error occurred in maze generation.  Aborting program.";
                    exit(1);
                    break;
            }
        }
        //now, check again to see if beg and end are connected
        connected = p.Find(beg,end);
    } //while (!connected)
} //end function Connect

void WriteMaze(std::ofstream & of, size_t start, size_t goal, size_t rows, size_t cols, fsu::Vector<uint8_t> & maze)
{
    //output maze to file

    //output rows and columns
    of << "\t" << rows << "\t" << cols << "\n";

    //create iterator for maze
    fsu::Vector<uint8_t>::ConstIterator i = maze.Begin();

    //output the maze array
    for (size_t row = 0; row < rows; ++row)
    {
        for (size_t col = 0; col < cols; ++col)
        {
            of << "\t" << (unsigned int)*i;
            ++i;
        }
        of << "\n";
    }

    //output start and goal cells
    of << "\t" << start << "\t" << goal << "\n";
}