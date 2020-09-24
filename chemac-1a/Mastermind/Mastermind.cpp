// Project 1 - Part A
// Anthony Cherubino, Eli MacColl
// 
// Main program file for Project 1 - Part A. Contains the code to play a game
// of Mastermind against the computer.

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
using namespace std;

class code
// Code class containing the digits and range of the code as well as both the
// original code and a backup copy. Contains a constructor and functions to
// generate a code randomly, receive a code input by the user, check the digits
// of two codes against each other for both correct digit & correct place and
// correct digit but incorrect place, and format the code to be printed out to
// the user.
{
private:
	int digits, range;
	vector<int> numCode;
	vector<int> numCodeCopy;

public:
	code(int, int);
	void generateCode();
	int inputCode(string);
	int checkCorrect(code);
	int checkIncorrect(code);
	string formatCode();
};

code::code(int n, int m)
// Code constructor that initializes the digits and range of the code based on
// the user input data passed to it.
{
	digits = n;
	range = m;
}

void code::generateCode()
// Function which generates and stores a random code in both the 'numCode' and
// 'numCodeCopy' vectors with the number of digits and within the range determined
// by the values input by the user and initialized in the constructor. Used to
// generate the secret code which the user is trying to guess.
{
	// clears any values stored in the vector
	numCode.clear();

	// generates random numbers up to 'digits' in a range between 0 and 'range' - 1
	// and numbers are then added to the back of the 'numCode' vector
	for (int i = 0; i < digits; i++)
	{
		int codeVal = rand() % range;
		numCode.push_back(codeVal);
	}

	// contents of the now filled 'numCode' copied to 'numCodeCopy' as a backup
	// so original code is saved when the contents of 'numCode' are modified in
	// 'checkCorrect' function
	numCodeCopy = numCode;
}

int code::inputCode(string input)
// Function which takes in a user input string representing a guessed code with
// digits delimited by a comma. Numeric characters for each digit are separated,
// cast to integers, and added to the back of 'numCode' vector. Checks for a
// valid input with the correct amount of digits and returns 0 if correct,
// -1 if invalid input, and -2 if incorrect number of digits. Used to process 
// user input guess code.
{
	// clears any values stored in the vector (previous guesses)
	numCode.clear();
	
	// converts input string to stringstream in order to separate out the 
	// individual digits in the code using 'getline'
	stringstream inputSS(input);

	// iterates through the stringstream until the end is reached, converting 
	// each digit to an int and adding it to the back of the 'numCode' vector
	while (inputSS.good())
	{
		string substr;

		// separates each digit out delimited by commas
		getline(inputSS, substr, ',');

		// convert separated digit substring to int
		// if there is an invalid input this will catch the error and return -1
		try
		{
			numCode.push_back(stoi(substr));
		}
		catch (...)
		{
			return -1;
		}
	}

	// check if the input code has the same number of digits as the generated one
	// if not correct, return -2
	if (numCode.size() != digits)
	{
		return -2;
	}

	// contents of the now filled 'numCode' copied to 'numCodeCopy' as a backup
	// so original code is saved when the contents of 'numCode' are modified in
	// 'checkCorrect' function
	numCodeCopy = numCode;
	
	// return 0 to indicate success
	return 0;
}

int code::checkCorrect(code guess)
// Function which checks the secret code against the user input guessed code and
// returns the number of digits that are both present in the code and in the
// correct position. Replaces the digits that are correct so they aren't counted
// a second time in 'checkIncorrect'.
{
	// counter that keeps track of correct digits in the correct position
	int cc = 0;

	// iterates through the indices of both codes and checks them against each other
	for (int i = 0; i < digits; i++)
	{
		// replaces digit with placeholder 'e' and increments counter when they match
		if (guess.numCode[i] == numCode[i])
		{
			cc++;
			guess.numCode[i] = 'e';
			numCode[i] = 'e';
			
		}
	}

	// returns value of the counter, this is the first value in the computer's
	// response when entering a guess
	return cc;
}

int code::checkIncorrect(code guess)
// Function which checks the secret code against the user input guessed code and
// returns the number of digits that are present in the code but in the wrong
// position.
{
	// counter that keeps track of correct digits in the incorrect position
	int ci = 0;

	// iterates through the indices of both codes separately and checks them
	// against each other
	for (int i = 0; i < digits; i++)
	{
		for (int j = 0; j < digits; j++)
		{
			// replaces digit with placeholder 'e' and increments counter when 
			// they match
			// does not increment if they are already 'e' from 'checkCorrect'
			if (guess.numCode[i] == numCode[j] && guess.numCode[i] != 'e')
			{
				ci++;
				guess.numCode[i] = 'e';
				numCode[j] = 'e';
			}
		}
	}

	// reset 'numCode' to be the originally generated code
	numCode = numCodeCopy;

	// returns value of the counter, this is the second value in the computer's
	// response when entering a guess
	return ci;
}

string code::formatCode()
// Function which formats the code as a string so it can be easily printed to 
// the user. String returns in the format '(X,X,X,...X)' for as many X as there
// are digits in the code.
{
	string output = "(";

	// iterate through the code and add every digit to the string in order
	for (int i = 0; i < digits; i++)
	{
		output += to_string(numCodeCopy[i]);

		// adds a comma after each digit in the code except the last one
		if (i != digits - 1)
		{
			output += ",";
		}
	}

	output += ")";
	return output;
}

int main()
// Main function where the user is prompted to enter the desired number of digits
// and range of values to generate the secret code with as well as enter their
// guess codes.
{
	// indicates whether the user has guessed the code successfully
	bool win = false;
	
	// keeps track of the number of attempts to guess the code
	int trys = 0;
	
	// stores the input values of digits and range, respectively
	int n, m;
	
	// stores the user input code guesses
	string input;
	
	// controls while loop repeating when the user inputs something invalid
	bool repeat;
	
	// stores the returned error code from 'inputCode'
	int err;

	// prompt user to enter desired amount of digits
	do
	{
		cout << "Enter the amount of digits in the code: ";
		cin >> n;

		// repeat prompt if value entered is less than or equal to 0
		if (n <= 0)
		{
			cout << "\nInvalid number of digits!\n\n";
			repeat = true;
		}
		else
		{
			repeat = false;
		}

	} while (repeat);

	// prompt user to enter desired range of digits
	do
	{
		cout << "Enter the exclusive upper bound of digits in the code: ";
		cin >> m;

		// repeat prompt if value entered is less than or equal to 0
		if (m <= 0)
		{
			cout << "\nInvalid upper bound!\n\n";
			repeat = true;
		}
		else
		{
			repeat = false;
		}

	} while (repeat);

	// initialize secret and guess code objects with input digit and range
	// values and generate secret code
	code secret(n, m);
	secret.generateCode();
	code guess(n, m);
	cout << "Code generated with (" << n << ") digits and" 
			"a range of (0-" << m - 1 << ")" << endl;

	// (for checking correct functionality)
	// uncomment the following line to have the secret code printed out at the start
	//cout << "The \"secret\" code is: " << secret.formatCode() << endl;

	// prompts the user to enter their guess code and loops until either the correct
	// code is entered or user runs out of guesses
	while (trys < 10 && win == false)
	{
		// outputs the remaining number of guesses counting down from 10
		cout << "\nYou have " << 10 - trys << " guesses remaining" << endl;

		// prompts the user for their guess code
		do
		{
			cout << "Enter your guess (digits separated with commas): ";
			cin >> input;
			err = guess.inputCode(input);

			// if 'inputCode' returns -1 the input was invalid and prompt repeats
			if (err == -1)
			{
				cout << "\nInvalid guess!\n\n";
				repeat = true;
			}
			// if 'inputCode' returns -2 the input had an incorrect number of 
			// digits and prompt repeats
			else if (err == -2)
			{
				cout << "\nGuess has incorrect number of digits!\n\n";
				repeat = true;
			}
			else
			{
				repeat = false;
			}

		} while (repeat);

		// check the input guess against the secret code
		int cc = secret.checkCorrect(guess);
		int ci = secret.checkIncorrect(guess);
		
		// output the two returned values to the user
		cout << "\nMastermind: " << cc << "," << ci << endl;

		// if the amount of correct digits in the correct position is equal to
		// the amount of digits entered at the beginning, the correct code was
		// entered and the user has won
		if (cc == n)
		{
			cout << "You cracked the code!" << endl;
			win = true;
		}
		// if the amount of tries reaches 10, the user runs out and loses
		else
		{
			trys++;

			if (trys == 10)
			{
				cout << "You failed to crack the code :(" << endl;
			}
		}
	}

	//output the secret code to the user at the end regardless of win or loss
	cout << "The secret code was: " << secret.formatCode() << endl;	//prints secret code
	return 0;
}