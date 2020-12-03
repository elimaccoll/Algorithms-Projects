// Project 5 - Part A
// Anthony Cherubino, Eli MacColl
// 
// Main program file for Project 5 - Part A

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"

using namespace std;

int mazes = 0;

class maze
{
public:
    maze(ifstream& fin);
    void print(int, int, int, int);
    bool isLegal(int i, int j);

    void setMap(int i, int j, int n);
    int getMap(int i, int j) const;
    void mapMazeToGraph(graph& g);

    int getRows();
    int getCols();

    void printMap(); //just for testing map

    void findPathRecursive();
    void findPathNonRecursive();

private:
    int rows; // number of rows in the maze
    int cols; // number of columns in the maze
    int nodeID = 1;
    int numNodes = 0;

    matrix<bool> value;
    matrix<int> map;      // Mapping from maze (i,j) values to node index values
};

int maze::getRows()
{
    return rows;
}

int maze::getCols()
{
    return cols;
}

void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n. 
{
    map[i][j] = n;
}

int maze::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
    return map[i][j];
}

maze::maze(ifstream& fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
    fin >> rows;
    fin >> cols;

    char x;

    value.resize(rows, cols);
    for (int i = 0; i <= rows - 1; i++)
        for (int j = 0; j <= cols - 1; j++)
        {
            fin >> x;
            if (x == 'O')
                value[i][j] = true;
            else
                value[i][j] = false;
        }

    map.resize(rows, cols);
    for (int i = 0; i <= rows - 1; i++)
        for (int j = 0; j <= cols - 1; j++)
        {
            if (value[i][j])
            {
                map[i][j] = nodeID;
                nodeID++;
            }
        }
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
    cout << endl;

    if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
        throw rangeError("Bad value in maze::print");

    if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
        throw rangeError("Bad value in maze::print");

    for (int i = 0; i <= rows - 1; i++)
    {
        for (int j = 0; j <= cols - 1; j++)
        {
            if (i == goalI && j == goalJ)
                cout << "*";
            else
                if (i == currI && j == currJ)
                    cout << "+";
                else
                    if (value[i][j])
                        cout << " ";
                    else
                        cout << "X";
        }
        cout << endl;
    }
    cout << endl;
}

// Just used to check if map was working
void maze::printMap()
{
    for (int i = 0; i <= rows - 1; i++)
    {
        for (int j = 0; j <= cols - 1; j++)
        {
            if (value[i][j])
                cout << map[i][j];
            else
                cout << "X";
        }
        cout << endl;
    }
    cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze.
{
    if (i < 0 || i > rows || j < 0 || j > cols)
        throw rangeError("Bad value in maze::isLegal");

    return value[i][j];
}

void maze::mapMazeToGraph(graph& g)
// Create a graph g that represents the legal moves in the maze m.
{
    // Step 1 - scan maze and call addNode() funciton for each open space
    for (int i = 0; i < rows; i++) 
    {
        for (int j = 0; j < cols; j++)
        {
            if (isLegal(i, j) == true)
            {
                g.addNode(map[i][j]);
                numNodes++;
            }   
        }
    }
    // Step 2 - scan graph for legal moves (look up down left and right) call addEdge() 
    // function for legal moves also add egde for backtracking (edge to and from neighbors)
    int n1, n2;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (isLegal(i, j))
            {
                for (int dir = 0; dir < 4; dir++)
                {
                    int Xpos = i;
                    int Ypos = j;

                    switch (dir)
                    {
                        //up
                    case 0:
                        Ypos--;

                        if (Ypos < 0)
                            break;

                        if (isLegal(Xpos, Ypos))
                        {
                            n1 = getMap(i, j) - 1;
                            n2 = getMap(Xpos, Ypos) - 1;
                            g.addEdge(n1, n2);
                        }
                        break;

                        //right
                    case 1:
                        Xpos++;

                        if (Xpos >= cols)
                            break;

                        if (isLegal(Xpos, Ypos))
                        {
                            n1 = getMap(i, j) - 1;
                            n2 = getMap(Xpos, Ypos) - 1;
                            g.addEdge(n1, n2);
                        }
                        break;

                        //down
                    case 2:
                        Ypos++;

                        if (Ypos >= rows)
                            break;

                        if (isLegal(Xpos, Ypos))
                        {
                            n1 = getMap(i, j) - 1;
                            n2 = getMap(Xpos, Ypos) - 1;
                            g.addEdge(n1, n2);
                        }
                        break;

                        //left
                    case 3:
                        Xpos--;

                        if (Xpos < 0)
                            break;

                        if (isLegal(Xpos, Ypos))
                        {
                            n1 = getMap(i, j) - 1;
                            n2 = getMap(Xpos, Ypos) - 1;
                            g.addEdge(n1, n2);
                        }
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }
}

void maze::findPathRecursive()
{

}

void maze::findPathNonRecursive()
{

}


int main()
{
    char x;
    ifstream fin;

    // Read the maze from the file.
    string fileName = "maze.txt";

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }

    try
    {

        graph g;
        while (fin && fin.peek() != 'Z')
        {
            maze m(fin);
            mazes++;
            m.print(m.getRows()-1, m.getCols()-1, 0, 0);
            m.printMap();
            m.mapMazeToGraph(g);
            cout << g;
            cout << "Maze Printed, press ENTER to continue..." << endl;
            cin.ignore();

        }
    }
    catch (indexRangeError& ex)
    {
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError& ex)
    {
        cout << ex.what() << endl; exit(1);
    }
}