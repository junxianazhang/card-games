// HoldEmDeck.h: This file contains defintions of methods, operators, and constructor of the HoldEmDeck class. 

#include "HoldEmDeck.h"

HoldEmDeck::HoldEmDeck()
{
	//Uses for-loops to add the cards that the HoldEm Deck needs.

	for (HoldEmRank hRank = HoldEmRank::two; hRank < HoldEmRank::undefined; ++hRank) {
		for (Suit s = Suit::clubs; s < Suit::undefined; ++s) {
			Card<HoldEmRank, Suit> newCard = Card<HoldEmRank, Suit>(hRank, s);
			cards.push_back(newCard);

		}

	}
	
}


ostream& operator<<(ostream& out, HoldEmRank hRank)
{
	//Uses a switch statement to print out the representation of the HoldEm rank

	switch (hRank) {
	case HoldEmRank::two:
		out << "2";
		break;
	case HoldEmRank::three:
		out << "3";
		break;
	case HoldEmRank::four:
		out << "4";
		break;
	case HoldEmRank::five:
		out << "5";
		break;
	case HoldEmRank::six:
		out << "6";
		break;
	case HoldEmRank::seven:
		out << "7";
		break;
	case HoldEmRank::eight:
		out << "8";
		break;
	case HoldEmRank::nine:
		out << "9";
		break;
	case HoldEmRank::ten:
		out << "10";
		break;
	case HoldEmRank::jack:
		out << "J";
		break;
	case HoldEmRank::queen:
		out << "Q";
		break;
	case HoldEmRank::king:
		out << "K";
		break;
	case HoldEmRank::ace:
		out << "A";
		break;
	case HoldEmRank::undefined:
		out << "?";
		break;
	}
	return out;
}

HoldEmRank& operator++(HoldEmRank& hRank)
{
	//Uses a switch statement to increment the HoldEm rank

	switch (hRank) {
	case HoldEmRank::two:
		hRank = HoldEmRank::three;
		break;
	case HoldEmRank::three:
		hRank = HoldEmRank::four;
		break;
	case HoldEmRank::four:
		hRank = HoldEmRank::five;
		break;
	case HoldEmRank::five:
		hRank = HoldEmRank::six;
		break;
	case HoldEmRank::six:
		hRank = HoldEmRank::seven;
		break;
	case HoldEmRank::seven:
		hRank = HoldEmRank::eight;
		break;
	case HoldEmRank::eight:
		hRank = HoldEmRank::nine;
		break;
	case HoldEmRank::nine:
		hRank = HoldEmRank::ten;
		break;
	case HoldEmRank::ten:
		hRank = HoldEmRank::jack;
		break;
	case HoldEmRank::jack:
		hRank = HoldEmRank::queen;
		break;
	case HoldEmRank::queen:
		hRank = HoldEmRank::king;
		break;
	case HoldEmRank::king:
		hRank = HoldEmRank::ace;
		break;
	case HoldEmRank::ace:
		hRank = HoldEmRank::undefined;
		break;
	case HoldEmRank::undefined:
		break;

	}
	return hRank;
}
