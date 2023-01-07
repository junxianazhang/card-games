// Suit.cpp: This file contains the operators' definition of the Suit enum class.

#include "Suit.h"

ostream& operator<<(ostream& out, const Suit& suit)
{
	//Uses a switch statement to print out the representation of the suits

	switch (suit) {
	case Suit::clubs:
		out << "C";
		break;
	case Suit::diamonds:
		out << "D";
		break;
	case Suit::hearts:
		out << "H";
		break;
	case Suit::spades:
		out << "S";
		break;
	case Suit::undefined:
		out << "?";
		break;
	}
	return out;
}

Suit& operator++(Suit& suit)
{
	//Uses a switch statement to increment the suit

	switch (suit) {
	case Suit::clubs:
		suit = Suit::diamonds;
		break;
	case Suit::diamonds:
		suit = Suit::hearts;
		break;
	case Suit::hearts:
		suit = Suit::spades;
		break;
	case Suit::spades:
		suit = Suit::undefined;
		break;
	case Suit::undefined:
		break;

	}
	return suit;
}
