// Card_T.h: This file contains the struct template of Card with its declaration of constructor and operator.

#pragma once
#include <iostream>
using namespace std;

template <typename R, typename S> struct Card {

	//Contains the constructor and member variables
	Card(R rank, S suit);
	R rank;
	S suit;
};
template <typename R, typename S> ostream& operator<< (ostream& out, Card<R, S> card);

template <typename R, typename S> bool lessThanRank(const Card<R, S>& c1, const Card<R, S>& c2);

template <typename R, typename S> bool lessThanSuit(const Card<R, S>& c1, const Card<R, S>& c2);

template <typename R, typename S> bool equal(const Card<R, S>& c1, const Card<R, S>& c2);

template <typename R, typename S> bool equalSuit(const Card<R, S>& c1, const Card<R, S>& c2);

template <typename R, typename S> bool equalRank(const Card<R, S>& c1, const Card<R, S>& c2);
#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE /* test whether guard symbol is defined */
#include "Card_T.cpp"
#endif