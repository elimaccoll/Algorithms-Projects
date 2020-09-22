#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
using namespace std;

class code							//program crashes when the guess contains the wrong number of digits
{
private:
	int digits, range;
	vector<int> numCode;

public:
	code(int, int);
	void generateCode();
	int inputCode(string);
	int checkCorrect(code);
	int checkIncorrect(code);
	string formatCode();
};

code::code(int n, int m) {
	digits = n;
	range = m;
}

void code::generateCode() {
	for (int i = 0; i < digits; i++) {
		int codeVal = rand() % range;
		numCode.push_back(codeVal);
	}
}

int code::inputCode(string input) {
	numCode.clear();
	stringstream inputSS(input);
	int num;
	while (inputSS.good())
	{
		string substr;
		getline(inputSS, substr, ',');
		try {
			numCode.push_back(stoi(substr));
		}
		catch (...) {
			return -1;
		}
	}
	if (numCode.size() != digits) {
		return -2;
	}
	return 0;
}

int code::checkCorrect(code guess) {
	int cc = 0;		//counter for check correct
	for (int i = 0; i < digits; i++) {
		if (guess.numCode[i] == numCode[i]) {
			cc++;
		}
	}
	return cc;
}

int code::checkIncorrect(code guess) {
	vector<int> numCodeCopy = numCode;
	int ci = 0;	//counter for check incorrect
	for (int i = 0; i < digits; i++) {	//eliminates correct values in correct indicies
		if (guess.numCode[i] == numCode[i]) {
			guess.numCode[i] = 'e';
			numCode[i] = 'e';
		}
	}
	for (int i = 0; i < digits; i++) {
		for (int j = 0; j < digits; j++) {
			if (guess.numCode[i] == numCode[j] && guess.numCode[i] != 'e') {
				ci++;
				guess.numCode[i] = 'e';
				numCode[j] = 'e';
			}
		}
	}
	numCode = numCodeCopy;
	return ci;
}

string code::formatCode() {
	string output = "(";
	for (int i = 0; i < digits; i++) {
		output += to_string(numCode[i]);
		if (i != digits - 1)
		{
			output += ",";
		}
	}
	output += ")";
	return output;
}

int main()
{
	bool win = false;
	int trys = 0;
	int n, m;
	string input;
	bool repeat;
	int err;

	do {
		cout << "Enter the amount of digits in the code: ";
		cin >> n;
		if (n <= 0) {
			cout << "\nInvalid number of digits!\n\n";
			repeat = true;
		}
		else {
			repeat = false;
		}
	} while (repeat);

	do {
		cout << "Enter the exclusive upper bound of digits in the code: ";
		cin >> m;
		if (m <= 0) {
			cout << "\nInvalid upper bound!\n\n";
			repeat = true;
		}
		else {
			repeat = false;
		}
	} while (repeat);


	code secret(n, m);
	secret.generateCode();
	code guess(n, m);
	cout << "Code generated with (" << n << ") digits and a range of (0-" << m - 1 << ")" << endl;
	
	cout << "The \"secret\" code is: " << secret.formatCode() << endl;
	
	while (trys < 10 && win == false) {
		cout << "\nYou have " << 10 - trys << " guesses remaining" << endl;
		do {
			cout << "Enter your guess (digits separated with commas): ";
			cin >> input;
			err = guess.inputCode(input);
			if (err == -1) {
				cout << "\nInvalid guess!\n\n";
				repeat = true;
			}
			else if (err == -2) {
				cout << "\nGuess has incorrect number of digits!\n\n";
				repeat = true;
			}
			else {
				repeat = false;
			}
		} while (repeat);

		int cc = secret.checkCorrect(guess);
		int ci = secret.checkIncorrect(guess);
		cout << "\nMastermind: " << cc << "," << ci << endl;
		
		if (cc == n) {
			cout << "You cracked the code!" << endl;
			win = true;
		}
		else {
			trys++;
			if (trys == 10) {
				cout << "You failed to crack the code :(" << endl;
			}
		}

	}



	cout << "The secret code was: " << secret.formatCode() << endl;	//prints secret code

	return 0;
}