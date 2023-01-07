// CardSet.cpp: This file contains the definitions of the methods and operators for the CardSet class template.

#include "CardSet.h"

template <typename R, typename S>
void CardSet<R, S>::print(ostream &out, size_t linebreak)
{
	// Prints out the cards using iterators with specified linebreaks using a count int variable
	size_t count = 1;
	for (typename vector<Card<R, S>>::iterator it = cards.begin(); it != cards.end(); ++it)
	{
		out << *it << " ";
		if (count % linebreak == 0)
		{
			cout << "\n";
		}
		++count;
	}
}

template <typename R, typename S>
CardSet<R, S> &CardSet<R, S>::operator>>(CardSet<R, S> &set)
{
	// Moves a card from one set to another and checks if the set that's being taken from is empty or not
	if (this->is_empty())
	{
		throw runtime_error("CardSet is empty.");
	}
	else
	{

		Card<R, S> newCard = Card<R, S>((this->cards[this->cards.size() - 1]).rank, (this->cards[this->cards.size() - 1]).suit);
		set.cards.push_back(newCard);
		this->cards.pop_back();
		return *this;
	}
}

template <typename R, typename S>
bool CardSet<R, S>::is_empty()
{
	// Checks to see if there are any cards in the set.
	return cards.empty();
}

template <typename R, typename S>
CardSet<R, S>::CardSet(const CardSet<R, S> &target)
{
	// Deep copies a CardSet by creating new cards using the rank and suit of the cards in the CardSet being copied
	for (auto it = target.cards.begin(); it != target.cards.end(); ++it)
	{
		Card newCard = Card((*it).rank, (*it).suit);
		this->cards.push_back(newCard);
	}
}

template <typename R, typename S>
vector<Card<R, S>> CardSet<R, S>::*CardSet<R, S>::getCards()
{
	// Gets the pointer to the cards member variable to be used for access
	return &CardSet<R, S>::cards;
}

template <typename R, typename S>
void CardSet<R, S>::removeCard(Card<R, S> target)
{
	for (typename vector<Card<R, S>>::iterator it = cards.begin(); it != cards.end(); ++it)
	{
		if ((*it).rank == target.rank && (*it).suit == target.suit)
		{
			cards.erase(it);
			return;
		}
	}
}

template <typename R, typename S>
void CardSet<R, S>::addCard(Card<R, S> target)
{
	cards.push_back(target);
}