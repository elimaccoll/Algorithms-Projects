// Project 2 - Part A
// Anthony Cherubino, Eli MacColl
// 
// Main program file for Project 2 - Part A. Contains the code to generate 
//a deck of cards and shuffle it.
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
using namespace std;

//Class Declarations

template <typename T>
class deck;

template <typename T>
class card 
	// card class contains the value and suit of a card as well as a pointer used
	// later to indicate the next card in the list.  Contains a default constructor 
	// a constructor that takes the value and suit as parameters.  Also contains
	// functions to set the suit, set the value, return the suit, and return the
	// value of a card.  Has an overloaded operator << to print out a card's value
	// and suit to the user.
{
friend class deck<T>;
public:
	card();
	card(T, T);
	void setValue(T);
	void setSuit(T);
	T getValue();
	T getSuit();
	friend ostream& operator<<(ostream& os, const card<T>& c)	
	// overloads << operator to print out a single card's value and suit
	{
		return os << "Card: " << c.value << " of " << c.suit << "s\n" << endl;
	}
	card<T>* next;
private:		
	T value, suit;
};

// card class member functions

template <typename T>
card<T>::card()
// default constructor for card class that initializes the value and suit 
// of the card to a default of Ace of Clubs and pointer next to NULL
{
	value = "Ace";
	suit = "Club";
	next = NULL;
}

template <typename T>
card<T>::card(T v, T s)
// card constructor that takes value and suit as parameters and initializes
// the card's value and suit and sets next pointer to NULL.
{
	suit = s;
	value = v;
	next = NULL;
}

template <typename T>
void card<T>::setValue(T v)
// Function that initializes the value of a card
{
	value = v;
}

template <typename T>		
T card<T>::getValue()
// Function that returns the value of a card
{
	return value;
}

template <typename T>
void card<T>::setSuit(T s)
// Function that initializes the suit of a card
{
	suit = s;
}

template <typename T>
T card<T>::getSuit()
// Function that returns the suit of a card
{	
	return suit;
}

//End of class card functions

template <typename T>
class deck 
	// deck class contains card pointers used to navigate the deck of cards 
	// linked list that is created by the constructor in the following order, 
	// (ace-king, club-diamond-heart-spade). deck class contains an overloaded 
	// operator << that prints out the deck of cards in its current order. 
	// There is also a function that shuffles the deck of cards linked list.
{ 
public:
	deck();
	~deck();
	friend ostream& operator<<(ostream& os, const deck<T>& d)	
	// overloads << operator to print out deck of cards
	{				
		// declares a card pointer and points it to the front of the deck
		card<T>* print;
		print = d.front;

		// iterates through the deck of cards linked list by printing the value
		// and suit of the card currently being pointed at by pointer print and
		// then setting pointer print to point at the next card in the list.
		// This continues through the end of the deck until the pointer reads NULL
		while (print != NULL) 
		{
			os << print->getValue() << " of " << print->getSuit() << "s" << endl;	
			print = print->next;
		}
		return os;
	}
	void shuffle();
private:
	card<T> *front;
	card<T> *curr;
	card<T> *prev;
};

// deck class member functions

template <typename T>
deck<T>::deck() 
// constructor that creates a deck of cards in order 
// (ace-king, club-diamond-heart-spade)
{
	// initializes an empty list, declares strings suit and value, and initializes 
	// two vectors, one containing each value in a deck of cards (ace-king), the 
	// other containing each suit in a deck of cards club-diamond-heart-spade. 
	// These vectors are used to create the deck of cards linked list
	front = NULL;
	curr = front;
	string suit, value;
	vector<string> v = { "Ace", "2", "3", "4", "5", "6", "7", "8",
						 "9", "10", "Jack", "Queen", "King" };
	vector<string> s= { "Club", "Diamond", "Heart", "Spade" };

	// iterates through the vector of suits, setting string suit equal to the next 
	// suit in the vector (starts with clubs) in each iteration
	for (int i = 0; i < s.size(); i++) 
	{
		suit = s[i];

		// iterates through the vector of values, setting string value equal to the 
		// next value in the vector (starts with ace) in each iteration
		for (int j = 0; j < v.size(); j++) 
		{
			value = v[j];

			// condition that accounts for creating the first card in the deck. It
			// initializes a pointer to a new card with the first suit (Club) and 
			// first value (Ace) in the vectors. Then sets pointers front and curr
			// to point at the new card, which is the front of the list
			if (front == NULL) {
				card<T>* ptr = new card<T>(value, suit);
				front = ptr;
				curr = ptr;
			}

			// condition that runs after the first card in the deck has been created.
			// initializes a new card with the next value and next suit in the vectors
			// then points the previous card at the new card and moves the current 
			// pointer to point at the new card, creating the list 
			else {
				card<T>* ptr = new card<T>(value, suit);
				curr->next = ptr;
				curr = ptr;

			}
		}
	}
}

template <typename T>
deck<T>::~deck()
// destructor deletes the linked list when the program finishes executing
{
	// runs if the list is not empty, sets the current pointer at the 
	// front of the linked list and declares a local pointer that is
	// used to iterate through the list, deleting the nodes
	if (front!=NULL)
	{
		curr = front;
		card<T>* tempPtr;

		// the loop deletes nodes in the list by the method of deleting 
		// the node at the front until there are no nodes remaining
		while (curr != NULL)
		{
			tempPtr = curr;	
			curr = curr->next;
			delete tempPtr;
		}
	}
}

template <typename T>
void deck<T>::shuffle()
// Function that shuffles the deck of cards by generating a random number and
// then moving the curr pointer (curr starts at the front of the list ) through 
// the list that number of times. This will result in pointer curr pointing at a 
// random node in the list. There is also a pointer prev that points to the
// previous node in the list (the one before curr). A node (card) is then
// inserted in the position before the node that curr is pointing at with the value
// and suit of the card currently at the front of the list. The front of the list 
// then moves to the next card and this is repeated 364 times which means a 52 card 
// deck is cycled through 7 times
{
	string suit, value;
	card<T>* s;
	int ran;
	srand(time(NULL));

	// iterates through the deck 364 times to shuffle it 7 times
	for (int x = 0; x < 364; x++) 
	{
		// generates a random number from 0 to 51 for each time a card is shuffled 
		// in the deck and resets the pointers to point at the front of the list each time
		ran = rand() % 52;
		s = front;
		curr = front;
		prev = front;

		// iterates the curr pointer through the linked list a number of times equal
		// to the randomly generated number
		for (int j = 0; j < ran+1; j++) 
		{
			curr = curr->next;				
		}

		// iterates the prev pointer through the linked list a number of times equal
		// to one less than the randomly generated number so that it will point to 
		// the node immediately before the curr pointer node
		for (int r = 1; r < ran+1; r++) 
		{
			prev = prev->next;
		}
		card<T>* insert = new card<T>(s->getValue(), s->getSuit());
		insert->next = curr;
		prev->next = insert;
		front = front->next;
	}
}

int main()
// At this point, the main function is only used to initialize a deck of cards 
// and print it out before and after being shuffled
{
	// declares d deck object which generates a sorted deck of cards with
	// its constructor 
	deck<string> d;

	cout << "Deck before shuffle: " << endl;

	// utilizes overloaded operator << to print out the deck of cards
	cout << d << endl;

	// calls shuffle function to shuffle deck d
	d.shuffle();

	cout << "Deck after shuffle: " << endl;

	// uses overloaded operator << to print out the shuffled deck
	cout << d << endl;
}