#include <iostream>
#include <string>
using namespace std;

class card {
public:						
	string value, suit;	

	card();
	card(string, string);
	void setValue(string);
	void setSuit(string);
	string getValue();
	string getSuit();
	void operator<<(card);
};
card::card() {		//default constructor
	value = "2";
	suit = "club";
}
card::card(string v, string s) {		//constructor that sets value and suit
	value = v;
	suit = s;
}
void card::setValue(string v) {	//sets card value
	value = v;
}
string card::getValue() {		//returns card value
	return value;
}
void card::setSuit(string s) {	//sets card suit
	suit = s;
}
string card::getSuit() {	//returns card suit
	return suit;
}
void card::operator<<(card c) {	//overloaded << operator to print out card value and suit
	value = c.getValue();
	suit = c.getSuit();
	cout << value << " " << suit << endl;
}	
//End class card functions

struct node {
	string value;
	string suit;
};

class deck { 
public:
	deck* front;
	deck* curr;
	deck* next;

	deck();	//constructor
	void operator<<(deck);
	void shuffle(deck);
};
deck::deck() {
	while (curr != NULL) {
		node<card>* newNode;

	}
}


int main()
{
	card test1("3", "spade");
	test1 << test1;

	card test2;
	test2 << test2;
	test2.setValue("7");
	test2.setSuit("heart");
	test2 << test2;
}