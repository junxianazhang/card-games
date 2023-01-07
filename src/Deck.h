// Deck.h: This file contains the template class of Deck with its declaration of methods.

#pragma once

#include "CardSet.h"
#include <random>
#include <algorithm>

template <typename R, typename S> class Deck : public CardSet<R, S> {
public:
	void shuffle();
	void collect(CardSet<R, S>& set);
};



#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE /* test whether guard symbol is defined */
#include "Deck.cpp"
#endif