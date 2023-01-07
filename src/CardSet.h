// CardSet.h: This file contains the template class of CardSet with its declaration of methods, operators, and member variables.

#pragma once

#include <iostream>
#include <vector>
#include "Card_T.h"

using namespace std;

template <typename R, typename S>
class CardSet
{
protected:
	// Contains all the cards of the set
	vector< Card<R, S> > cards;

public:
	CardSet() = default;
	void print(ostream &out, size_t linebreak);
	CardSet<R, S> &operator>>(CardSet<R, S> &set);
	CardSet(const CardSet<R, S> &target);
	bool is_empty();
	static vector< Card<R, S> > CardSet<R,S>::* getCards();
	void removeCard(Card<R, S> target);
	void addCard(Card<R, S> target);
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE /* test whether guard symbol is defined */
#include "CardSet.cpp"
#endif