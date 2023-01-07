// Junxian Zhang - junxian@wustl.edu

// HoldEmGame.cpp: This file contains the definitions of the methods, constructor, and member variable for the HoldEmGame class.

#include "HoldEmGame.h"
// Gives parent class game its parameters to store player names and sets the starting state
HoldEmGame::HoldEmGame(int argc, const char *argv[]) : Game(argc, argv), state(HoldEmState::preflop)
{
    // Creates new empty hands and put them in a vector
    for (int i = 2; i < argc; ++i)
    {
        CardSet<HoldEmRank, Suit> newHand;
        hands.push_back(newHand);
    }
    scores.insert(scores.begin(), hands.size(), 60);
    foldedPlayers = 0;
}

void HoldEmGame::deal()
{
    // Uses a switch statement too determine how the game should deal cards depending on the state of the game
    switch (state)
    {
    case HoldEmState::preflop:
        for (size_t i = 0; i < hands.size(); ++i)
        {
            deck >> hands.at((dealer + i + 1) % hands.size());
        }
        for (size_t i = 0; i < hands.size(); ++i)
        {
            deck >> hands.at((dealer + i + 1) % hands.size());
        }
        state = HoldEmState::flop;
        break;
    case HoldEmState::flop:
        deck >> board;
        deck >> board;
        deck >> board;
        cout << "BOARD (flop):" << endl;
        printBoard();
        state = HoldEmState::turn;
        break;
    case HoldEmState::turn:
        deck >> board;
        cout << "BOARD (turn):" << endl;
        printBoard();
        state = HoldEmState::river;
        break;
    case HoldEmState::river:
        deck >> board;
        cout << "BOARD (river):" << endl;
        printBoard();
        state = HoldEmState::undefined;
        break;
    case HoldEmState::undefined:
        break;
    }
}

void HoldEmGame::printHands()
{
    // Prints the hands of each player with the player's name
    for (size_t i = 0; i < players.size(); ++i)
    {
        cout << players.at(i).playerName;
        if (dealer == i)
        {
            cout << "*";
        }
        cout << "'s Hand: " << endl;
        players.at(i).hand.print(cout, 2);
        cout << endl;
    }
}

void HoldEmGame::printBoard()
{
    // Prints the cards on the board
    board.print(cout, 5);
    cout << endl;
}

void HoldEmGame::collectAllCards()
{
    // Collect all the cards back from the players and board and put them back into the deck
    for (typename vector<CardSet<HoldEmRank, Suit>>::iterator it = hands.begin(); it != hands.end(); ++it)
    {
        while (!(*it).is_empty())
        {
            deck.collect(*it);
        }
    }
    while (!board.is_empty())
    {
        deck.collect(board);
    }
}

int HoldEmGame::play()
{
    // Sets up how the game should run and will continue to run until the user wants to stop
    do
    {
        //Sets up the player vector
        players.clear();
        for (size_t i = 0; i < hands.size(); ++i)
        {
            HoldEmGame::PlayerStatus player = HoldEmGame::PlayerStatus(hands.at(i), player_names.at(i), HoldEmHandRank::undefined, i);
            players.push_back(player);
        }
        //Runs the while loop until there is only one person left with money
        while (players.size() != 1)
        {
            dealer = players.size() - 1;
            deck.shuffle();
            state = HoldEmState::preflop;

            // Pre-flop
            deal();
            prepareBetting(players);
            fillPlayerStatus(players);
            printHands();
            conductBetting(players, state);
            cout << endl;

            // Flop
            deal();
            fillPlayerStatus(players);
            evaluateFlop(players);
            cout << endl;
            conductBetting(players, state);

            // Turn
            deal();
            fillPlayerStatus(players);
            evaluateTurn(players);
            cout << endl;
            conductBetting(players, state);

            // River
            deal();
            fillPlayerStatus(players);
            evaluateRiver(players);
            cout << endl;
            conductBetting(players, state);

            cout << "Final Board:" << endl;
            printBoard();
            for(size_t i = 0; i < scores.begin(); ++i){
                cout << scores.at(i) << endl;
            }
            determineWinner(players);
            collectAllCards();

            for (size_t i = 0; i < players.size(); ++i)
            {
                players.at(i).folded = false;
            }
            foldedPlayers = 0;
        }
        //Prints out the winner with their score
        cout << endl;
        cout << players.at(0).playerName << " wins the game with a final chip count of " << scores.at(players.at(0).pos) << endl;
        scores.clear();
        scores.insert(scores.begin(), hands.size(), 60);
    } while (!endGame());

    return SUCCESSFULLY_ENDED_GAME;
}
void HoldEmGame::conductBetting(vector<PlayerStatus> &players, HoldEmState state)
{
    //While loop keeps looping until the players can't continue the betting round
    while (!endBettingRound(players))
    {
        
        //Loops through every player
        for (size_t i = 0; i < players.size(); ++i)
        {
            unsigned int position = 1;
            if (state == HoldEmState::flop)
            {
                position = 3;
            }

            ++players.at((dealer + i + position) % players.size()).round;
            if (!players.at((dealer + i + position) % players.size()).folded)
            {
                switch (state)
                {
                case HoldEmState::preflop:
                    break;
                case HoldEmState::flop:

                    preflopBetStrat(players.at((dealer + i + position) % players.size()));

                    break;
                case HoldEmState::turn:

                    flopBetStrat(players.at((dealer + i + position) % players.size()));

                    break;
                case HoldEmState::river:

                    turnBetStrat(players.at((dealer + i + position) % players.size()));

                    break;
                case HoldEmState::undefined:

                    riverBetStrat(players.at((dealer + i + position) % players.size()));
                    break;
                }
            }
        }
    }
    for (size_t i = 0; i < players.size(); ++i)
    {
        pot += players.at(i).currentBet;
        largestBet = 0;
        players.at(i).currentBet = 0;
        players.at(i).round = 0;
    }
}
//Checks if anyone has raised the largest bet
bool HoldEmGame::hasRaised(PlayerStatus &player)
{

    return (player.currentBet < largestBet);
}
//Prepares the betting rounds
void HoldEmGame::prepareBetting(vector<PlayerStatus> &players)
{

    largestBet = 0;
    players.at((dealer + 1) % players.size()).currentBet = 1;
    players.at((dealer + 2) % players.size()).currentBet = 2;
    scores.at(players.at((dealer + 1) % players.size()).pos) -= 1;
    scores.at(players.at((dealer + 2) % players.size()).pos) -= 2;
    largestBet = 2;
    bigBlind = players.at((dealer + 2) % players.size()).pos;
}
//Checks if the betting round is over or not
bool HoldEmGame::endBettingRound(vector<PlayerStatus> &players)
{
    bool end = true;
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        if (!(*it).folded)
        {
            if (((*it).currentBet != largestBet) && (scores.at((*it).pos) != 0))
            {
                end = false;
            }
        }
    }
    return end;
}
//Finds the winner of the betting rounds by looking at their cards
void HoldEmGame::determineWinner(vector<PlayerStatus> &players)
{
    vector<PlayerStatus> playersCopy(players);
    vector<int> winners;
    players.clear();
    //loops throug the players starting with the one with the highest hand rank
    for (int i = ((int)playersCopy.size()) - 1; i >= 0; --i)
    {
        if (!playersCopy.at(i).folded)
        {
            if (winners.size() == 0)
            {
                winners.push_back(i);
            }
            else
            {
                if (playersCopy.at(winners.at(0)).handRank == playersCopy.at(i).handRank)
                {
                    if (isSameHand(playersCopy.at(winners.at(0)).hand, playersCopy.at(i).hand))
                    {
                        winners.push_back(i);
                    }
                }
            }
        }
    }
    //Calculates how to split the pot for the winner(s)
    if (winners.size() == 1)
    {
        scores.at(playersCopy.at(winners.at(0)).pos) += pot;
    }
    else if (winners.size() > 1)
    {
        unsigned int potToShare = pot;
        unsigned int numOfWinners = winners.size();
        unsigned int perShare = pot / numOfWinners;
        unsigned int leftOver = potToShare - (perShare * numOfWinners);

        for (auto it = winners.begin(); it != winners.end(); ++it)
        {
            scores.at(playersCopy.at(*it).pos) += perShare;
        }
        scores.at(playersCopy.at(winners.at(0)).pos) += leftOver;
    }
    //Finds the players who busted out
    for (size_t i = 0; i < playersCopy.size(); ++i)
    {
        if (scores.at(playersCopy.at(i).pos) != 0)
        {
            players.push_back(playersCopy.at(i));
        }
        else
        {
            cout << playersCopy.at(i).playerName << " have busted out. "
                 << "Place No." << playersCopy.size() << endl;
        }
    }
    pot = 0;
}
//Checks if two players have the same hand or not, in order to see if there are multiple winners
bool isSameHand(CardSet<HoldEmRank, Suit> playersHand1, CardSet<HoldEmRank, Suit> playersHand2)
{
    auto hand1 = playersHand1.*(CardSet<HoldEmRank, Suit>::getCards());
    auto hand2 = playersHand2.*(CardSet<HoldEmRank, Suit>::getCards());
    sort(hand1.begin(), hand1.end(), [](Card<HoldEmRank, Suit> card1, Card<HoldEmRank, Suit> card2)
         { return lessThanRank(card1, card2); });
    sort(hand2.begin(), hand2.end(), [](Card<HoldEmRank, Suit> card1, Card<HoldEmRank, Suit> card2)
         { return lessThanRank(card1, card2); });

    for (size_t i = 0; i < hand1.size(); ++i)
    {
        if (hand1.at(i).rank != hand2.at(i).rank)
        {
            return false;
        }
    }
    return true;
}
//Betting Strategy For Preflop
void HoldEmGame::preflopBetStrat(PlayerStatus &player)
{

    pot = 0;

    auto cards = player.hand.*(CardSet<HoldEmRank, Suit>::getCards());
    vector<Card<HoldEmRank, Suit>> hand(cards);
    sort(hand.begin(), hand.end(), [](Card<HoldEmRank, Suit> card1, Card<HoldEmRank, Suit> card2)
         { return lessThanRank(card1, card2); });

    if (isPair(cards))
    {

        if (cards.at(0).rank == HoldEmRank::ace)
        {
            raise(player, 2);
        }
        else if (cards.at(0).rank == HoldEmRank::king || cards.at(0).rank == HoldEmRank::queen || cards.at(0).rank == HoldEmRank::jack)
        {
            if (player.round == 1)
            {
                raise(player, 2);
            }
            else
            {
                call(player);
            }
        }
        else
        {
            call(player);
        }
    }
    else
    {

        if (cards.at(0).suit == cards.at(1).suit)
        {
            call(player);
        }
        else if ((cards.at(0).rank > HoldEmRank::three) && (cards.at(1).rank > HoldEmRank::three))
        {
            Card<HoldEmRank, Suit> tempCard1 = Card<HoldEmRank, Suit>(cards.at(0).rank, cards.at(0).suit);
            Card<HoldEmRank, Suit> tempCard2 = Card<HoldEmRank, Suit>(cards.at(1).rank, cards.at(1).suit);

            if (((++tempCard1.rank) == tempCard2.rank))
            {
                call(player);
            }
            else
            {
                fold(player);
            }
        }
        else
        {
            fold(player);
        }
    }
}
//Betting Strategy For Flop
void HoldEmGame::flopBetStrat(PlayerStatus &player)
{

    auto cards = player.hand.*(CardSet<HoldEmRank, Suit>::getCards());
    if (player.handRank >= HoldEmHandRank::threeofakind)
    {
        raise(player, 2);
    }
    else if (player.handRank == HoldEmHandRank::twopair)
    {
        if (player.round == 1)
        {

            raise(player, 2);
        }
        else
        {

            call(player);
        }
    }
    else if (player.handRank == HoldEmHandRank::pair || isFourSameSuit(cards) || isFourConsecutive(cards))
    {
        if (hasRaised(player))
        {

            call(player);
        }
        else
        {

            check(player);
        }
    }
    else
    {
        if (hasRaised(player))
        {

            fold(player);
        }
        else
        {

            check(player);
        }
    }
}
//Betting Strategy For Turn
void HoldEmGame::turnBetStrat(PlayerStatus &player)
{
    auto cards = player.hand.*(CardSet<HoldEmRank, Suit>::getCards());
    if (player.handRank >= HoldEmHandRank::straight)
    {
        raise(player, 4);
    }
    else if (player.handRank == HoldEmHandRank::threeofakind)
    {
        if (player.round == 1)
        {
            raise(player, 4);
        }
        else
        {
            call(player);
        }
    }
    else if ((player.handRank == HoldEmHandRank::pair) || (player.handRank == HoldEmHandRank::twopair) || isFourSameSuit(cards) || isFourConsecutive(cards))
    {
        if (hasRaised(player))
        {
            call(player);
        }
        else
        {
            check(player);
        }
    }
    else
    {
        if (hasRaised(player))
        {
            fold(player);
        }
        else
        {
            check(player);
        }
    }
}
//Betting Strategy For River
void HoldEmGame::riverBetStrat(PlayerStatus &player)
{

    if (player.handRank >= HoldEmHandRank::straight)
    {
        raise(player, 4);
    }
    else if (player.handRank == HoldEmHandRank::threeofakind)
    {
        if (player.round == 1)
        {
            raise(player, 4);
        }
        else
        {
            call(player);
        }
    }
    else if ((player.handRank == HoldEmHandRank::twopair))
    {
        if (hasRaised(player))
        {
            call(player);
        }
        else
        {
            check(player);
        }
    }
    else
    {
        if (hasRaised(player))
        {
            fold(player);
        }
        else
        {
            check(player);
        }
    }
}
//Makes the player fold
void HoldEmGame::fold(PlayerStatus &player)
{
    if (bigBlind == player.pos && foldedPlayers == (players.size() - 1))
    {
        check(player);
    }
    else
    {
        player.folded = true;
        ++foldedPlayers;
        cout << player.playerName << " folds." << endl;
    }
}
//Makes the player call
void HoldEmGame::call(PlayerStatus &player)
{
    if ((player.currentBet == largestBet) && (player.pos == bigBlind))
    {
        check(player);
    }
    else
    {
        unsigned int diff = largestBet - player.currentBet;
        if (diff > scores.at(player.pos))
        {
            player.currentBet += scores.at(player.pos);
            cout << player.playerName << " calls." << endl;
            scores.at(player.pos) = 0;
        }
        else
        {
            scores.at(player.pos) -= diff;
            player.currentBet = largestBet;
            cout << player.playerName << " calls." << endl;
        }
    }
}
//Makes the player raise by x amount
void HoldEmGame::raise(PlayerStatus &player, unsigned int x)
{
    if (x > scores.at(player.pos))
    {
        player.currentBet = largestBet + scores.at(player.pos);
        largestBet = player.currentBet;
        cout << player.playerName << " raises the bet by " << scores.at(player.pos) << "." << endl;
        scores.at(player.pos) = 0;
    }
    else
    {
        int diff = (largestBet + x) - player.currentBet;
        scores.at(player.pos) -= diff;
        player.currentBet = largestBet + x;
        largestBet = player.currentBet;
        cout << player.playerName << " raises the bet by " << x << "." << endl;
    }
}
//Makes the player check
void HoldEmGame::check(PlayerStatus &player)
{
    cout << player.playerName << " checks." << endl;
}
//Fill the player struct with their hands
void HoldEmGame::fillPlayerStatus(vector<PlayerStatus> &players)
{

    // Creates PlayerStatus structs for each player and stores them in the vector

    for (size_t i = 0; i < players.size(); ++i)
    {
        players.at(i).hand = hands.at(players.at(i).pos);
    }
}
//Checks if four cards have the same suit
bool isFourSameSuit(vector<Card<HoldEmRank, Suit>> &cards)
{
    return (suitMatches(cards, cards.at(0)) == 4) || (suitMatches(cards, cards.at(1)) == 4);
}
//Checks if four cards are consecutive
bool isFourConsecutive(vector<Card<HoldEmRank, Suit>> &cards)
{
    vector<Card<HoldEmRank, Suit>> hand(cards);
    bool consec = true;
    sort(hand.begin(), hand.end(), [](Card<HoldEmRank, Suit> card1, Card<HoldEmRank, Suit> card2)
         { return lessThanRank(card1, card2); });

    for (int i = 0; i < 3; ++i)
    {
        Card<HoldEmRank, Suit> tempCard = Card<HoldEmRank, Suit>(hand.at(i).rank, hand.at(i).suit);

        if ((++tempCard.rank) != (hand.at(i + 1).rank))
        {
            consec = false;
        }
        if ((hand.at(i).rank == HoldEmRank::ace) || (hand.at(i + 1).rank == HoldEmRank::ace))
        {
            consec = false;
        }
    }
    if (consec)
    {
        return consec;
    }
    consec = true;
    for (int i = 1; i < 4; ++i)
    {
        Card<HoldEmRank, Suit> tempCard = Card<HoldEmRank, Suit>(hand.at(i).rank, hand.at(i).suit);

        if ((++tempCard.rank) != (hand.at(i + 1).rank))
        {
            consec = false;
        }
        if ((hand.at(i).rank == HoldEmRank::ace) || (hand.at(i + 1).rank == HoldEmRank::ace))
        {
            consec = false;
        }
    }
    return consec;
}

//Evaluates the hand rank for each player during flop
void HoldEmGame::evaluateFlop(vector<PlayerStatus> &players)
{

    // Adds the cards from the board to each player's hand in the struct and then evaluates it to get a Hand Rank
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        CardSet<HoldEmRank, Suit> table(board);
        while (!table.is_empty())
        {
            table >> (*it).hand;
        }
        (*it).handRank = holdem_hand_evaluation((*it).hand);
    }
    sort(players.begin(), players.end());
    // Prints the PlayerStatus structs based on the Hand Rank from highest to lowest
    for (int i = ((int)players.size()) - 1; i >= 0; --i)
    {
        cout << endl;
        cout << "[" << players.at(i).playerName << "]" << endl;
        cout << "Cards: ";
        players.at(i).hand.print(cout, 5);
        cout << "Hand Rank: " << players.at(i).handRank << endl;
    }
}
//Evaluates best the hand rank for each player during turn
void HoldEmGame::evaluateTurn(vector<PlayerStatus> &players)
{

    // Adds the cards from the board to each player's hand in the struct and then evaluates it to get a Hand Rank
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        CardSet<HoldEmRank, Suit> table(board);
        HoldEmGame::PlayerStatus player(*it);
        vector<Card<HoldEmRank, Suit>> allCards;
        auto tableHand = table.*(CardSet<HoldEmRank, Suit>::getCards());
        auto playerHand = player.hand.*(CardSet<HoldEmRank, Suit>::getCards());
        for (size_t i = 0; i < tableHand.size(); ++i)
        {
            allCards.push_back(tableHand.at(i));
        }
        for (size_t i = 0; i < playerHand.size(); ++i)
        {
            allCards.push_back(playerHand.at(i));
        }
        CardSet<HoldEmRank, Suit> newSet;
        (*it).handRank = getBestComboForSix(allCards, newSet);
        (*it).hand = newSet;
    }
    sort(players.begin(), players.end());
    // Prints the PlayerStatus structs based on the Hand Rank from highest to lowest
    for (int i = ((int)players.size()) - 1; i >= 0; --i)
    {
        cout << endl;
        cout << "[" << players.at(i).playerName << "]" << endl;
        cout << "Cards: ";
        players.at(i).hand.print(cout, 5);
        cout << "Hand Rank: " << players.at(i).handRank << endl;
    }
}
//Evaluates best the hand rank for each player during river
void HoldEmGame::evaluateRiver(vector<PlayerStatus> &players)
{

    // Adds the cards from the board to each player's hand in the struct and then evaluates it to get a Hand Rank
    for (auto it = players.begin(); it != players.end(); ++it)
    {

        CardSet<HoldEmRank, Suit> table(board);
        HoldEmGame::PlayerStatus player(*it);
        vector<Card<HoldEmRank, Suit>> allCards;
        auto tableHand = table.*(CardSet<HoldEmRank, Suit>::getCards());
        auto playerHand = player.hand.*(CardSet<HoldEmRank, Suit>::getCards());

        for (size_t i = 0; i < tableHand.size(); ++i)
        {
            allCards.push_back(tableHand.at(i));
        }
        for (size_t i = 0; i < playerHand.size(); ++i)
        {
            allCards.push_back(playerHand.at(i));
        }
        CardSet<HoldEmRank, Suit> newSet;
        (*it).handRank = getBestComboForSeven(allCards, newSet);
        (*it).hand = newSet;
    }
    sort(players.begin(), players.end());
    // Prints the PlayerStatus structs based on the Hand Rank from highest to lowest
    for (int i = ((int)players.size()) - 1; i >= 0; --i)
    {
        cout << endl;
        cout << "[" << players.at(i).playerName << "]" << endl;
        cout << "Cards: ";
        players.at(i).hand.print(cout, 5);
        cout << "Hand Rank: " << players.at(i).handRank << endl;
    }
}
ostream &operator<<(ostream &out, HoldEmHandRank handRank)
{
    // Uses a switch statement to print out the label related to the Hand Rank for displaying purposes
    switch (handRank)
    {
    case HoldEmHandRank::xhigh:
        out << "High Card";
        break;
    case HoldEmHandRank::pair:
        out << "One Pair";
        break;
    case HoldEmHandRank::twopair:
        out << "Two Pair";
        break;
    case HoldEmHandRank::threeofakind:
        out << "Three of a Kind";
        break;
    case HoldEmHandRank::straight:
        out << "Straight";
        break;
    case HoldEmHandRank::flush:
        out << "Flush";
        break;
    case HoldEmHandRank::fullhouse:
        out << "Full House";
        break;
    case HoldEmHandRank::fourofakind:
        out << "Four of a Kind";
        break;
    case HoldEmHandRank::straightflush:
        out << "Straight Flush";
        break;
    case HoldEmHandRank::undefined:
        out << "undefined";
        break;
    }
    return out;
}

HoldEmHandRank HoldEmGame::holdem_hand_evaluation(const CardSet<HoldEmRank, Suit> &hand)
{
    // Sorts the cards from lowest to highest and then calls the necessary functions to evaluate for Hand Rank
    CardSet<HoldEmRank, Suit> playersHand(hand);
    auto cards = playersHand.*(CardSet<HoldEmRank, Suit>::getCards());
    sort(cards.begin(), cards.end(), [](Card<HoldEmRank, Suit> card1, Card<HoldEmRank, Suit> card2)
         { return lessThanRank(card1, card2); });
    if (cards.size() != 5)
    {
        return HoldEmHandRank::undefined;
    }
    else if (isStraightFlush(cards))
    {
        return HoldEmHandRank::straightflush;
    }
    else if (isFourOfAKind(cards))
    {
        return HoldEmHandRank::fourofakind;
    }
    else if (isFullHouse(cards))
    {
        return HoldEmHandRank::fullhouse;
    }
    else if (isFlush(cards))
    {
        return HoldEmHandRank::flush;
    }
    else if (isStraight(cards))
    {
        return HoldEmHandRank::straight;
    }
    else if (isThreeOfAKind(cards))
    {
        return HoldEmHandRank::threeofakind;
    }
    else if (isTwoPair(cards))
    {
        return HoldEmHandRank::twopair;
    }
    else if (isPair(cards))
    {
        return HoldEmHandRank::pair;
    }
    else
    {
        return HoldEmHandRank::xhigh;
    }
}

bool isStraightFlush(vector<Card<HoldEmRank, Suit>> &cards)
{
    // Checks if the suits are the same
    if (suitMatches(cards, cards.at(0)) != 5)
    {
        return false;
    }
    // Checks if the ranks are consecutive (excludes anything with K A 2)
    for (int i = 0; i < 4; ++i)
    {
        Card<HoldEmRank, Suit> tempCard = Card<HoldEmRank, Suit>(cards.at(i).rank, cards.at(i).suit);
        if ((++tempCard.rank) == (cards.at(i + 1).rank))
        {
            if (((cards.at(i).rank) == HoldEmRank::five) && ((cards.at(i + 1).rank) == HoldEmRank::ace))
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool isFourOfAKind(vector<Card<HoldEmRank, Suit>> &cards)
{
    // Checks if any four cards of the five have the same rank
    return ((rankMatches(cards, cards.at(0)) == 4) || (rankMatches(cards, cards.at(1)) == 4));
}

bool isFullHouse(vector<Card<HoldEmRank, Suit>> &cards)
{
    // Checks if any cards of the five don't have a match of more than one (which is itself)
    for (auto it = cards.begin(); it != cards.end(); ++it)
    {
        if (rankMatches(cards, *it) < 2)
        {
            return false;
        }
    }
    return true;
}

bool isFlush(vector<Card<HoldEmRank, Suit>> &cards)
{
    // Checks if all of the suits are the same or not
    if (suitMatches(cards, cards.at(0)) != 5)
    {
        return false;
    }
    return true;
}

bool isStraight(vector<Card<HoldEmRank, Suit>> &cards)
{
    // Checks if the ranks are consecutive (excludes anything with K A 2)
    for (int i = 0; i < 4; ++i)
    {
        Card<HoldEmRank, Suit> tempCard = Card<HoldEmRank, Suit>(cards.at(i).rank, cards.at(i).suit);

        if ((++tempCard.rank) == (cards.at(i + 1).rank))
        {
            if (((cards.at(i).rank) == HoldEmRank::five) && ((cards.at(i + 1).rank) == HoldEmRank::ace))
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool isThreeOfAKind(vector<Card<HoldEmRank, Suit>> &cards)
{
    // Checks if any three cards of the five have the same rank
    return ((rankMatches(cards, cards.at(0)) == 3) || (rankMatches(cards, cards.at(1)) == 3) || (rankMatches(cards, cards.at(2)) == 3));
}

bool isTwoPair(vector<Card<HoldEmRank, Suit>> &cards)
{
    // Check if 4 cards give a 2 for rank matches
    int pairs = 0;
    for (auto it = cards.begin(); it != cards.end(); ++it)
    {
        if (rankMatches(cards, *it) == 2)
        {
            pairs = pairs + 1;
        }
    }
    if (pairs == 4)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isPair(vector<Card<HoldEmRank, Suit>> &cards)
{
    // Check if 2 cards give a 2 for rank matches
    int pairs = 0;
    for (auto it = cards.begin(); it != cards.end(); ++it)
    {
        if (rankMatches(cards, *it) == 2)
        {
            pairs = pairs + 1;
        }
    }
    if (pairs == 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int suitMatches(vector<Card<HoldEmRank, Suit>> &cards, Card<HoldEmRank, Suit> card)
{
    // Finds how many suit matches a card has with a hand
    int totalMatches = 0;
    for (auto it = cards.begin(); it != cards.end(); ++it)
    {
        if (!equalSuit(card, *it))
        {
            ++totalMatches;
        }
    }
    return totalMatches;
}

int rankMatches(vector<Card<HoldEmRank, Suit>> &cards, Card<HoldEmRank, Suit> card)
{
    // Finds how many rank matches a card has with a hand
    int totalMatches = 0;
    for (auto it = cards.begin(); it != cards.end(); ++it)
    {

        if (equalRank(card, *it))
        {
            ++totalMatches;
        }
    }

    return totalMatches;
}

bool operator<(const HoldEmGame::PlayerStatus &p1, const HoldEmGame::PlayerStatus &p2)
{

    // Checks if p1 is less than p2 based on Hand Rank
    if (p1.handRank < p2.handRank)
    {
        return true;
    }
    else if (p1.handRank > p2.handRank)
    {
        return false;
    }
    else
    {
        CardSet<HoldEmRank, Suit> playersHand1(p1.hand);
        CardSet<HoldEmRank, Suit> playersHand2(p2.hand);

        auto hand1 = playersHand1.*(CardSet<HoldEmRank, Suit>::getCards());
        auto hand2 = playersHand2.*(CardSet<HoldEmRank, Suit>::getCards());

        sort(hand1.begin(), hand1.end(), [](Card<HoldEmRank, Suit> card1, Card<HoldEmRank, Suit> card2)
             { return lessThanRank(card1, card2); });
        sort(hand2.begin(), hand2.end(), [](Card<HoldEmRank, Suit> card1, Card<HoldEmRank, Suit> card2)
             { return lessThanRank(card1, card2); });

        Card<HoldEmRank, Suit> card1(HoldEmRank::undefined, Suit::undefined);
        Card<HoldEmRank, Suit> card2(HoldEmRank::undefined, Suit::undefined);
        Card<HoldEmRank, Suit> card3(HoldEmRank::undefined, Suit::undefined);
        Card<HoldEmRank, Suit> card4(HoldEmRank::undefined, Suit::undefined);

        int pairIndex1;
        int pairIndex2;

        // Uses a switch statement to check more in depth if the Hand Rank is the same for both
        switch (p1.handRank)
        {
        case HoldEmHandRank::xhigh:
            // Checks the highest card first and then goes down the list
            for (int i = ((int)hand1.size()) - 1; i >= 0; --i)
            {

                if (hand1.at(i).rank < hand2.at(i).rank)
                {
                    return true;
                }
                else if (hand1.at(i).rank > hand2.at(i).rank)
                {
                    return false;
                }
            }
            return false;
            break;

        case HoldEmHandRank::pair:
            // Finds the first card that's part of a pair in the first hand
            for (size_t i = 0; i < hand1.size(); ++i)
            {
                if (rankMatches(hand1, hand1.at(i)) == 2)
                {
                    card1 = hand1.at(i);
                    pairIndex1 = i;
                    break;
                }
            }
            // Finds the first card that's part of a pair in the second hand
            for (size_t i = 0; i < hand2.size(); ++i)
            {
                if (rankMatches(hand2, hand2.at(i)) == 2)
                {
                    card2 = hand2.at(i);
                    pairIndex2 = i;
                    break;
                }
            }
            // Uses the rank of each card to check if one pair is less than the other
            if (card1.rank < card2.rank)
            {

                return true;
            }
            else if (card1.rank > card2.rank)
            {
                return false;
            }
            else
            {
                // Deletes the pair from each hand and then checks each individual card
                hand1.erase(hand1.begin() + pairIndex1);
                hand1.erase(hand1.begin() + pairIndex1);
                hand2.erase(hand2.begin() + pairIndex2);
                hand2.erase(hand2.begin() + pairIndex2);
                for (int i = ((int)hand1.size()) - 1; i >= 0; --i)
                {
                    if (hand1.at(i).rank < hand2.at(i).rank)
                    {
                        return true;
                    }
                    else if (hand1.at(i).rank > hand2.at(i).rank)
                    {
                        return false;
                    }
                }
                return false;
            }
            break;
        case HoldEmHandRank::twopair:
            // Gets the card of the highest pair and the unpaired card in each hand
            for (size_t i = 0; i < hand1.size(); ++i)
            {
                if (rankMatches(hand1, hand1.at(i)) == 2)
                {
                    if (card1.rank == HoldEmRank::undefined || card1.rank < hand1.at(i).rank)
                    {
                        card1 = hand1.at(i);
                    }
                }
                else if (rankMatches(hand1, hand1.at(i)) == 1)
                {
                    card3 = hand1.at(i);
                }
                if (rankMatches(hand2, hand2.at(i)) == 2)
                {
                    if (card2.rank == HoldEmRank::undefined || card2.rank < hand2.at(i).rank)
                    {
                        card2 = hand2.at(i);
                    }
                }
                else if (rankMatches(hand2, hand2.at(i)) == 1)
                {
                    card4 = hand2.at(i);
                }
            }
            // Compares the highest pair of each hand, and if they're equal, use the single unpaired card to compare
            if (card1.rank < card2.rank)
            {
                return true;
            }
            else if (card1.rank > card2.rank)
            {
                return false;
            }
            else
            {
                return (card3.rank < card4.rank);
            }
            break;
        case HoldEmHandRank::threeofakind:
            // Gets the card of the three of a kind for each hand
            for (size_t i = 0; i < hand1.size(); ++i)
            {
                if (rankMatches(hand1, hand1.at(i)) == 3)
                {
                    card1 = hand1.at(i);
                }
                if (rankMatches(hand2, hand2.at(i)) == 3)
                {
                    card2 = hand2.at(i);
                }
            }
            // Compares the card of the three of a kind to check which is less than based on rank
            if (card1.rank < card2.rank)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case HoldEmHandRank::straight:
            // Compares the highest card of each hand (checks if the first hand is A 2 3 4 5)
            if ((hand1.at(4).rank == HoldEmRank::ace) && (hand1.at(0).rank == HoldEmRank::two))
            {
                return true;
            }
            else
            {
                if (lessThanRank(hand1.at(4), hand2.at(4)))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            break;
        case HoldEmHandRank::flush:
            // Checks the highest card first and then goes down the list
            for (int i = ((int)hand1.size()) - 1; i >= 0; --i)
            {
                if (hand1.at(i).rank < hand2.at(i).rank)
                {
                    return true;
                }
                else if (hand1.at(i).rank > hand2.at(i).rank)
                {
                    return false;
                }
                else
                {
                    return false;
                }
            }
            break;
        case HoldEmHandRank::fullhouse:
            // Gets the card of the three cards that have the same rank in each hand
            for (size_t i = 0; i < hand1.size(); ++i)
            {
                if (rankMatches(hand1, hand1.at(i)) == 3)
                {
                    card1 = hand1.at(i);
                }
                if (rankMatches(hand2, hand2.at(i)) == 3)
                {
                    card2 = hand2.at(i);
                }
            }
            // Compares the card to see which one is less
            if (card1.rank < card2.rank)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case HoldEmHandRank::fourofakind:
            // Gets the card of the four of a kind in each hand
            for (size_t i = 0; i < hand1.size(); ++i)
            {
                if (rankMatches(hand1, hand1.at(i)) == 4)
                {
                    card1 = hand1.at(i);
                }
                if (rankMatches(hand2, hand2.at(i)) == 4)
                {
                    card2 = hand2.at(i);
                }
            }
            // Compares the card to see which one is less
            if (card1.rank < card2.rank)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case HoldEmHandRank::straightflush:
            // Compares the highest card of each hand (checks if the first hand is A 2 3 4 5)
            if ((hand1.at(4).rank == HoldEmRank::ace) && (hand1.at(0).rank == HoldEmRank::two))
            {
                return true;
            }
            else
            {
                if (lessThanRank(hand1.at(4), hand2.at(4)))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            break;
        case HoldEmHandRank::undefined:
            return false;
            break;
        }
    }
    return false;
}
//Add five cards to a cardset
void addAllCards(CardSet<HoldEmRank, Suit> &set, Card<HoldEmRank, Suit> c1, Card<HoldEmRank, Suit> c2, Card<HoldEmRank, Suit> c3, Card<HoldEmRank, Suit> c4, Card<HoldEmRank, Suit> c5)
{

    set.addCard(c1);
    set.addCard(c2);
    set.addCard(c3);
    set.addCard(c4);
    set.addCard(c5);
}
//Find the best hand rank combination for six cards
HoldEmHandRank HoldEmGame::getBestComboForSix(vector<Card<HoldEmRank, Suit>> &allCards, CardSet<HoldEmRank, Suit> &cardSet)
{
    /*
            Combinations of 6 cards for 5:
            0 1 2 3 4
            0 1 2 3 5
            0 1 2 4 5
            0 1 3 4 5
            0 2 3 4 5
            1 2 3 4 5
        */
    vector<HoldEmHandRank> handRanks;
    vector<CardSet<HoldEmRank, Suit>> cardSets;
    CardSet<HoldEmRank, Suit> cardSet1;
    addAllCards(cardSet1, allCards.at(0), allCards.at(1), allCards.at(2), allCards.at(3), allCards.at(4));
    handRanks.push_back(holdem_hand_evaluation(cardSet1));
    cardSets.push_back(cardSet1);

    CardSet<HoldEmRank, Suit> cardSet2;
    addAllCards(cardSet2, allCards.at(0), allCards.at(1), allCards.at(2), allCards.at(3), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet2));
    cardSets.push_back(cardSet2);

    CardSet<HoldEmRank, Suit> cardSet3;
    addAllCards(cardSet3, allCards.at(0), allCards.at(1), allCards.at(2), allCards.at(4), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet3));
    cardSets.push_back(cardSet3);

    CardSet<HoldEmRank, Suit> cardSet4;
    addAllCards(cardSet4, allCards.at(0), allCards.at(1), allCards.at(3), allCards.at(4), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet4));
    cardSets.push_back(cardSet4);

    CardSet<HoldEmRank, Suit> cardSet5;
    addAllCards(cardSet5, allCards.at(0), allCards.at(2), allCards.at(3), allCards.at(4), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet5));
    cardSets.push_back(cardSet5);

    CardSet<HoldEmRank, Suit> cardSet6;
    addAllCards(cardSet6, allCards.at(1), allCards.at(2), allCards.at(3), allCards.at(4), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet6));
    cardSets.push_back(cardSet6);

    HoldEmHandRank highestRank = handRanks.front();
    unsigned highestIndex = 0;
    for (size_t i = 0; i < handRanks.size(); ++i)
    {
        if (highestRank < handRanks.at(i))
        {
            highestRank = handRanks.at(i);
            highestIndex = i;
        }
    }
    while (!cardSets.at(highestIndex).is_empty())
    {
        cardSets.at(highestIndex) >> cardSet;
    }

    return highestRank;
}
//Find the best hand rank combination for seven cards
HoldEmHandRank HoldEmGame::getBestComboForSeven(vector<Card<HoldEmRank, Suit>> &allCards, CardSet<HoldEmRank, Suit> &cardSet)
{
    /*
            Combinations of 7 cards for 5:
            0 1 2 3 4
            0 1 2 3 5
            0 1 2 3 6
            0 1 2 4 5
            0 1 2 4 6
            0 1 2 5 6
            0 1 3 4 5
            0 1 3 4 6
            0 1 3 5 6
            0 1 4 5 6
            0 2 3 4 5
            0 2 3 4 6
            0 2 3 5 6
            0 2 4 5 6
            0 3 4 5 6
            1 2 3 4 5
            1 2 3 4 6
            1 2 3 5 6
            1 2 4 5 6
            1 3 4 5 6
            2 3 4 5 6
    */
    vector<HoldEmHandRank> handRanks;
    vector<CardSet<HoldEmRank, Suit>> cardSets;
    CardSet<HoldEmRank, Suit> cardSet1;
    addAllCards(cardSet1, allCards.at(0), allCards.at(1), allCards.at(2), allCards.at(3), allCards.at(4));
    handRanks.push_back(holdem_hand_evaluation(cardSet1));
    cardSets.push_back(cardSet1);

    CardSet<HoldEmRank, Suit> cardSet2;
    addAllCards(cardSet2, allCards.at(0), allCards.at(1), allCards.at(2), allCards.at(3), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet2));
    cardSets.push_back(cardSet2);

    CardSet<HoldEmRank, Suit> cardSet3;
    addAllCards(cardSet3, allCards.at(0), allCards.at(1), allCards.at(2), allCards.at(3), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet3));
    cardSets.push_back(cardSet3);

    CardSet<HoldEmRank, Suit> cardSet4;
    addAllCards(cardSet4, allCards.at(0), allCards.at(1), allCards.at(2), allCards.at(4), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet4));
    cardSets.push_back(cardSet4);

    CardSet<HoldEmRank, Suit> cardSet5;
    addAllCards(cardSet5, allCards.at(0), allCards.at(1), allCards.at(2), allCards.at(4), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet5));
    cardSets.push_back(cardSet5);

    CardSet<HoldEmRank, Suit> cardSet6;
    addAllCards(cardSet6, allCards.at(0), allCards.at(1), allCards.at(2), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet6));
    cardSets.push_back(cardSet6);

    CardSet<HoldEmRank, Suit> cardSet7;
    addAllCards(cardSet7, allCards.at(0), allCards.at(1), allCards.at(3), allCards.at(4), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet7));
    cardSets.push_back(cardSet7);

    CardSet<HoldEmRank, Suit> cardSet8;
    addAllCards(cardSet8, allCards.at(0), allCards.at(1), allCards.at(3), allCards.at(4), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet8));
    cardSets.push_back(cardSet8);

    CardSet<HoldEmRank, Suit> cardSet9;
    addAllCards(cardSet9, allCards.at(0), allCards.at(1), allCards.at(3), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet9));
    cardSets.push_back(cardSet9);

    CardSet<HoldEmRank, Suit> cardSet10;
    addAllCards(cardSet10, allCards.at(0), allCards.at(1), allCards.at(4), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet10));
    cardSets.push_back(cardSet10);

    CardSet<HoldEmRank, Suit> cardSet11;
    addAllCards(cardSet11, allCards.at(0), allCards.at(2), allCards.at(3), allCards.at(4), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet11));
    cardSets.push_back(cardSet11);

    CardSet<HoldEmRank, Suit> cardSet12;
    addAllCards(cardSet12, allCards.at(0), allCards.at(2), allCards.at(3), allCards.at(4), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet12));
    cardSets.push_back(cardSet12);

    CardSet<HoldEmRank, Suit> cardSet13;
    addAllCards(cardSet13, allCards.at(0), allCards.at(2), allCards.at(3), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet13));
    cardSets.push_back(cardSet13);

    CardSet<HoldEmRank, Suit> cardSet14;
    addAllCards(cardSet14, allCards.at(0), allCards.at(2), allCards.at(4), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet14));
    cardSets.push_back(cardSet14);

    CardSet<HoldEmRank, Suit> cardSet15;
    addAllCards(cardSet15, allCards.at(0), allCards.at(3), allCards.at(4), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet15));
    cardSets.push_back(cardSet15);

    CardSet<HoldEmRank, Suit> cardSet16;
    addAllCards(cardSet16, allCards.at(1), allCards.at(2), allCards.at(3), allCards.at(4), allCards.at(5));
    handRanks.push_back(holdem_hand_evaluation(cardSet16));
    cardSets.push_back(cardSet16);

    CardSet<HoldEmRank, Suit> cardSet17;
    addAllCards(cardSet17, allCards.at(1), allCards.at(2), allCards.at(3), allCards.at(4), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet17));
    cardSets.push_back(cardSet17);

    CardSet<HoldEmRank, Suit> cardSet18;
    addAllCards(cardSet18, allCards.at(1), allCards.at(2), allCards.at(3), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet18));
    cardSets.push_back(cardSet18);

    CardSet<HoldEmRank, Suit> cardSet19;
    addAllCards(cardSet19, allCards.at(1), allCards.at(2), allCards.at(4), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet19));
    cardSets.push_back(cardSet19);

    CardSet<HoldEmRank, Suit> cardSet20;
    addAllCards(cardSet20, allCards.at(1), allCards.at(3), allCards.at(4), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet20));
    cardSets.push_back(cardSet20);

    CardSet<HoldEmRank, Suit> cardSet21;
    addAllCards(cardSet21, allCards.at(2), allCards.at(3), allCards.at(4), allCards.at(5), allCards.at(6));
    handRanks.push_back(holdem_hand_evaluation(cardSet21));
    cardSets.push_back(cardSet21);

    HoldEmHandRank highestRank = handRanks.front();
    unsigned highestIndex = 0;
    for (size_t i = 0; i < handRanks.size(); ++i)
    {
        if (highestRank < handRanks.at(i))
        {
            highestRank = handRanks.at(i);
            highestIndex = i;
        }
    }
    while (!cardSets.at(highestIndex).is_empty())
    {
        cardSets.at(highestIndex) >> cardSet;
    }
    return highestRank;
}
