// Project 5 - Part A
// Anthony Cherubino, Eli MacColl
// 
// Main program file for Project 5 - Part A. Contains the code to read in mazes
// and solve them Recursively and Non Recursively

#include <iostream>
#include <list>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"

using namespace std;

int mazes = 0;

class maze
    // Contains functions to store, print, and solve the mazes
{
public:
    maze(ifstream& fin);
    void print(int, int, int);
    bool isLegal(int i, int j);

    void setMap(int i, int j, int n);
    int getMap(int i, int j) const;
    void mapMazeToGraph(graph& g);

    void newEdge(graph&, int, int, int, int);
    void delEdge(graph&, int, int, int, int);

    int getRows();
    int getCols();

    void printMap();

    void clearEdges(graph&);

    void findPathRecursive(graph&, int);
    void findPathNonRecursive(graph&, int);

    bool SolvedR(int);
    void SolvedNR(int);

private:
    int rows; // number of rows in the maze
    int cols; // number of columns in the maze

    int nodeVal = 1;
    int numNodes = 0;
    int numEdges = 0;
    int numVisit = 0;

    int curr=1; // Current location in the maze in terms of map matrix
    int goal;   // End goal of maze location in terms of map matrix

    list<int>* adj; // Pointer for adjacency list used in findPathNonRecursive

    matrix<bool> value;
    matrix<int> map;      // Mapping from maze (i,j) values to node index values

};

int maze::getRows()
// Returns number of rows in the maze
{
    return rows;
}

int maze::getCols()
// Returns number of columns in the maze
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

    // Creates the value matrix which marks open cells as 1 and walls 'X' as 0
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

    // Creates the map matrix which marks the open cells with a node value
    map.resize(rows, cols);
    for (int i = 0; i <= rows - 1; i++)
        for (int j = 0; j <= cols - 1; j++)
        {
            if (value[i][j])
            {
                map[i][j] = nodeVal;
                nodeVal++;
            }
        }

    // Initializes the goal of the current maze
    goal = map[rows - 1][cols - 1];
}


void maze::print(int goalI, int goalJ, int currNode)
// Print out a maze, with the goal and current location marked on the board
{
    cout << endl;

    if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
        throw rangeError("Bad value in maze::print");

    if (currNode < 0 || currNode > numNodes)
        throw rangeError("Bad value in maze::print");

    for (int i = 0; i <= rows - 1; i++)
    {
        for (int j = 0; j <= cols - 1; j++)
        {
            if (map[i][j] - 1 == currNode)
                cout << "+";
            else
                if (i == goalI && j == goalJ)
                {
                    cout << "*";
                }
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

void maze::printMap()
// Print out a maze displaying the map matrix - used for testing
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

void maze::delEdge(graph& g, int i, int j, int Ypos, int Xpos)
// Removes an edge between two nodes.  Used in the process of clearing all 
// edges in the clearEdges function once a maze is solved
{
    int n1, n2;
    if (isLegal(Ypos, Xpos))
    {
        n1 = getMap(i, j) - 1;
        n2 = getMap(Ypos, Xpos) - 1;
        g.removeEdge(n1, n2);
        numEdges--;
    }
}
void maze::clearEdges(graph& g)
// Iterates through the nodes used to solve the maze, identifies and has
// edges removed in order to continue to the next maze
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (map[i][j] > 0)
            {
                for (int dir = 0; dir < 4; dir++)
                {
                    int Xpos = j;
                    int Ypos = i;
                    switch (dir)
                    {
                        //up
                    case 0:
                        Ypos--;

                        if (Ypos < 0)
                            break;

                        delEdge(g, i, j, Ypos, Xpos);
                        break;

                        //right
                    case 1:
                        Xpos++;

                        if (Xpos >= cols)
                            break;

                        delEdge(g, i, j, Ypos, Xpos);
                        break;

                        //down
                    case 2:
                        Ypos++;

                        if (Ypos >= rows)
                            break;

                        delEdge(g, i, j, Ypos, Xpos);
                        break;

                        //left
                    case 3:
                        Xpos--;

                        if (Xpos < 0)
                            break;

                        delEdge(g, i, j, Ypos, Xpos);
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze.
{
    if (i < 0 || i > rows || j < 0 || j > cols)
        throw rangeError("Bad value in maze::isLegal");

    return value[i][j];
}

void maze::newEdge(graph& g, int i, int j, int Ypos, int Xpos)
// Creates an edge between two nodes.  Used in the process of converting
// the map matrix to a graph that is used to solve the maze
{
    int n1, n2;
    if (isLegal(Ypos, Xpos))
    {
        n1 = getMap(i, j) - 1;
        n2 = getMap(Ypos, Xpos) - 1;
        g.addEdge(n1, n2, numEdges);
        adj[n1].push_back(n2);
        numEdges++;
    }
}
void maze::mapMazeToGraph(graph& g)
// Create a graph g that represents the legal moves in the maze m.
{
    // Scan maze and create a node for each open space
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

    int nodes = numNodes;
    adj = new list<int>[numNodes];

    // Scan graph for legal moves (look up, down, left, and right) create an edge 
    // between nodes for each legal move
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (map[i][j]>0)
            {
                for (int dir = 0; dir < 4; dir++)
                {
                    int Xpos = j;
                    int Ypos = i;
                    switch (dir)
                    {
                        //up
                    case 0:
                        Ypos--;

                        if (Ypos < 0)
                            break;

                        newEdge(g, i, j, Ypos, Xpos);
                        break;

                        //right
                    case 1:
                        Xpos++;

                        if (Xpos >= cols)
                            break;

                        newEdge(g, i, j, Ypos, Xpos);
                        break;
                    
                        //down
                    case 2:
                        Ypos++;

                        if (Ypos >= rows)
                            break;

                        newEdge(g, i, j, Ypos, Xpos);
                        break;

                        //left
                    case 3:
                        Xpos--;

                        if (Xpos < 0)
                            break;

                        newEdge(g, i, j, Ypos, Xpos);
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }
}

bool maze::SolvedR(int vertex)
// Checks to see if the maze is solved for the Recursive path finder
{
    if (vertex+1 == goal)
    {
        print(rows - 1, cols - 1, vertex);
        cout << "Maze #" << mazes << " Solved Recursively!" << endl;
        return true;
    }
    return false;
}

void maze::findPathRecursive(graph& g, int vertex)
// Recursive function that uses recursive DFS to solve the current maze.
// Checks every node that has an edge with the current position, and if it
// hasn't been visited, calls itself to move to that node and repeat the process.
// Runs until maze is solved or every node besides the goal has been visited, 
// meaning no path exists to solve the maze. It then clears all edges and marks
// all nodes as not visited in order to solve the next maze.
{
    if (!SolvedR(vertex))
    {
        g.visit(vertex);
        print(rows - 1, cols - 1, vertex);
        int n;
        numVisit = 0;

        for (int i = 0; i < numNodes; i++)
        {
            if (g.isVisited(i))
            {
                numVisit++;
                if (numVisit == goal - 1)
                {
                    cout << "No Path Exists for Maze #" << mazes << endl;
                    clearEdges(g);
                    g.clearVisit();
                    numVisit = 0;
                }
            }
        }

        for (int i = 0; i < numNodes; i++)
        {
            if (g.isEdge(vertex, i))
            {
                n = i;
                if (!g.isVisited(n))
                {
                    findPathRecursive(g, n);
                }
            }
        }
    }
    else
    {
        clearEdges(g);
        g.clearVisit();
        numVisit = 0;
    }
}

void maze::SolvedNR(int vertex)
// Checks to see if the maze is solved for the Non Recursive path finder
{
    if (vertex + 1 == goal)
    {
        print(rows - 1, cols - 1, vertex);
        cout << "Maze #" << mazes << " Solved Non Recursively!" << endl;
    }
    else
        cout << "No Path Exists for Maze #"<<mazes<< endl;
}

void maze::findPathNonRecursive(graph& g, int v)
// Non Recursive function that uses Queue-based BFS to solve the current maze.
// Checks each unvisited, adjacent node of current node, marking them as visited, 
// adding them to the queue, and removing current node from queue. Runs until the 
// queue is empty and then checks if the maze was solved or if no path exists by 
// calling the SolvedNR function.
{
    g.clearVisit();
    list<int> q;

    g.visit(v);
    q.push_back(v);

    list<int>::iterator i;

    while (!q.empty())
    {
        print(rows - 1, cols - 1, v);

        v = q.front();
        q.pop_front();

        for (i = adj[v].begin(); i != adj[v].end(); i++) 
        {
            if (!g.isVisited(*i))
            {
                g.visit(*i);
                q.push_back(*i);
            }
        }
    }
    SolvedNR(v);
    g.clearVisit();
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
            // Loads new maze once previous one was solved. Solves the maze recursively 
            // and then Non Recurisvely before moving onto the next maze
        {
            maze m(fin);
            mazes++;

            m.mapMazeToGraph(g);

            cout << "----- Solving Maze #"<<mazes<< " Recursively -----" << endl;
            cout << "Press ENTER to begin..." << endl;
            cin.ignore();
            m.findPathRecursive(g, 0);
            cout << "Press ENTER to continue..." << endl;
            cin.ignore();

            cout << "----- Solving Maze #" << mazes << " Non Recursively -----" << endl;
            cout << "Press ENTER to begin..." << endl;
            cin.ignore();
            m.findPathNonRecursive(g, 0);
            cout << "Press ENTER to continue..." << endl;
            cin.ignore();
        }
        cout << "All mazes solved!"<<endl;
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