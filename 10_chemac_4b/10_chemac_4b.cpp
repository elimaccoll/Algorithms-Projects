// Project 4 - Part B
// Anthony Cherubino, Eli MacColl
// 
// Main program file for Project 4 - Part B. Contains the code to read in Sudoku
// puzzles and solve them, outputting the amount of recursive calls needed.

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <Windows.h>
#include "d_matrix.h"
#include "d_except.h"

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank

const int SquareSize = 3;  //  The number of cells in a small square
						   //  (usually 3).  The board has
						   //  SquareSize^2 rows and SquareSize^2
						   //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;
int boards = 0;
int totalCalls = 0;

class board
	// Stores the entire Sudoku board
{
public:
	board(int);
	void clear();
	void initialize(ifstream& fin);
	void print();
	void printConflicts();
	bool isBlank(int, int);
	bool isConflicts();
	bool isSolved();
	ValueType getCell(int, int);
	void setCell(int, int, int);
	void resetCell(int, int);
	void updateConflicts();
	vector<int> findBlank();
	vector<vector<int>> updatePoss();
	vector<int> inputUnique();
	bool solve();
	int calls = 0;

private:

	vector<vector<int>> conflicts;

	// The following matrices go from 1 to BoardSize in each
	// dimension, i.e., they are each (BoardSize+1) * (BoardSize+1)
	matrix<ValueType> value;
};

board::board(int sqSize)
	: value(BoardSize + 1, BoardSize + 1)
	// Board constructor
{
	clear();
}

void board::clear()
// Mark all possible values as legal for each board entry
{
	for (int i = 1; i <= BoardSize; i++)
		for (int j = 1; j <= BoardSize; j++)
		{
			value[i][j] = Blank;
		}
}

void board::initialize(ifstream& fin)
// Read a Sudoku board from the input file.
{
	char ch;

	clear();

	for (int i = 1; i <= BoardSize; i++)
		for (int j = 1; j <= BoardSize; j++)
		{
			fin >> ch;

			// If the read char is not Blank
			if (ch != '.')
				setCell(i, j, ch - '0');   // Convert char to int
		}

	updateConflicts();
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
	// Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
	// coordinates of the square that i,j is in.  

	return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

ostream& operator<<(ostream& ostr, vector<int>& v)
// Overloaded output operator for vector class.
{

	for (int i = 0; i < v.size(); i++)
		ostr << v[i] << " ";
	ostr << endl;
	return ostr;
}

void board::setCell(int i, int j, int num)
// Adds a value to a given cell and updates conflicts.
{
	value[i][j] = num;
	updateConflicts();
}

ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
	if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
		return value[i][j];
	else
		throw rangeError("bad value in getCell");
}

void board::resetCell(int i, int j)
// Clears the value of a cell and updates conflicts.
{
	value[i][j] = Blank;
	updateConflicts();
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
		throw rangeError("bad value in setCell");

	return (getCell(i, j) == Blank);
}

bool board::isConflicts()
// Returns true if there are any conflicts, and false otherwise.
{
	for (size_t i = 0; i < conflicts.size(); i++)
	{
		if (!conflicts[i].empty())
		{
			return true;
		}
	}
	return false;
}

bool board::isSolved()
// Returns true if there are no conflicts and all cells are filled, and false otherwise.
{
	if (!isConflicts())
	{
		for (size_t i = 1; i < BoardSize + 1; i++)
		{
			for (size_t j = 1; j < BoardSize + 1; j++)
			{
				if (isBlank(i,j))
				{
					cout << "\nNot Solved"<<endl;
					return false;
				}
			}
		}
		cout << "\nSolved"<<endl;
		return true;
	}
	else
	{
		cout << "\nNot Solved"<<endl;
		return false;
	}
}

void board::print()
// Prints the current board.
{
	cout << "\nBoard " << boards;
	cout << "\n";
	for (int i = 1; i <= BoardSize; i++)
	{
		if ((i - 1) % SquareSize == 0)
		{
			cout << " -";
			for (int j = 1; j <= BoardSize; j++)
				cout << "---";
			cout << "-";
			cout << endl;
		}
		for (int j = 1; j <= BoardSize; j++)
		{
			if ((j - 1) % SquareSize == 0)
				cout << "|";
			if (!isBlank(i, j))
				cout << " " << getCell(i, j) << " ";
			else
				cout << "   ";
		}
		cout << "|";
		cout << endl;
	}

	cout << " -";
	for (int j = 1; j <= BoardSize; j++)
		cout << "---";
	cout << "-";
}

void board::printConflicts()
// Prints out all conflicts found in the current board
{
	if (isConflicts())
	{
		cout << "\nConflicts";

		for (size_t i = 0; i < conflicts.size(); i++)
		{
			if (!conflicts[i].empty())
			{
				if (i < BoardSize)
				{
					cout << "\nColumn " << i + 1 << ": ";
				}
				else if (i < 2 * BoardSize)
				{
					cout << "\nRow    " << i - BoardSize + 1 << ": ";
				}
				else
				{
					cout << "\nSquare  " << i - (2 * BoardSize) + 1 << ": ";
				}

				for (size_t j = 0; j < conflicts[i].size(); j++)
				{
					cout << conflicts[i][j] << " ";
				}
			}
		}
	}
	else
	{
		cout << "\nNo Conflicts";
	}
}

void board::updateConflicts()
// Checks the matrix for any conflicts and stores those that it finds
{
	conflicts.clear();

	// checks for conflicts in each column
	for (size_t j = 1; j < BoardSize + 1; j++)
	{
		vector<int> column;

		for (size_t i = 1; i < BoardSize + 1; i++)
		{
			if (!isBlank(i, j))
			{
				column.push_back(value[i][j]);
			}
		}

		vector<int> dupes;

		if (column.size() > 1)
		{
			sort(column.begin(), column.end());

			for (size_t x = 1; x < column.size(); x++)
			{
				if (column[x - 1] == column[x])
				{
					dupes.push_back(column[x]);
				}
			}

			if (dupes.size() > 1)
			{
				unique(dupes.begin(), dupes.end());
			}
		}

		conflicts.push_back(dupes);
	}

	// checks for conflicts in each row
	for (size_t i = 1; i < BoardSize + 1; i++)
	{
		vector<int> row;

		for (size_t j = 1; j < BoardSize + 1; j++)
		{
			if (!isBlank(i, j))
			{
				row.push_back(value[i][j]);
			}
		}

		vector<int> dupes;

		if (row.size() > 1)
		{
			sort(row.begin(), row.end());

			for (size_t x = 1; x < row.size(); x++)
			{
				if (row[x - 1] == row[x])
				{
					dupes.push_back(row[x]);
				}
			}

			if (dupes.size() > 1)
			{
				unique(dupes.begin(), dupes.end());
			}
		}

		conflicts.push_back(dupes);
	}

	// checks for conflicts in each square
	for (size_t x = 0; x < BoardSize; x++)
	{
		vector<int> square;

		for (size_t i = 1 + (3 * (x / 3)); i < (BoardSize / 3) + (3 * (x / 3)) + 1; i++)
		{
			for (size_t j = 1 + (3 * (x % 3)); j < (BoardSize / 3) + (3 * (x % 3)) + 1; j++)
			{
				if (!isBlank(i, j))
				{
					square.push_back(value[i][j]);
				}
			}
		}

		vector<int> dupes;

		if (square.size() > 1)
		{
			sort(square.begin(), square.end());

			for (size_t i = 1; i < square.size(); i++)
			{
				if (square[i - 1] == square[i])
				{
					dupes.push_back(square[i]);
				}
			}

			if (dupes.size() > 1)
			{
				unique(dupes.begin(), dupes.end());
			}
		}

		conflicts.push_back(dupes);
	}
}

vector<int> board::findBlank()
// Find the first blank cell and returns its coordinates
// If all cells are filled, returns -1
{
	vector<int> coords;

	for (size_t i = 1; i < BoardSize + 1; i++)
	{
		for (size_t j = 1; j < BoardSize + 1; j++)
		{
			if (isBlank(i, j))
			{
				coords.push_back(i);
				coords.push_back(j);
				return coords;
			}
		}
	}

	coords.push_back(-1);
	return coords;
}

vector<vector<int>> board::updatePoss()
// determines and returns all the possible numbers that could go in each empty cell based on the current board state
// returns -1 for all filled cells
{
	vector<vector<int>> poss;

	for (size_t i = 1; i < BoardSize + 1; i++)
	{
		for (size_t j = 1; j < BoardSize + 1; j++)
		{
			if (isBlank(i, j))
			{
				vector<int> temp;

				for (size_t x = 1; x < 10; x++)
				{
					setCell(i, j, x);
					updateConflicts();
					
					if (!isConflicts())
					{
						temp.push_back(x);
					}
				}
				
				poss.push_back(temp);
				setCell(i, j, Blank);
			}
			else
			{
				vector<int> temp;
				temp.push_back(-1);
				poss.push_back(temp);
			}
		}
	}

	return poss;
}

vector<int> board::inputUnique()
// inputs the correct number in any cell which either has only one possible value or only one possible value that all other cells in its column, row, or square do not have
// returns the coordinates of any cell that has been altered with the trailing value being the coordinates of the cell with the fewest multiple possible values
// if any cell has no possible values, returns -1
{
	vector<vector<int>> poss = updatePoss();

	for (size_t i = 1; i < BoardSize + 1; i++)
	{
		for (size_t j = 1; j < BoardSize + 1; j++)
		{
			int index = 9 * (i - 1) + (j - 1);
			
			if (isBlank(i, j))
			{
				vector<int> uniquePoss;
				uniquePoss = poss[index];

				if (uniquePoss.empty())
				{
					vector<int> inputs;
					inputs.push_back(-1);
					return inputs;
				}
				else if (uniquePoss.size() == 1)
				{
					setCell(i, j, uniquePoss.front());
					vector<int> input;
					input.push_back(i);
					input.push_back(j);
					return input;
				}

				for (size_t x = 0; x < 27; x++)
				{
					int colIndex = 9 * (i - 1) + x;
					int rowIndex = 9 * (x - 9) + (j - 1);
					int squareIndex = 9 * ((x - 18) / 3) + ((x - 18) % 3);
					vector<int> tempPoss;

					if (!uniquePoss.empty())
					{
						if (x < 9 && x != j - 1)
						{
							set_difference(uniquePoss.begin(), uniquePoss.end(), poss[colIndex].begin(), poss[colIndex].end(), inserter(tempPoss, tempPoss.begin()));
							uniquePoss = tempPoss;
						}

						if (x >= 9 && x < 18 && x - 9 != i - 1)
						{
							set_difference(uniquePoss.begin(), uniquePoss.end(), poss[rowIndex].begin(), poss[rowIndex].end(), inserter(tempPoss, tempPoss.begin()));
							uniquePoss = tempPoss;
						}

						if (x >= 18 && (x - 18) % 3 != j - 1 && (x - 18) / 3 != i - 1)
						{
							set_difference(uniquePoss.begin(), uniquePoss.end(), poss[squareIndex].begin(), poss[squareIndex].end(), inserter(tempPoss, tempPoss.begin()));
							uniquePoss = tempPoss;
						}
					}
				}

				if (uniquePoss.size() == 1)
				{
					setCell(i, j, uniquePoss.front());
					vector<int> input;
					input.push_back(i);
					input.push_back(j);
					return input;
				}
			}
		}
	}
	
	vector<int> input;
	input.push_back(-2);
	vector<int> sizes;
	
	for (size_t i = 0; i < poss.size(); i++)
	{
		if (poss[i].front() == -1)
		{
			sizes.push_back(10);
		}
		else
		{
			sizes.push_back(poss[i].size());
		}
	}

	auto min = min_element(sizes.begin(), sizes.end());
	int minIndex = distance(sizes.begin(), min);
	input.push_back((minIndex / 9) + 1);
	input.push_back((minIndex % 9) + 1);
	
	return input;
}

bool board::solve()
// recursive function that inputs possible values 1-9 in cell that has the fewest multiple possible values
// re-checks possible values for every cell after any change
// recursively calls itself to change next value
// if board becomes unsolvable, it will back up through previous steps until an untested value can be tried
{	
	calls++;
	totalCalls++;
	system("cls");
	print();
	bool repeat = true;
	vector<int> coords;
	vector<vector<int>> changes;

	while (repeat)
	{
		changes.push_back(inputUnique());
		system("cls");
		print();

		if (changes.back().empty())
		{
			changes.pop_back();
			repeat = false;
		}
		else if (changes.back().front() == -1)
		{
			changes.pop_back();
			for (size_t i = 0; i < changes.size(); i++)
			{
				setCell(changes[i].front(), changes[i].back(), Blank);
			}
			return false;
		}
		else if (changes.back().front() == -2)
		{
			coords.push_back(changes.back().at(1));
			coords.push_back(changes.back().at(2));
			changes.pop_back();
			repeat = false;
		}
	}

	if (findBlank().front() != -1)
	{
		int x = 1;
		
		while (true) {
			do
			{
				setCell(coords[0], coords[1], x);
				x++;
			} while (x != 10 && isConflicts());

			if (isConflicts())
			{
				setCell(coords[0], coords[1], Blank);
				for (size_t i = 0; i < changes.size(); i++)
				{
					setCell(changes[i].front(), changes[i].back(), Blank);
				}
				return false;
			}

			if (solve())
			{
				return true;
			}
			else if (x == 10)
			{
				setCell(coords[0], coords[1], Blank);
				for (size_t i = 0; i < changes.size(); i++)
				{
					setCell(changes[i].front(), changes[i].back(), Blank);
				}
				return false;
			}
		}
	}
	
	return true;
}

int main()
{
	ifstream fin;

	// Read the sample grid from the file.
	string fileName = "sudokuB.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		board b1(SquareSize);

		while (fin && fin.peek() != 'Z')
		{
			b1.initialize(fin);
			boards++;
			b1.print();
			b1.printConflicts();
			if (!b1.isSolved())
			{
				b1.solve();
			}
			b1.isSolved();
			cout << "\nRecursive Calls for this board: " << b1.calls;
			cout << "\nPress ENTER to continue...";
			cin.ignore();
		}

		cout << "\nAll boards solved!";
		cout << "\nTotal Recursive Calls: " << totalCalls;
		cout << "\nAverage Recursive Calls per board: " << totalCalls / boards;
	}
	catch (indexRangeError& ex)
	{
		cout << ex.what() << endl;
		exit(1);
	}
}