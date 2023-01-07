// Junxian Zhang - junxian@wustl.edu 

// Pinochle.cpp: This file contains defintions of methods, operators, and constructor of the PinochleDeck class. 

#include "PinochleDeck.h"

PinochleDeck::PinochleDeck()
{
	//Uses for-loops to add the cards that the Pinochle Deck needs.

	for (PinochleRank pRank = PinochleRank::nine; pRank < PinochleRank::undefined; ++pRank) {
		
		for (Suit s = Suit::clubs; s < Suit::undefined; ++s) {
			Card<PinochleRank, Suit > newCard = Card<PinochleRank, Suit>(pRank, s);
			cards.push_back(newCard);
			cards.push_back(newCard);

		}
		
	}
	
}

ostream& operator<<(ostream& out, PinochleRank pRank)
{

	//Uses a switch statement to print out the representation of the Pinochle rank

	switch (pRank) {
	case PinochleRank::nine:
		out << "9";
		break;
	case PinochleRank::jack:
		out << "J";
		break;
	case PinochleRank::queen:
		out << "Q";
		break;
	case PinochleRank::king:
		out << "K";
		break;
	case PinochleRank::ten:
		out << "10";
		break;
	case PinochleRank::ace:
		out << "A";
		break;
	case PinochleRank::undefined:
		out << "?";
		break;
	}
	return out;
}

PinochleRank& operator++(PinochleRank& pRank)
{
	//Uses a switch statement to increment the Pinochle rank

	switch (pRank) {
	case PinochleRank::nine:
		pRank = PinochleRank::jack;
		break;
	case PinochleRank::jack:
		pRank = PinochleRank::queen;
		break;
	case PinochleRank::queen:
		pRank = PinochleRank::king;
		break;
	case PinochleRank::king:
		pRank = PinochleRank::ten;
		break;
	case PinochleRank::ten:
		pRank = PinochleRank::ace;
		break;
	case PinochleRank::ace:
		pRank = PinochleRank::undefined;
		break;
	case PinochleRank::undefined:
		break;

	}
	return pRank;

}


