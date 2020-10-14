// Project 2 - Part A
// Anthony Cherubino, Eli MacColl
// 
// Main program file for Project 2 - Part A. Contains the code to generate a deck
// of cards and shuffle it.
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
using namespace std;

template <typename T>
class deck;

template <typename T>
class card {
friend class deck<T>;
public:
	card();
	card(T, T);
	void setValue(T);
	void setSuit(T);
	T getValue();
	T getSuit();
	friend ostream& operator<<(ostream& os, const card<T>& c)	//overloads << operator to print out a single card's value and suit
	{
		return os << "Card: " << c.value << " of " << c.suit << "s\n" << endl;
	}
	card* next;
private:		
	T value, suit;	//node data for a single card
};

template <typename T>
card<T>::card()		//default constructor
{
	value = "Ace";
	suit = "Club";
}

template <typename T>
card<T>::card(T v, T s)		//constructor that sets value and suit
{
	suit = s;
	value = v;
	next = NULL;
}

template <typename T>
void card<T>::setValue(T v)		//sets card value
{
	value = v;
}

template <typename T>		
T card<T>::getValue() //returns card value
{
	return value;
}

template <typename T>
void card<T>::setSuit(T s) //sets card suit
{
	suit = s;
}

template <typename T>
T card<T>::getSuit()	//returns card suit
{	
	return suit;
}
//End class card functions

template <typename T>
class deck { 
public:
	deck();
	~deck();
	friend ostream& operator<<(ostream& os, const deck<T>& d)	//overloads << operator to print out deck of cards
	{															
		card<T>* print;
		print = d.front;
		while (print != NULL) {			//iterates through deck while the pointer is pointing to a node with value and suit data
			os << print->getValue() << " of " << print->getSuit() << "s" << endl;	//prints value and suit of card currently being pointed at
			print = print->next;	//moves print pointer to point at the next card in the list
		}
		return os;
	}
	void shuffle();
private:
	card<T> *front;
	card<T> *curr;
	card<T> *prev;
};

template <typename T>
deck<T>::deck() //constructor that creates a deck of cards in order
{
	front = NULL;
	curr = front;
	string suit, value;
	vector<string> v = { "Ace", "2", "3", "4", "5", "6", "7", "8",
						 "9", "10", "Jack", "Queen", "King" };	//values used to create deck
	vector<string> s= { "Club", "Diamond", "Heart", "Spade" };	//suits used to create deck

	for (int i = 0; i < s.size(); i++) {		//iterates through the vector of suits
		suit = s[i];
		for (int j = 0; j < v.size(); j++) {	//iterates through the vector of values
			value = v[j];
			if (front == NULL) {				//condition that accounts for setting the first card in the deck
				card<T>* ptr = new card<T>(value, suit);//creates a pointer to a new card with the first suit (Club) and first value (Ace)
				front = ptr;					//makes the new card the front of the list
				curr = ptr;						//makes the new card the current node in the list
			}
			else {								//condition that runs after the first card in the deck has been created
				card<T>* ptr = new card<T>(value, suit);//creates a pointer to a new card with the next value and next suit in the vectors
				curr->next = ptr;				//points the previous card this new card
				curr = ptr;						//makes the new card the current card in the list

			}
		}
	}
}

template <typename T>
deck<T>::~deck()				//destructor
{
	if (front!=NULL)			//runs if the list is not empty
	{
		curr = front;			//sets the first node as the current node
		card<T>* tempPtr;		//creates a temporary pointer

		while (curr != NULL)	//deletes remaining nodes
		{
			tempPtr = curr;		//sets the temp pointer to the current node in the list
			curr = curr->next;	//sets the current pointer to the next node in the list
			delete tempPtr;		//deletes the previous node
		}
	}
}


template <typename T>
void deck<T>::shuffle()
{
	string suit, value;
	card<T>* s;
	int ran;
	srand(time(NULL));

	for (int x = 0; x < 364; x++) {	// 364/52=7 so this shuffles through the deck 7 times
		ran = rand() % 52;
		s = front;
		curr = front;
		prev = front;
		for (int j = 0; j < ran+1; j++) {
			curr = curr->next;				
		}
		for (int r = 1; r < ran+1; r++) {
			prev = prev->next;
		}
		card<T>* insert = new card<T>(s->getValue(), s->getSuit());
		insert->next = curr;
		prev->next = insert;
		front = front->next;
	}
}

int main()
{
	//card<string> c1;
	//cout << c1;

	deck<string> d;
	cout << "Deck before shuffle: " << endl;
	cout << d << endl;
	d.shuffle();
	cout << "Deck after shuffle: " << endl;
	cout << d << endl;

}