#include <iostream>
#include <string>
#include <vector>
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
	friend ostream& operator<<(ostream& os, const card<T>& c)
	{
		return os << "Card: " << c.value << " " << c.suit<<endl;
	}
	card* next;
private:
	T value, suit;
};

template <typename T>
card<T>::card() {		//default constructor
	value = "2";
	suit = "Clubs";
}

template <typename T>
card<T>::card(T v, T s) {	//constructor that sets value and suit
	value = v;
	suit = s;
	next = NULL;
}

template <typename T>
void card<T>::setValue(T v) {	//sets card value
	value = v;
}

template <typename T>
T card<T>::getValue() {		//returns card value
	return value;
}

template <typename T>
void card<T>::setSuit(T s) {	//sets card suit
	suit = s;
}

template <typename T>
T card<T>::getSuit() {	//returns card suit
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
		while (print != NULL) {
			os << print->getValue() << " " << print->getSuit()<<endl;
			print = print->next;
		}
		return os;
	}
	//void shuffle();
private:
	card<T> *front;
	card<T> *curr;
};

template <typename T>
deck<T>::deck() {
	front = NULL;
	curr = front;
	string suit, value;
	vector<string> v = { "Ace", "King", "Queen", "Jack", "10", "9", "7", "6", "5", "4", "3", "2" };
	vector<string> s= { "Club", "Diamond", "Heart", "Spade" };

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
deck<T>::~deck(){
	if (front!=NULL) // List is not empty
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

int main()
{
	card<string> c1;
	cout << c1;

	deck<string> d;
	cout << d;
}