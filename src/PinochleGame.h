// PinochleGame.h: This file contains the definitions of the methods, constructor, and member variable for the PinochleGame class.

#pragma once

#include "Game.h"
#include "PinochleDeck.h"
#include <iterator>

enum class PinochleMelds
{
    dix,
    offsuitmarriage,
    fortyjacks,
    pinochle,
    insuitmarriage,
    sixtyqueens,
    eightykings,
    hundredaces,
    insuitrun,
    doublepinochle,
    fourhundredjacks,
    sixhundredqueens,
    eighthundredkings,
    thousandaces,
    insuitdoublerun
};
unsigned int meldPoints[15] = {10, 20, 40, 40, 40, 60, 80, 100, 150, 300, 400, 600, 800, 1000, 1500};

class PinochleGame : public Game
{
private:
    void suit_independent_evaluation(const CardSet<PinochleRank, Suit> &hand, vector<PinochleMelds> &melds);
    void suit_dependent_evaluation(const CardSet<PinochleRank, Suit> &hand, vector<PinochleMelds> &melds, Suit s);
    void awardContract(vector<unsigned int> &bids);
    void startTricking();
    void determineNextCard(CardSet<PinochleRank, Suit> &handset, vector<Card<PinochleRank, Suit>> &trick, Suit leadSuit);
    void checkWinner();
    Suit trump_suit;
    unsigned int contractWinner;
    CardSet<PinochleRank, Suit> trick;
    unsigned int trickFirstPlayer;
   
    
protected:
    PinochleDeck deck;
    vector<CardSet<PinochleRank, Suit>> hands;
    virtual void deal();
    void printHands();
    void collectAllCards();
    

public:
    PinochleGame(int argc, const char *argv[]);
    virtual int play();
    static int getPointValue(PinochleMelds meld);
};

ostream &operator<<(ostream &out, PinochleMelds meld);

int binarySearch(vector<Card<PinochleRank, Suit>> &cards, Card<PinochleRank, Suit> card, int low, int high);
bool hasCardOnce(vector<Card<PinochleRank, Suit>> &cards, Card<PinochleRank, Suit> card);
bool hasCardTwice(vector<Card<PinochleRank, Suit>> &cards, Card<PinochleRank, Suit> card);
bool hasThousandAces(vector<Card<PinochleRank, Suit>> cards);
bool hasHundredAces(vector<Card<PinochleRank, Suit>> cards);
bool hasEightHundredKings(vector<Card<PinochleRank, Suit>> cards);
bool hasEightyKings(vector<Card<PinochleRank, Suit>> cards);
bool hasSixHundredQueens(vector<Card<PinochleRank, Suit>> cards);
bool hasSixtyQueens(vector<Card<PinochleRank, Suit>> cards);
bool hasFourHundredJacks(vector<Card<PinochleRank, Suit>> cards);
bool hasFortyJacks(vector<Card<PinochleRank, Suit>> cards);
bool hasDoublePinochle(vector<Card<PinochleRank, Suit>> cards);
bool hasPinochle(vector<Card<PinochleRank, Suit>> cards);
bool hasInsuitDoubleRun(vector<Card<PinochleRank, Suit>> cards, Suit &suit);
bool hasInsuitRun(vector<Card<PinochleRank, Suit>> cards, Suit &suit);
bool hasInsuitMarriage(vector<Card<PinochleRank, Suit>> cards, Suit &suit);
bool hasOffsuitMarriage(vector<Card<PinochleRank, Suit>> cards, Suit &suit);
bool hasDix(vector<Card<PinochleRank, Suit>> cards, Suit &suit);
int calculateBid(CardSet<PinochleRank, Suit>& hand, vector<PinochleMelds>& melds);
int calculateContract(vector<PinochleMelds>& firstMelds, vector<PinochleMelds>& secondMelds);
int getHighestRank(vector<Card<PinochleRank, Suit>> &hand, Suit suit);
int getLowestRankExclude(vector<Card<PinochleRank, Suit>> &hand, Suit suit);
int getLowestRankInclude(vector<Card<PinochleRank, Suit>> &hand, Suit suit);
int calculateTrick(vector<Card<PinochleRank, Suit>> &trickHand);

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE /* test whether guard symbol is defined */
#include "PinochleGame.cpp"
#endif