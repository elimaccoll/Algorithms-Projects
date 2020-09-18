#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

class code							//program crashes when the guess contains the wrong number of digits
{
public:
	int n, m;
	string sCode, guess;
	bool win=false;

	code(int, int);
	string generateCode(int, int);
	int checkCorrect(string);
	int checkIncorrect(string);
};
    code::code(int digits, int range) {
		n = digits;
		m = range;
		int trys = 0;
		generateCode(n, m);
		while (trys!=10 && win == false){
			cout << "This is try #" << trys + 1<< endl;
			cout << "Enter your guess: " << endl;
			cin >> guess;
			int cc = checkCorrect(guess);
			int ci = checkIncorrect(guess);
			cout << cc << "," << ci << endl;
			trys++;
			if (trys == 10) {
				cout << "You failed the crack the code :(" << endl;
			}
		}
	}

	string code::generateCode(int digits, int range) {
		for (int i = 0; i < digits; i++) {
			int codeVal = rand() % range;
			sCode += to_string(codeVal);
			/*if (i != digits - 1) {					maybe put back in later but for now its messing up formatting 
				sCode += to_string(codeVal) + ",";		makes code format: (1,2,3,4,5) but commas mess up for loop when comparing elements
			}
			else {
				sCode += to_string(codeVal);
			}*/
		}
		cout << "Secret Code: "<< sCode << endl;	//prints secret code
		return sCode;
	}

	int code::checkCorrect(string g) {
		int cc=0;		//counter for check correct
		if (g.length() !=n) {
			cout << "Guess has incorrect number of digits" << endl;
		}
		else {
			for (int i = 0; i<n; i++) {
				if (g[i] == sCode[i]) {
					cc++;
				}
			}
		}
		//cout << "Check Correct: " << cc << endl; //prints check correct return
		if (cc == n) {
			cout << "You Cracked the Code!" << endl;
			win = true;
		}
		return cc;
	}

	int code::checkIncorrect(string g) {
		string sCodeCopy=sCode;
		int ci = 0;	//counter for check incorrect
		for (int i = 0; i < n; i++) {	//eliminates correct values in correct indicies
			if (g[i] == sCode[i]) {
				g[i] = 'e';
				sCode[i] = 'e';
			}
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (g[i] == sCode[j] && g[i]!='e') {		
					ci++;
					g[i] = 'e';
					sCode[j] = 'e';
				}
			}
		}
		sCode = sCodeCopy;
		//cout << "Check Incorrect: " << ci << endl;	//prints check incorrect return
		return ci;
	}

	int main()
	{
		int digits, range;
		string guess;
		cout << "Enter your number of digits: ";
		cin >> digits;
		cout << "Enter the upper bound of your range: ";
		cin >> range;

		code play(digits, range);
		return 0;
	}