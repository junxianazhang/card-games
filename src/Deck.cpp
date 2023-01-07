// Junxian Zhang - junxian@wustl.edu 

// Deck.cpp: This file contains the definitions of the methods for the Deck class template.

#include "Deck.h"
#include <random>
#include <algorithm>

template<typename R, typename S> void Deck<R, S>::shuffle(){
    //Shuffles the cards in the set using the shuffle algorithm with a random_device that uses mt19937
    random_device rd;
    mt19937 rng (rd());
    std::shuffle(CardSet<R,S>::cards.begin(), CardSet<R,S>::cards.end(), rng);
}

template<class R, class S> void Deck<R, S>::collect(CardSet<R, S>& set){
   //Takes a card from a CardSet input and puts it in the cards vector of this Deck
   while (!set.is_empty()){
        set >> *this;
   }
}