// Junxian Zhang - junxian@wustl.edu

// Card_T.cpp: This file contains the increment operator definition and constructor of the Card struct template.

#include "Card_T.h"

template <typename R, typename S>
Card<R, S>::Card(R rank, S suit)
{
	// Initializes the member variables for Card
	Card::rank = rank;
	Card::suit = suit;
}

template <typename R, typename S>
ostream &operator<<(ostream &out, Card<R, S> card)
{
	// Inserts the card's rank and suit into the ostream and then returns a reference to the ostream
	out << card.rank << card.suit;
	return out;
}

template <typename R, typename S>
bool lessThanRank(const Card<R, S> &c1, const Card<R, S> &c2)
{
	// Checks if one card is less than the other based on their rank
	if ((c1.rank) < (c2.rank))
	{
		return true;
	}
	else if ((c1.rank) == (c2.rank))
	{
		if ((c1.rank) < (c2.rank))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

template <typename R, typename S>
bool lessThanSuit(const Card<R, S> &c1, const Card<R, S> &c2)
{
	// Checks if one card is less than the other based on their suit
	if (c1.suit < c2.suit)
	{
		return true;
	}
	else if (c1.suit == c2.suit)
	{
		if ((c1.rank) < (c2.rank))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

template <typename R, typename S>
bool equal(const Card<R, S> &c1, const Card<R, S> &c2)
{
	//Checks if one card is equal to another card
	return ((c1.suit == c2.suit) && ((c1.rank) == (c2.rank)));
}

template <typename R, typename S>
bool equalSuit(const Card<R, S> &c1, const Card<R, S> &c2)
{
	//Checks if the suit of one card is equal to the suit of another card
	return (c1.suit == c2.suit);
}

template <typename R, typename S>
bool equalRank(const Card<R, S> &c1, const Card<R, S> &c2)
{
	//Checks if the rank of one card is equal to the rank of another card
	return (c1.rank == c2.rank);
}