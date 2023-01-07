// Junxian Zhang - junxian@wustl.edu 

// Pinochle.h: This file contains the class of PinochleDeck with its declaration of methods, operators, constructor, and member variables.

#pragma once

#include "Deck.h"
#include "Suit.h"

enum class PinochleRank {nine, jack, queen, king, ten, ace, undefined};

class PinochleDeck : public Deck<PinochleRank, Suit> {
public:
	PinochleDeck();
	
private:
	
};

ostream& operator<< (ostream& out, PinochleRank pRank);

PinochleRank& operator++ (PinochleRank& pRank);

