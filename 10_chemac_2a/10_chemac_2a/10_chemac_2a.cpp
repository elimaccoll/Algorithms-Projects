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
	card(T, T);
	void setValue(T);
	void setSuit(T);
	T getValue();
	T getSuit();
	friend ostream& operator<<(ostream& os, const card<T>& c)
	{
		return os << "Card: " << c.value << " of " << c.suit << "s\n" << endl;
	}
	card* next;
private:
	T value, suit;
};

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
	friend ostream& operator<<(ostream& os, const deck<T>& d)
	{
		card<T>* print;
		print = d.front;
		os << "Deck:\n";
		while (print != NULL) {
			os << print->getValue() << " of " << print->getSuit() << "s" << endl;
			print = print->next;
		}
		return os;
	}
	void shuffle();
private:
	card<T>* front;
	card<T>* curr;
};

template <typename T>
deck<T>::deck()
{
	front = NULL;
	curr = front;
	string suit, value;
	vector<string> v = { "Ace", "2", "3", "4", "5", "6", "7", "8",
						 "9", "10", "Jack", "Queen", "King" };
	vector<string> s = { "Club", "Diamond", "Heart", "Spade" };

	for (int i = 0; i < s.size(); i++) {
		suit = s[i];
		for (int j = 0; j < v.size(); j++) {
			value = v[j];
			if (front == NULL) {
				card<T>* ptr = new card<T>();
				front = ptr;
				curr = ptr;
				curr->setSuit(suit);
				curr->setValue(value);
			}
			else {
				card<T>* ptr = new card<T>();
				curr->next = ptr;
				curr = ptr;
				curr->setSuit(suit);
				curr->setValue(value);
			}
		}
	}
}

template <typename T>
deck<T>::~deck()
{
	if (front != NULL) // List is not empty
	{
		curr = front;
		card<T>* tempPtr;

		while (curr != NULL) // delete remaining nodes
		{
			tempPtr = curr;
			curr = curr->next;
			delete tempPtr;
		}
	}
}

template <typename T>
void deck<T>::shuffle()
// 
{
	string origSuit, origValue, destSuit, destValue;
	int ran;
	srand(time(NULL));
	cout << "Shuffling...\n" << endl;
	for (int i = 0; i < 52; i++) {	//shuffles each card in the deck
		curr = this->front;
		for (int x = 0; x < i - 1; x++) {
			curr = curr->next;
		}
		if (i > 0) {
			curr->setSuit(destSuit);
			curr->setValue(destValue);
			curr = curr->next;
		}
		origSuit = curr->getSuit();
		origValue = curr->getValue();
		ran = rand() % 52;
		curr = this->front;
		for (int j = 0; j < ran; j++) {	//move curr rand # of times
			curr = curr->next;
		}
		destSuit = curr->getSuit();
		destValue = curr->getValue();
		curr->setSuit(origSuit);
		curr->setValue(origValue);
	}
}

int main()
{
	deck<string> d;
	cout << d << endl;
	d.shuffle();
	cout << d << endl;
}