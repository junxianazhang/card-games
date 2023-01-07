// HoldEmDeck.h: This file contains the class of HoldEmDeck with its declaration of methods, operators, constructor, and member variables.

#pragma once

#include "Deck.h"
#include "Suit.h"


enum class HoldEmRank { two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, undefined};

class HoldEmDeck : public Deck<HoldEmRank, Suit> {
public:
	HoldEmDeck();
private:
	
};

ostream& operator<< (ostream& out, HoldEmRank hRank);

HoldEmRank& operator++ (HoldEmRank& hRank);