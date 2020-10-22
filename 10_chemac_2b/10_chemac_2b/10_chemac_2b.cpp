// Project 2 - Part B
// Anthony Cherubino, Eli MacColl
// 
// Main program file for Project 2 - Part B. Contains the code to play the 
// card game flip.
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <sstream>
using namespace std;

//Class Declarations

template <typename T>
class deck;

template <typename T>
class card
	// card class contains the value, suit of a card, and if it has been flipped as well as a pointer used
	// later to indicate the next card in the list.  Contains a default constructor 
	// a constructor that takes the value and suit as parameters.  Also contains
	// functions to set the suit, set the value, set the flip status, return the suit, return the
	// value, and return the flip status of a card.  Has an overloaded operator << to print out a card's value
	// and suit to the user.
{
	friend class deck<T>;
public:
	card();
	card(const card<T>&);
	void setValue(T);
	void setSuit(T);
	void setFlip(bool);
	T getValue();
	T getSuit();
	bool getFlip();
	friend ostream& operator<<(ostream& os, const card<T>& c)
		// overloads << operator to print out a single card's value and suit
	{
		return os << "Card: " << c.value << " of " << c.suit << "s\n" << endl;
	}
	card<T>& operator=(card<T>& c);
	card<T>* next;
private:
	T value, suit;

	// flip status of card starts as false
	bool flipped = false;
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
card<T>::card(const card<T>& c2)	
// card copy constructor that initializes the card's value and suit based on
// an existing card.
{
	value = c2.value;
	suit = c2.suit;
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

template <typename T>
void card<T>::setFlip(bool f)
// Function that initializes the flip of a card
{
	flipped = f;
}

template <typename T>
bool card<T>::getFlip()
// Function that returns the flip status of a card
{
	return flipped;
}

template <typename T>
card<T>& card<T>::operator=(card<T>& c) 
// overloads = operator to copy a card value and suit to a different card
{
	value = c.value;
	suit = c.suit;
	return *this;
}

//End of class card functions

template <typename T>
class deck
	// deck class contains card pointers used to navigate the deck of cards 
	// linked list that is created by the constructor in the following order, 
	// (ace-king, club-diamond-heart-spade). deck class contains an overloaded 
	// operator << that prints out the deck of cards in its current order. 
	// There are also functions that shuffle the deck of cards linked list, 
	// find a specific card based on its numeric index, add a new card to the end
	// of the linked list, and return and remove the top card from the deck.
{
public:
	deck();
	deck(card<T>*);
	~deck();
	friend ostream& operator<<(ostream& os, const deck<T>& d)
		// overloads << operator to print out deck of cards
	{
		// declares a card pointer and points it to the front of the deck
		card<T>* print;
		print = d.front;
		int numCard = 1;

		// iterates through the deck of cards linked list by printing the value
		// and suit of the card currently being pointed at by pointer print and
		// then setting pointer print to point at the next card in the list.
		// This continues through the end of the deck until the pointer reads NULL
		while (print != NULL)
		{
			os << numCard << ": " << print->getValue() << " of " << print->getSuit() << "s" << endl;
			print = print->next;
			numCard++;
		}
		return os;
	}
	void shuffle();
	card<T>* deal();
	card<T>* findCard(int n);
	void replace(card<T>*);
private:
	card<T>* front;
	card<T>* curr;
	card<T>* prev;
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
	vector<string> s = { "Club", "Diamond", "Heart", "Spade" };

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
				card<T>* ptr = new card<T>();
				ptr->setValue(value);
				ptr->setSuit(suit);
				front = ptr;
				curr = ptr;
			}

			// condition that runs after the first card in the deck has been created.
			// initializes a new card with the next value and next suit in the vectors
			// then points the previous card at the new card and moves the current 
			// pointer to point at the new card, creating the list 
			else {
				card<T>* ptr = new card<T>();
				ptr->setValue(value);
				ptr->setSuit(suit);
				curr->next = ptr;
				curr = ptr;

			}
		}
	}
}

template <typename T>
deck<T>::deck(card<T>* firstCard)
// constructor that creates a deck with a single card that is passed in 
{
	front = firstCard;
	curr = front;
	prev = front;
	firstCard->next = NULL;
}

template <typename T>
deck<T>::~deck()
// destructor deletes the linked list when the program finishes executing
{
	// runs if the list is not empty, sets the current pointer at the 
	// front of the linked list and declares a local pointer that is
	// used to iterate through the list, deleting the nodes
	if (front != NULL)
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

	// iterates through the deck to shuffle it 3 times
	for (int x = 0; x < 156; x++)
	{
		// generates a random number from 0 to 51 for each time a card is shuffled 
		// in the deck and resets the pointers to point at the front of the list each time
		ran = rand() % 52;
		s = front;
		curr = front;
		prev = front;

		// iterates the curr pointer through the linked list a number of times equal
		// to the randomly generated number
		for (int j = 0; j < ran + 1; j++)
		{
			curr = curr->next;
		}

		// iterates the prev pointer through the linked list a number of times equal
		// to one less than the randomly generated number so that it will point to 
		// the node immediately before the curr pointer node
		for (int r = 1; r < ran + 1; r++)
		{
			prev = prev->next;
		}
		card<T>* insert = new card<T>();
		insert->setValue(s->getValue());
		insert->setSuit(s->getSuit());
		insert->next = curr;
		prev->next = insert;
		front = front->next;
	}
}

template <typename T>
card<T>* deck<T>::deal()
// Function that returns the first card in the list and removes it from the list
{
	card<T>* top;
	top = front;
	front = front->next;
	return top;
}

template <typename T>
card<T>* deck<T>::findCard(int n)
// Function that finds and returns a card from the deck based on a given index
{
	curr = this->front;
	for (size_t i = 0; i < n - 1; i++)
	{
		curr = curr->next;
	}
	return curr;
}

template <typename T>
void deck<T>::replace(card<T>* c)
// Function that adds a passed in card to the end of the list
{
	curr->next = c;
	c->next = NULL;
	curr = curr->next;
}
template <typename T>
void playFlip(deck<T>* flipDeck)
// Function which begins play of the flip game
{
	string input;
	int n;
	bool repeat;
	bool exit = false;
	int points = 0;
	int flipped = 0;
	cout << "Points: " << points << endl;

	while (!exit) {
		
		// check if all 24 cards have already been flipped and if so end the game
		if (flipped == 24) {
			cout << "\nAll cards have been flipped!" << endl;
			break;
		}
		
		do
		{
			// prompt user for card to flip
			cout << "\nSelect a card (1 - 24) to flip or 0 to stop: ";

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

			// exit game if user enters 0
			if (n == 0)
			{
				exit = true;
				repeat = false;
			}
			
			// repeat prompt if value entered is less than 1 or greater than 24
			else if (n < 1 || n > 24)
			{
				cout << "Invalid card!" << endl;
				repeat = true;
			}
			else
			{
				repeat = false;
			}

		} while (repeat);

		// if exit is true form either user input or flipping all cards, quit the game
		if (exit) {
			break;
		}

		// get pointer to card that user has selected
		card<string>* inFlip;
		inFlip = flipDeck->findCard(n);
		
		// check if card has already been flipped
		if (!inFlip->getFlip())
		{
			inFlip->setFlip(true);

			string flipVal = inFlip->getValue();
			string flipSuit = inFlip->getSuit();

			// output value of flipped card
			cout << "Card " << n << ": " << flipVal << " of " << flipSuit << "s" << endl;

			// calculate change in user score
			if (flipVal == "Ace")
			{
				points += 10;
			}
			else if (flipVal == "King" || flipVal == "Queen" || flipVal == "Jack")
			{
				points += 5;
			}
			else if (flipVal == "7")
			{
				points /= 2;
			}
			else if (flipVal == "2" || flipVal == "3" || flipVal == "4" || flipVal == "5" || flipVal == "6")
			{
				points = 0;
			}

			if (flipSuit == "Heart")
			{
				points += 1;
			}
			cout << "Points: " << points << endl;

			flipped++;
		}
		else
		{
			cout << "Card already flipped!" << endl;
		}
	}

	// when game exits output final score
	cout << "\nYou got a score of: " << points << endl;

}

int main()
// Main function that initiates play of the game
{
	// create and shuffle deck and output the results to the user
	deck<string> shuffleDeck;
	cout << "Deck:\n" << shuffleDeck << endl;
	shuffleDeck.shuffle();
	cout << "Deck (Shuffled):\n" << shuffleDeck << endl;
	
	//draw 24 cards to be used for the game and output results to the user
	deck<string> drawDeck(shuffleDeck.deal());
	
	for (size_t i = 0; i < 23; i++)
	{
		drawDeck.replace(shuffleDeck.deal());
	}
	
	cout << "Not Drawn:\n" << shuffleDeck << endl;
	cout << "Drawn:\n" << drawDeck << endl;
	
	// begin play of the game
	playFlip(&drawDeck);
	
	return 0;
}