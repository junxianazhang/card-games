// HoldEmGame.cpp: This file contains the definitions of the methods, constructor, and member variable for the HoldEmGame class.

#pragma once

#include "Game.h"
#include "HoldEmDeck.h"

enum class HoldEmState
{
    preflop,
    flop,
    turn,
    river,
    undefined
};
enum class HoldEmHandRank
{
    xhigh,
    pair,
    twopair,
    threeofakind,
    straight,
    flush,
    fullhouse,
    fourofakind,
    straightflush,
    undefined
};

class HoldEmGame : public Game
{
private:
    HoldEmHandRank holdem_hand_evaluation(const CardSet<HoldEmRank, Suit> &hand);
    HoldEmHandRank getBestComboForSix(vector<Card<HoldEmRank, Suit>> &allCards, CardSet<HoldEmRank, Suit> &cardSet);
    HoldEmHandRank getBestComboForSeven(vector<Card<HoldEmRank, Suit>> &allCards, CardSet<HoldEmRank, Suit> &cardSet);

protected:
    HoldEmState state;
    HoldEmDeck deck;
    unsigned int pot;
    unsigned int largestBet;
    unsigned int foldedPlayers;
    unsigned int bigBlind;
    vector<CardSet<HoldEmRank, Suit>> hands;
    CardSet<HoldEmRank, Suit> board;
    
    virtual void deal();
    void printHands();
    void printBoard();
    void collectAllCards();

public:
    HoldEmGame(int argc, const char *argv[]);
    virtual int play();
    
    struct PlayerStatus
    {
        CardSet<HoldEmRank, Suit> hand;
        string playerName;
        HoldEmHandRank handRank;
        unsigned int currentBet;
        bool folded;
        unsigned int pos;
        unsigned int round;
        PlayerStatus(CardSet<HoldEmRank, Suit> h, string pName, HoldEmHandRank hRank, int p) : hand(h), playerName(pName), handRank(hRank), currentBet(0), folded(false), pos(p), round(0){};
    };
    vector<PlayerStatus> players;
    void evaluateFlop(vector<PlayerStatus> &players);
    void evaluateTurn(vector<PlayerStatus> &players);
    void evaluateRiver(vector<PlayerStatus> &players);
    void fillPlayerStatus(vector<PlayerStatus> &players);
    void conductBetting(vector<PlayerStatus>& players, HoldEmState state);
    void fold(PlayerStatus &player);
    void call(PlayerStatus &player);
    void raise(PlayerStatus &player, unsigned int x);
    void check(PlayerStatus &player);
    void preflopBetStrat(PlayerStatus &player);
    void flopBetStrat(PlayerStatus &player);
    void turnBetStrat(PlayerStatus &player);
    void riverBetStrat(PlayerStatus &player);
    void determineWinner(vector<PlayerStatus> &players);
    bool endBettingRound(vector<PlayerStatus> &players);
    void prepareBetting(vector<PlayerStatus> &players);
    bool hasRaised(PlayerStatus &player);
};

ostream &operator<<(ostream &out, HoldEmHandRank handRank);

bool operator<(const HoldEmGame::PlayerStatus &p1, const HoldEmGame::PlayerStatus &p2);

bool isStraightFlush(vector<Card<HoldEmRank, Suit>> &cards);
bool isFourOfAKind(vector<Card<HoldEmRank, Suit>> &cards);
bool isFullHouse(vector<Card<HoldEmRank, Suit>> &cards);
bool isFlush(vector<Card<HoldEmRank, Suit>> &cards);
bool isStraight(vector<Card<HoldEmRank, Suit>> &cards);
bool isThreeOfAKind(vector<Card<HoldEmRank, Suit>> &cards);
bool isTwoPair(vector<Card<HoldEmRank, Suit>> &cards);
bool isPair(vector<Card<HoldEmRank, Suit>> &cards);
int suitMatches(vector<Card<HoldEmRank, Suit>> &cards, Card<HoldEmRank, Suit> card);
int rankMatches(vector<Card<HoldEmRank, Suit>> &cards, Card<HoldEmRank, Suit> card);
void addAllCards(Card<HoldEmRank, Suit> c1, Card<HoldEmRank, Suit> c2, Card<HoldEmRank, Suit> c3, Card<HoldEmRank, Suit> c4, Card<HoldEmRank, Suit> c5);
bool isFourSameSuit(vector<Card<HoldEmRank, Suit>> &cards);
bool isFourConsecutive(vector<Card<HoldEmRank, Suit>> &cards);
bool isSameHand(CardSet<HoldEmRank, Suit> playersHand1, CardSet<HoldEmRank, Suit> playersHand2);
#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE /* test whether guard symbol is defined */
#include "HoldEmGame.cpp"
#endif