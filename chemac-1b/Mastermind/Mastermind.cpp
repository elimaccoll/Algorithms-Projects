// Project 1 - Part B
// Anthony Cherubino, Eli MacColl
// 
// Main program file for Project 1 - Part B. Contains the code to play a game
// of Mastermind against the computer.

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <time.h>
using namespace std;

// Class Decelerations

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
	code();
	void setParams(int, int);
	int getDigits();
	int getRange();
	void generateCode();
	int inputCode(string);
	int checkCorrect(code);
	int checkIncorrect(code);
	string formatCode();
};

class response
	// Response class containing the amount of digits that are correct and in the
	// correct position as well as the amount of digits that are correct and in the
	// incorrect position based on a user input code guess being compared against the
	// randomly generated secret code. Contains a constructor which is initializes these
	// with a default value of 0 and functions to set and get these values individually.
{
private:
	int cc, ci;

public:
	response();
	void setCC(int);
	void setCI(int);
	int getCC();
	int getCI();
};

class mastermind
	// Mastermind class containing the secret code object and correct response object.
	// Contains a constructor which initializes the secret code based on input digit and
	// range values as well as a default constructor that initializes to 5 and 10, respectively.
	// Contains functions to print the secret code, prompt the user to enter a guess code,
	// get a response based on a user input code, check if the code has been correctly guessed,
	// and begin play of the game.
{
private:
	code secret;
	response winResp;

public:
	mastermind(int n, int m);
	mastermind();
	void printCode();
	code humanGuess();
	response getResponse(code guess);
	bool isSolved(response resp);
	void playGame();

};

// Global Overloads

bool operator==(response r1, response r2)
// Overload of the == operator which compares the equivalence of the two values
// contained in a response object.
{
	return r1.getCC() == r2.getCC() && r1.getCI() == r2.getCI();
}

ostream& operator<<(ostream& output, response resp)
// Overload of the == operator which outputs the two values contained in a
// response object.
{
	output << resp.getCC() << "," << resp.getCI() << endl;
	return output;
}

// Code Class Member Functions

code::code()
// Code constructor that initializes the digits and range of the code to a
// default value of 0.
{
	digits = 0;
	range = 0;
}

void code::setParams(int n, int m)
// Code constructor that initializes the digits and range of the code based
// on the input values.
{
	digits = n;
	range = m;
}

int code::getDigits()
// Function that returns the digits value of the code.
{
	return digits;
}

int code::getRange()
// Function that returns the range value of the code.
{
	return range;
}

void code::generateCode()
// Function that generates and stores a random code in both the 'numCode' and
// 'numCodeCopy' vectors with the number of digits and within the range determined
// by the values input by the user and initialized in the constructor. Used to
// generate the secret code which the user is trying to guess.
{
	// clears any values stored in the vector
	numCode.clear();

	// generates random numbers up to 'digits' in a range between 0 and 'range' - 1
	// and numbers are then added to the back of the 'numCode' vector
	srand(time(NULL));
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
// Function that takes in a user input string representing a guessed code with
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

	// check if any of the digits in the input code are outside of the range of
	// the secret code
	// if not correct, return -3
	for (int i = 0; i < numCode.size(); i++)
	{
		if (numCode[i] >= range || numCode[i] < 0) {
			return -3;
		}
	}

	// contents of the now filled 'numCode' copied to 'numCodeCopy' as a backup
	// so original code is saved when the contents of 'numCode' are modified in
	// 'checkCorrect' function
	numCodeCopy = numCode;

	// return 0 to indicate success
	return 0;
}

int code::checkCorrect(code guess)
// Function that checks the secret code against the user input guessed code and
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
// Function that checks the secret code against the user input guessed code and
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
// Function that formats the code as a string so it can be easily printed to 
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

// Response Member Functions

response::response()
// Response constructor that initializes the amount of digits that are correct
// and in the correct position (cc) as well as the amount of digits that are
// correct and in the incorrect position (ci) to be 0.
{
	setCC(0);
	setCI(0);
}

void response::setCC(int cc_in)
// Function that sets the value of the correct digits in the correct position
// based on an input value.
{
	cc = cc_in;
}

void response::setCI(int ci_in)
// Function that sets the value of the correct digits in the incorrect position
// based on an input value.
{
	ci = ci_in;
}

int response::getCC()
// Function that returns the value of the correct digits in the correct position.
{
	return cc;
}

int response::getCI()
// Function that returns the value of the correct digits in the incorrect position.
{
	return ci;
}

// Mastermind Member Functions

mastermind::mastermind(int n, int m)
// Constructor for mastermind object that initializes the secret code using
// input digit and range values.
{
	secret.setParams(n, m);
}

mastermind::mastermind()
// Constructor for mastermind object that initializes the secret code using
// default values of 5 (digits) and 10 (range).
{
	secret.setParams(5, 10);
}

void mastermind::printCode()
// Function that prints out the secret code
{
	cout << secret.formatCode();
}

code mastermind::humanGuess()
// Function that prompts the user to enter a guess code 
{
	// stores the user input code guesses
	string input;

	// controls while loop repeating when the user inputs something invalid
	bool repeat;

	// stores the returned error code from 'inputCode'
	int err;

	// create the guess code object and initialize it with the same digit and
	// range values as the secret code
	code guess;
	guess.setParams(secret.getDigits(), secret.getRange());

	// prompts the user for their guess code
	do
	{
		cout << "Enter your guess (digits separated with commas): ";
		getline(cin, input);
		err = guess.inputCode(input);

		switch (err) {

			// if 'inputCode' returns -1 the input was invalid and prompt repeats
		case -1:
			cout << "\nInvalid guess!\n\n";
			repeat = true;
			break;

			// if 'inputCode' returns -2 the input had an incorrect number of 
			// digits and prompt repeats
		case -2:
			cout << "\nGuess has incorrect number of digits!\n\n";
			repeat = true;
			break;

			// if 'inputCode' returns -3 the input has digits outside the input
			// range and prompt repeats
		case -3:
			cout << "\nGuess has digits out of range!\n\n";
			repeat = true;
			break;

		default:
			repeat = false;
			break;
		}

	} while (repeat);

	return guess;
}

response mastermind::getResponse(code guess)
// Function that compares a guessed code against the secret code and returns
// the amount of correct digits in the correct place and the amount of correct
// digits in the incorrect place.
{
	response guessResp;

	// check the input guess against the secret code
	guessResp.setCC(secret.checkCorrect(guess));
	guessResp.setCI(secret.checkIncorrect(guess));

	return guessResp;
}

bool mastermind::isSolved(response resp)
// Function that checks the received response for equivalence with the response
// that would be returned if the code was guessed correctly.
{
	return resp == winResp;
}

void mastermind::playGame()
// Function that begins play of the game versus the computer.
{
	// indicates whether the user has guessed the code successfully
	bool win = false;

	// keeps track of the number of attempts to guess the code
	int trys = 0;

	// sets the value of the 'cc' of the correct response to be equal to the
	// number of digits in the secret code as this would be the response received
	// when the code has been guessed successfully
	winResp.setCC(secret.getDigits());

	// generates the secret code based on the input digit and range parameters
	secret.generateCode();
	cout << "Code generated with (" << secret.getDigits() << ") digits and ";
	cout << "a range of (0-" << secret.getRange() - 1 << ")" << endl;

	// (for checking correct functionality)
	// have the following lines uncommented to have the secret code printed out at the start
	cout << "The \"secret\" code is: ";
	printCode();
	cout << endl;

	// prompts the user to enter their guess code and loops until either the correct
	// code is entered or user runs out of guesses
	while (trys < 10 && win == false)
	{
		// outputs the remaining number of guesses counting down from 10
		cout << "\nYou have " << 10 - trys << " guesses remaining" << endl;

		// get user input guess code and check against secret code for response
		response guessResp = getResponse(humanGuess());

		// output response to the user
		cout << "\nMastermind: " << guessResp;

		// if the amount of correct digits in the correct position is equal to
		// the amount of digits entered at the beginning, the correct code was
		// entered and the user has won
		if (isSolved(guessResp))
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

	// output the secret code to the user at the end regardless of win or loss
	cout << "The secret code was: ";
	printCode();
	cout << endl;
}

// Main Function

int main()
// Main function where the user is prompted to enter the desired number of digits
// and range of values to generate the secret code with and then begins play of
// the game.
{
	// stores the input values of digits and range, respectively
	int n, m;

	// stores the user input code guesses
	string input;

	// controls while loop repeating when the user inputs something invalid
	bool repeat;

	// prompt user to enter desired amount of digits
	do
	{
		cout << "Enter the amount of digits in the code: ";

		getline(cin, input);

		// if there is an invalid input this will catch the error and return -1
		try
		{
			n = stoi(input);
		}
		catch (...)
		{
			// if input is invalid, set n to -1 so prompt will repeat
			n = -1;
		}

		// repeat prompt if value entered is less than or equal to 0
		if (n <= 0)
		{
			cout << "\nInvalid amount of digits!\n\n";
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

		getline(cin, input);

		// if there is an invalid input this will catch the error and return -1
		try
		{
			m = stoi(input);
		}
		catch (...)
		{
			// if input is invalid, set n to -1 so prompt will repeat
			m = -1;
		}

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

	// initialize game mastermind object with input digit and range and begin play
	mastermind game(n, m);
	game.playGame();

	return 0;
}