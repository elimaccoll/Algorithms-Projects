// Project 3 - Part A
// Anthony Cherubino, Eli MacColl
// 
// Main program file for Project 3 - Part B. Contains the code to play the 
// card game flip.
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "d_matrix.h"

using namespace std;

// Class Declarations

class dictionary
	// Dictionary class contains a vector of words read in from a file. Constructor reads
	// strings in from a passed in file into a vector and sorts them using the selection sort
	// member function. Also contains a function to search through the vector for a specific
	// passed in word and an overloaded << operator which prints out the list of words.
{
public:
	dictionary(string dictPath);
	bool checkDict(string word);
	friend ostream& operator<<(ostream& os, const dictionary fullDict)
		// overloads << operator to print out words in dictionary
	{

		// iterates through the words in the dictionary vector and prints them out
		os << endl;
		for (size_t i = 0; i < fullDict.words.size(); i++)
		{
			os << fullDict.words.at(i) << endl;
		}
		return os;
	}

private:
	vector<string> words;
	vector<string> inputDict(string dictPath);
	void selectionSort();

};

class grid
	// Grid class contains a matrix of characters read in from a file. Constructor reads in
	// characters from a pass in file into a matrix of a size contained within the file being.
	// The matrix is resized to these dimensions and filled with the read in characters. Also
	// contains an overloaded << operator to print out the grid matrix of the characters.
{
public:
	grid(string gridPath);
	size_t getRows();
	size_t getCols();
	char getAt(size_t i, size_t j);
	friend ostream& operator<<(ostream& os, const grid fullGrid)
		// overloads << operator to print out characters in grid
	{

		// iterates through the characters in the grid matrix and prints them out
		cout << "\nRows: " << fullGrid.charMatrix.rows() << "\nColumns: " << fullGrid.charMatrix.rows() << endl;
		for (size_t i = 0; i < fullGrid.charMatrix.rows(); i++)
		{
			for (size_t j = 0; j < fullGrid.charMatrix.rows(); j++)
			{
				os << fullGrid.charMatrix[i][j] << ' ';
			}
			os << endl;
		}
		return os;
	}

private:
	matrix<char> charMatrix;
};

// global function declarations

void findMatches(dictionary wordDict, grid letterGrid);

void search();

ostream& operator<<(ostream& os, const vector<string> strVec);

// dictionary class member functions

dictionary::dictionary(string dictPath)
	// Constructor for dictionary class that is passed in a string containing the path
	// to a dictionary text file. The words from this file are stored in a vector and
	// sorted alphabetically
{
	words = inputDict(dictPath);
	cout << *this;
	selectionSort();

}

bool dictionary::checkDict(string word)
	// Function that searches for a passed in word in the dictionary vector of all words
{
	return binary_search(words.begin(), words.end(), word);
}

vector<string> dictionary::inputDict(string dictPath)
	// Function that takes in a path to a dictionary text file and reads in each word,
	// stores them in a vector, and then returns that vector
{
	ifstream input(dictPath);
	istream_iterator<string> start(input), end;
	vector<string> inFile(start, end);
	return inFile;
}

void dictionary::selectionSort()
	// Function that sorts the dictionary vector alphabetically using a selection sort
{
	size_t vecSize = words.size();
	cout << "\nSorting dictionary alphabetically: " << endl;
	cout << vecSize;
	
	for (size_t j = 0; j < vecSize - 1; ++j) {
		
		// print an updating count of how many words left to sort
		cout << "\r" << vecSize - j - 1;
		size_t min = j;
		
		for (size_t i = j + 1; i < vecSize; ++i) {
			
			if (words.at(min) > words.at(i)) {
				min = i;
			}
		}
		
		if (min != j)
		{
			swap(words.at(j), words.at(min));
		}
	}

	cout << "\rDone!";
}

// grid class member functions

grid::grid(string gridPath)
// Constructor for grid class that is passed in a string containing the path
// to a input grid text file. The characters from this file are stored in a
// matrix.
{
	ifstream input(gridPath);
	istream_iterator<string> start(input), end;
	vector<string> inFile(start, end);
	charMatrix.resize(stoi(inFile.at(0)), stoi(inFile.at(1)));
	size_t x = 2;

	for (size_t i = 0; i < charMatrix.rows(); i++)
	{
		for (size_t j = 0; j < charMatrix.cols(); j++)
		{
			charMatrix[i][j] = (*inFile.at(x).c_str());
			x++;
		}
	}
}

size_t grid::getRows()
{
	return charMatrix.rows();
}

size_t grid::getCols()
{
	return charMatrix.cols();
}

char grid::getAt(size_t i, size_t j)
{
	return charMatrix[i][j];
}

// global functions

void findMatches(dictionary wordDict, grid letterGrid)
// Function checks increasing letters around each character in the grid against dictionary
// to see if any words are made. If a word is found and it has not already been found before,
// it is added to vector and all found words are printed out at the end. 
{
	vector<string> foundWords;
	size_t rows = letterGrid.getRows();
	size_t cols = letterGrid.getCols();
	
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			string baseLetter;
			baseLetter += letterGrid.getAt(i, j);

			if (wordDict.checkDict(baseLetter))
			{
				if (find(foundWords.begin(), foundWords.end(), baseLetter) == foundWords.end())
				{
					foundWords.push_back(baseLetter);
				}
			}

			for (size_t dir = 0; dir < 8; dir++)
			{
				string wordBuilder = baseLetter;
				int coordY = j;
				int coordX = i;

				switch (dir)
				{

					// up 
				case 0:
					for (size_t size = 1; size < rows; size++)
					{
						coordY--;

						if (coordY < 0)
						{
							coordY = rows - 1;
						}

						wordBuilder += letterGrid.getAt(coordX, coordY);

						if (wordDict.checkDict(wordBuilder))
						{
							if (find(foundWords.begin(), foundWords.end(), wordBuilder) == foundWords.end())
							{
								foundWords.push_back(wordBuilder);
							}
						}
					}
					break;

					// up & right
				case 1:
					for (size_t size = 1; size < rows && size < cols; size++)
					{
						coordY--;
						coordX++;

						if (coordY < 0)
						{
							coordY = rows - 1;
						}

						if (coordX >= cols)
						{
							coordX = 0;
						}

						wordBuilder += letterGrid.getAt(coordX, coordY);

						if (wordDict.checkDict(wordBuilder))
						{
							if (find(foundWords.begin(), foundWords.end(), wordBuilder) == foundWords.end())
							{
								foundWords.push_back(wordBuilder);
							}
						}
					}
					break;

					// right
				case 2:
					for (size_t size = 1; size < cols; size++)
					{
						coordX++;

						if (coordX >= cols)
						{
							coordX = 0;
						}

						wordBuilder += letterGrid.getAt(coordX, coordY);

						if (wordDict.checkDict(wordBuilder))
						{
							if (find(foundWords.begin(), foundWords.end(), wordBuilder) == foundWords.end())
							{
								foundWords.push_back(wordBuilder);
							}
						}
					}
					break;

					// down & right
				case 3:
					for (size_t size = 1; size < rows && size < cols; size++)
					{
						coordY++;
						coordX++;

						if (coordY >= rows)
						{
							coordY = 0;
						}

						if (coordX >= cols)
						{
							coordX = 0;
						}

						wordBuilder += letterGrid.getAt(coordX, coordY);

						if (wordDict.checkDict(wordBuilder))
						{
							if (find(foundWords.begin(), foundWords.end(), wordBuilder) == foundWords.end())
							{
								foundWords.push_back(wordBuilder);
							}
						}
					}
					break;

					// down
				case 4:
					for (size_t size = 1; size < rows; size++)
					{
						coordY++;

						if (coordY >= rows)
						{
							coordY = 0;
						}

						wordBuilder += letterGrid.getAt(coordX, coordY);

						if (wordDict.checkDict(wordBuilder))
						{
							if (find(foundWords.begin(), foundWords.end(), wordBuilder) == foundWords.end())
							{
								foundWords.push_back(wordBuilder);
							}
						}
					}
					break;

					// down & left
				case 5:
					for (size_t size = 1; size < rows && size < cols; size++)
					{
						coordY++;
						coordX--;

						if (coordY >= rows)
						{
							coordY = 0;
						}

						if (coordX < 0)
						{
							coordX = cols - 1;
						}

						wordBuilder += letterGrid.getAt(coordX, coordY);

						if (wordDict.checkDict(wordBuilder))
						{
							if (find(foundWords.begin(), foundWords.end(), wordBuilder) == foundWords.end())
							{
								foundWords.push_back(wordBuilder);
							}
						}
					}
					break;

					// left
				case 6:
					for (size_t size = 1; size < cols; size++)
					{
						coordX--;

						if (coordX < 0)
						{
							coordX = cols - 1;
						}

						wordBuilder += letterGrid.getAt(coordX, coordY);

						if (wordDict.checkDict(wordBuilder))
						{
							if (find(foundWords.begin(), foundWords.end(), wordBuilder) == foundWords.end())
							{
								foundWords.push_back(wordBuilder);
							}
						}
					}
					break;

					// left & up
				case 7:
					for (size_t size = 1; size < rows && size < cols; size++)
					{
						coordY--;
						coordX--;

						if (coordY < 0)
						{
							coordY = rows - 1;
						}

						if (coordX < 0)
						{
							coordX = cols - 1;
						}

						wordBuilder += letterGrid.getAt(coordX, coordY);

						if (wordDict.checkDict(wordBuilder))
						{
							if (find(foundWords.begin(), foundWords.end(), wordBuilder) == foundWords.end())
							{
								foundWords.push_back(wordBuilder);
							}
						}
					}
					break;
				
				default:
					break;
				}
			}
		}
	}

	cout << foundWords;
}

void search()
// Function initializes dictionary from file, prompts user to enter file name
// to be used to initialize grid, and initiates search
{
	dictionary searchDict("dictionary.txt");
	cout << "\n" << searchDict;

	cout << "\nEnter grid file name: ";
	string input;
	cin >> input;

	grid wordSearch(input);
	cout << wordSearch;

	findMatches(searchDict, wordSearch);
}

ostream& operator<<(ostream& os, const vector<string> strVec)
// overloads << operator to print out vectors of strings
{

	// iterates through the strings in the vector and prints them out
	os << endl;
	for (size_t i = 0; i < strVec.size(); i++)
	{
		os << strVec.at(i) << endl;
	}
	return os;
}

// main function

int main()
// main function initiates word search
{
	search();

	return 0;
}