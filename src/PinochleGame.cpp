// Junxian Zhang - junxian@wustl.edu

// PinochleGame.cpp: This file contains the definitions of the methods, constructor, and member variable for the PinochleGame class.

#include "PinochleGame.h"
#include <algorithm>

const unsigned int PLAYER_ONE = 0;
const unsigned int PLAYER_TWO = 1;
const unsigned int PLAYER_THREE = 2;
const unsigned int PLAYER_FOUR = 3;
const unsigned int TOTAL_PLAYERS = 4;
const unsigned int TEAM_ONE = 0;
const unsigned int TEAM_TWO = 1;
unsigned int tallyTotal = 0;
unsigned int teamOneBid = 0;
unsigned int teamTwoBid = 0;

// Gives parent class game its parameters to store player names and sets the starting state
PinochleGame::PinochleGame(int argc, const char *argv[]) : Game(argc, argv)
{
  // Creates new empty hands and put them in a vector
  for (int i = 2; i < argc; ++i)
  {
    CardSet<PinochleRank, Suit> newHand;
    hands.push_back(newHand);
  }
  trump_suit = Suit::undefined;
  scores.insert(scores.begin(), (TOTAL_PLAYERS / 2), 0);
}
void PinochleGame::deal()
{
  // Deals 3 cards to each player until there are no more cards in the deck
  while (!deck.is_empty())
  {
    for (size_t i = 0; i < hands.size(); ++i)
    {
      deck >> hands.at((dealer + i + 1) % hands.size());
      deck >> hands.at((dealer + i + 1) % hands.size());
      deck >> hands.at((dealer + i + 1) % hands.size());
    }
  }
}

void PinochleGame::printHands()
{
  // Prints the hands of each player with the player's name
  for (size_t i = 0; i < hands.size(); ++i)
  {
    cout << player_names.at(i);
    if (dealer == i)
    {
      cout << "*";
    }
    cout << "'s Hand: " << endl;
    hands.at(i).print(cout, 6);
    cout << endl;
  }
}

void PinochleGame::collectAllCards()
{
  // Collect all the cards back from the players and put them back into the deck
  for (typename vector<CardSet<PinochleRank, Suit>>::iterator it = hands.begin(); it != hands.end(); ++it)
  {
    while (!(*it).is_empty())
    {
      deck.collect(*it);
    }
  }
}

int PinochleGame::play()
{
  // Sets up how the game should run and will continue to run until the user wants to stop
  do
  {
    //Runs while loop until a team has more than 1500 points
    while ((scores.at(TEAM_ONE) < 1500) && (scores.at(TEAM_TWO) < 1500))
    {
      vector<unsigned int> bids(hands.size(), 0);
      awardContract(bids);
      if (contractWinner == TEAM_ONE)
      {
        if (bids.at(PLAYER_ONE) >= bids.at(PLAYER_THREE))
        {
          trickFirstPlayer = PLAYER_ONE;
        }
        else
        {
          trickFirstPlayer = PLAYER_THREE;
        }
      }
      else
      {
        if (bids.at(PLAYER_TWO) >= bids.at(PLAYER_FOUR))
        {
          trickFirstPlayer = PLAYER_TWO;
        }
        else
        {
          trickFirstPlayer = PLAYER_FOUR;
        }
      }
      startTricking();
      checkWinner();

    }
    scores.clear();
    scores.insert(scores.begin(), (TOTAL_PLAYERS / 2), 0);

  } while (!endGame());

  return SUCCESSFULLY_ENDED_GAME;
}
//Finds which team has more than 1500 points and declares it
void PinochleGame::checkWinner()
{

  if ((scores.at(TEAM_ONE) >= 1500))
  {
    cout << player_names.at(PLAYER_ONE) << " and " << player_names.at(PLAYER_THREE) << " are the winners of the game with " << scores.at(TEAM_ONE) << " points." << endl;
  }
  if ((scores.at(TEAM_TWO) >= 1500))
  {
    cout << player_names.at(PLAYER_TWO) << " and " << player_names.at(PLAYER_FOUR) << " are the winners of the game with " << scores.at(TEAM_TWO) << " points." << endl;
  }
}
//Start the trick process
void PinochleGame::startTricking()
{
 
  Suit leadingSuit;
  //Runs until all the cards have been played for each player
  while (!hands.at(trickFirstPlayer).is_empty())
  {

    vector<Card<PinochleRank, Suit>> &trickHand = trick.*(CardSet<PinochleRank, Suit>::getCards());
    vector<Card<PinochleRank, Suit>> &hand = hands.at(trickFirstPlayer).*(CardSet<PinochleRank, Suit>::getCards());

    unsigned int highestCard = getHighestRank(hand, trump_suit);

    trickHand.push_back(hand.at(highestCard));
    leadingSuit = hand.at(highestCard).suit;
    hands.at(trickFirstPlayer).removeCard(hand.at(highestCard));
    for (size_t i = 1; i < hands.size(); ++i)
    {
      determineNextCard(hands.at((trickFirstPlayer + i) % hands.size()), trickHand, leadingSuit);
    }
    unsigned int highestNonTrump = getHighestRank(trickHand, leadingSuit);
    unsigned int highestTrump = getHighestRank(trickHand, trump_suit);
    unsigned int trickWinner;
    if (trickHand.at(highestTrump).suit == trump_suit)
    {
      trickWinner = highestTrump;
    }
    else
    {
      trickWinner = highestNonTrump;
    }
    int trickTotal = calculateTrick(trickHand);
    trickFirstPlayer = trickWinner;
    if (trickWinner == PLAYER_ONE || trickWinner == PLAYER_THREE)
    {
      if (contractWinner == TEAM_ONE)
      {
        tallyTotal += trickTotal;
        if (hand.size() == 0)
        {
          tallyTotal += 10;
        }
      }
    }
    else
    {
      if (contractWinner == TEAM_TWO)
      {
        tallyTotal += trickTotal;
        if (hand.size() == 0)
        {
          tallyTotal += 10;
        }
      }
    }
    while (!trick.is_empty())
    {
      deck.collect(trick);
    }
    
  }
  if (contractWinner == TEAM_ONE)
  {
 
    if (tallyTotal >= teamOneBid)
    {

      scores.at(TEAM_ONE) += teamOneBid;
    }
  }
  else
  {
    if (tallyTotal >= teamTwoBid)
    {
      scores.at(TEAM_TWO) += teamTwoBid;
    }
  }
}
//Finds the best card to play next for each player
void PinochleGame::determineNextCard(CardSet<PinochleRank, Suit> &handset, vector<Card<PinochleRank, Suit>> &trickHand, Suit leadSuit)
{

  auto hand = handset.*(CardSet<PinochleRank, Suit>::getCards());
  unsigned int cardToPlay;
  unsigned int highestTrick;
  if (leadSuit == trump_suit)
  {
    // Trump leading
    cardToPlay = getHighestRank(hand, trump_suit);
    highestTrick = getHighestRank(trickHand, trump_suit);
    if ((hand.at(cardToPlay).rank <= trickHand.at(highestTrick).rank) || (hand.at(cardToPlay).suit != trump_suit))
    {
      cardToPlay = getLowestRankInclude(hand, trump_suit);
    }
  }
  else
  {
    // Nontrump leading
    unsigned int highestNonTrump = getHighestRank(hand, leadSuit);
    unsigned int highestTrump = getHighestRank(hand, trump_suit);
    highestTrick = getHighestRank(trickHand, trump_suit);
    if ((trickHand.at(highestTrick).suit != trump_suit) && (hand.at(highestNonTrump).suit == leadSuit))
    {
      if (hand.at(highestNonTrump).rank > trickHand.at(highestTrick).rank)
      {
        cardToPlay = highestNonTrump;
      }
      else
      {
        cardToPlay = getLowestRankInclude(hand, leadSuit);
      }
    }
    else if ((trickHand.at(highestTrick).suit == trump_suit) && (hand.at(highestTrump).suit == trump_suit))
    {
      if (hand.at(highestTrump).rank > trickHand.at(highestTrick).rank)
      {
        cardToPlay = highestTrump;
      }
      else
      {
        cardToPlay = getLowestRankExclude(hand, trump_suit);
      }
    }
    else if ((trickHand.at(highestTrick).suit == trump_suit) && (hand.at(highestTrump).suit != trump_suit))
    {

      cardToPlay = getLowestRankInclude(hand, leadSuit);
    }
    else
    {
      cardToPlay = highestNonTrump;
    }
  }
  trickHand.push_back(hand.at(cardToPlay));
  handset.removeCard(hand.at(cardToPlay));
}
//Calculuate which team should be awarded the contract based on the bids
void PinochleGame::awardContract(vector<unsigned int> &bids)
{

  deck.shuffle();
  deal();
  printHands();
  trump_suit = (hands.back().*(CardSet<PinochleRank, Suit>::getCards())).back().suit;
  vector<vector<PinochleMelds>> playerMelds;
  for (size_t i = 0; i < hands.size(); ++i)
  {
    vector<PinochleMelds> melds;

    cout << player_names.at(i) << "'s Meld: " << endl;
    suit_independent_evaluation(hands.at(i), melds);
    suit_dependent_evaluation(hands.at(i), melds, trump_suit);
    bids.at(i) = calculateBid(hands.at(i), melds);
    playerMelds.push_back(melds);

    if (melds.empty())
    {
      cout << "NONE";
    }
    for (auto it = melds.begin(); it != melds.end(); ++it)
    {
      cout << *it << " ";
    }
    cout << endl;
  }

  teamOneBid = bids.at(PLAYER_ONE) + bids.at(PLAYER_THREE);
  teamTwoBid = bids.at(PLAYER_TWO) + bids.at(PLAYER_FOUR);
  if (teamOneBid != teamTwoBid)
  {
    if (teamOneBid > teamTwoBid)
    {
      cout << player_names.at(PLAYER_ONE) << " and " << player_names.at(PLAYER_THREE) << " are awarded the contract." << endl;
      tallyTotal = calculateContract(playerMelds.at(PLAYER_ONE), playerMelds.at(PLAYER_THREE));
      contractWinner = TEAM_ONE;
    }
    else
    {
      cout << player_names.at(PLAYER_TWO) << " and " << player_names.at(PLAYER_FOUR) << " are awarded the contract." << endl;
      tallyTotal = calculateContract(playerMelds.at(PLAYER_TWO), playerMelds.at(PLAYER_FOUR));
      contractWinner = TEAM_TWO;
    }
    if (contractWinner == TEAM_ONE)
    {
      if (teamOneBid >= tallyTotal)
      {
        cout << player_names.at(PLAYER_ONE) << " and " << player_names.at(PLAYER_THREE) << " have met the contract." << endl;
      }
      else
      {
        cout << player_names.at(PLAYER_ONE) << " and " << player_names.at(PLAYER_THREE) << " have not met the contract." << endl;
      }
    }
    else
    {
      if (teamTwoBid >= tallyTotal)
      {
        cout << player_names.at(PLAYER_TWO) << " and " << player_names.at(PLAYER_FOUR) << " have met the contract." << endl;
      }
      else
      {
        cout << player_names.at(PLAYER_TWO) << " and " << player_names.at(PLAYER_FOUR) << " have not met the contract." << endl;
      }
    }
  }
  else
  {
    cout << endl;
    cout << "There is a misdeal. Redealing Cards..." << endl;
    cout << endl;
    collectAllCards();
    bids.assign(TOTAL_PLAYERS, 0);
    awardContract(bids);
  }
}
//Get the points based on the melds
int PinochleGame::getPointValue(PinochleMelds meld)
{
  return meldPoints[(int)meld];
}

ostream &operator<<(ostream &out, PinochleMelds meld)
{
  // Uses a switch statement to print out the representation of the Pinochle melds

  switch (meld)
  {
  case PinochleMelds::dix:
    out << "Dix " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::offsuitmarriage:
    out << "Off Suit Marriage " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::fortyjacks:
    out << "Forty Jacks " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::pinochle:
    out << "Pinochle " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::insuitmarriage:
    out << "In Suit Marriage " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::sixtyqueens:
    out << "Sixty Queens " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::eightykings:
    out << "Eighty Kings " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::hundredaces:
    out << "Hundred Aces " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::insuitrun:
    out << "In Suit Run " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::doublepinochle:
    out << "Double Pinochle " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::fourhundredjacks:
    out << "Four Hundred Jacks " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::sixhundredqueens:
    out << "Six Hundred Queens " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::eighthundredkings:
    out << "Eight Hundred Kings " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::thousandaces:
    out << "Thousand Aces " << PinochleGame::getPointValue(meld);
    break;
  case PinochleMelds::insuitdoublerun:
    out << "In Suit Double Run " << PinochleGame::getPointValue(meld);
    break;
  }
  return out;
}

void PinochleGame::suit_independent_evaluation(const CardSet<PinochleRank, Suit> &hand, vector<PinochleMelds> &melds)
{
  CardSet<PinochleRank, Suit> playersHand(hand);
  auto cards = playersHand.*(CardSet<PinochleRank, Suit>::getCards());
  sort(cards.begin(), cards.end(), [](Card<PinochleRank, Suit> card1, Card<PinochleRank, Suit> card2)
       { return lessThanSuit(card1, card2); });
  // Finds the melds in the hand by calling the necessary functions
  if (hasThousandAces(cards))
  {
    melds.push_back(PinochleMelds::thousandaces);
  }
  else
  {
    if (hasHundredAces(cards))
    {
      melds.push_back(PinochleMelds::hundredaces);
    }
  }
  if (hasEightHundredKings(cards))
  {
    melds.push_back(PinochleMelds::eighthundredkings);
  }
  else
  {
    if (hasEightyKings(cards))
    {
      melds.push_back(PinochleMelds::eightykings);
    }
  }
  if (hasSixHundredQueens(cards))
  {
    melds.push_back(PinochleMelds::sixhundredqueens);
  }
  else
  {
    if (hasSixtyQueens(cards))
    {
      melds.push_back(PinochleMelds::sixtyqueens);
    }
  }
  if (hasFourHundredJacks(cards))
  {
    melds.push_back(PinochleMelds::fourhundredjacks);
  }
  else
  {
    if (hasFortyJacks(cards))
    {
      melds.push_back(PinochleMelds::fortyjacks);
    }
  }
  if (hasDoublePinochle(cards))
  {
    melds.push_back(PinochleMelds::doublepinochle);
  }
  else
  {
    if (hasPinochle(cards))
    {
      melds.push_back(PinochleMelds::pinochle);
    }
  }
}

void PinochleGame::suit_dependent_evaluation(const CardSet<PinochleRank, Suit> &hand, vector<PinochleMelds> &melds, Suit target_suit)
{
  CardSet<PinochleRank, Suit> playersHand(hand);
  auto cards = playersHand.*(CardSet<PinochleRank, Suit>::getCards());
  sort(cards.begin(), cards.end(), [](Card<PinochleRank, Suit> card1, Card<PinochleRank, Suit> card2)
       { return lessThanSuit(card1, card2); });
  // Finds the melds in the hand by calling the necessary functions
  if (hasInsuitDoubleRun(cards, target_suit))
  {
    melds.push_back(PinochleMelds::insuitdoublerun);
  }
  else
  {
    if (hasInsuitRun(cards, target_suit))
    {
      melds.push_back(PinochleMelds::insuitrun);
    }
  }
  if (hasInsuitMarriage(cards, target_suit))
  {
    melds.push_back(PinochleMelds::insuitmarriage);
  }
  if (hasOffsuitMarriage(cards, target_suit))
  {
    melds.push_back(PinochleMelds::offsuitmarriage);
  }
  if (hasDix(cards, target_suit))
  {
    melds.push_back(PinochleMelds::dix);
  }
}

int binarySearch(vector<Card<PinochleRank, Suit>> &cards, Card<PinochleRank, Suit> card, int low, int high)
{
  // Implements a binary search function for cards
  if (low > high)
  {
    return -1;
  }
  else
  {
    int mid = (low + high) / 2;

    if (equal(card, cards.at(mid)))
    {
      return mid;
    }
    else if (lessThanSuit(card, cards.at(mid)))
    {
      return binarySearch(cards, card, low, mid - 1);
    }
    else
    {
      return binarySearch(cards, card, mid + 1, high);
    }
  }
}

bool hasCardOnce(vector<Card<PinochleRank, Suit>> &cards, Card<PinochleRank, Suit> card)
{
  // Checks if a certain card is in the hand once
  int index = binarySearch(cards, card, 0, cards.size() - 1);
  if (index == -1)
  {
    return false;
  }
  else
  {
    cards.erase(cards.begin() + index);
    return true;
  }
}

bool hasCardTwice(vector<Card<PinochleRank, Suit>> &cards, Card<PinochleRank, Suit> card)
{
  // Checks if a certain card is in the hand twice
  int index = binarySearch(cards, card, 0, cards.size() - 1);
  if (index == -1)
  {
    return false;
  }
  else
  {
    cards.erase(cards.begin() + index);
    index = binarySearch(cards, card, 0, cards.size() - 1);
    if (index == -1)
    {
      return false;
    }
    else
    {
      cards.erase(cards.begin() + index);
      return true;
    }
  }
}

bool hasThousandAces(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's an ace of each suit twice in the hand
  for (Suit s = Suit::clubs; s < Suit::undefined; ++s)
  {
    if (!hasCardTwice(cards, Card<PinochleRank, Suit>(PinochleRank::ace, s)))
    {
      return false;
    }
  }
  return true;
}

bool hasHundredAces(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's an ace of each suit once in the hand
  for (Suit s = Suit::clubs; s < Suit::undefined; ++s)
  {
    if (!hasCardOnce(cards, Card<PinochleRank, Suit>(PinochleRank::ace, s)))
    {
      return false;
    }
  }
  return true;
}

bool hasEightHundredKings(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's a king of each suit twice in the hand
  for (Suit s = Suit::clubs; s < Suit::undefined; ++s)
  {
    if (!hasCardTwice(cards, Card<PinochleRank, Suit>(PinochleRank::king, s)))
    {
      return false;
    }
  }
  return true;
}

bool hasEightyKings(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's a king of each suit once in the hand
  for (Suit s = Suit::clubs; s < Suit::undefined; ++s)
  {
    if (!hasCardOnce(cards, Card<PinochleRank, Suit>(PinochleRank::king, s)))
    {
      return false;
    }
  }
  return true;
}

bool hasSixHundredQueens(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's a queen of each suit twice in the hand
  for (Suit s = Suit::clubs; s < Suit::undefined; ++s)
  {
    if (!hasCardTwice(cards, Card<PinochleRank, Suit>(PinochleRank::queen, s)))
    {
      return false;
    }
  }
  return true;
}

bool hasSixtyQueens(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's a queen of each suit once in the hand
  for (Suit s = Suit::clubs; s < Suit::undefined; ++s)
  {
    if (!hasCardOnce(cards, Card<PinochleRank, Suit>(PinochleRank::queen, s)))
    {
      return false;
    }
  }
  return true;
}

bool hasFourHundredJacks(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's a jack of each suit twice in the hand
  for (Suit s = Suit::clubs; s < Suit::undefined; ++s)
  {
    if (!hasCardTwice(cards, Card<PinochleRank, Suit>(PinochleRank::jack, s)))
    {
      return false;
    }
  }
  return true;
}

bool hasFortyJacks(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's a jack of each suit once in the hand
  for (Suit s = Suit::clubs; s < Suit::undefined; ++s)
  {
    if (!hasCardOnce(cards, Card<PinochleRank, Suit>(PinochleRank::jack, s)))
    {
      return false;
    }
  }
  return true;
}

bool hasDoublePinochle(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's a jack of diamonds and a queen of spades twice in the hand
  Card<PinochleRank, Suit> jDiamond = Card<PinochleRank, Suit>(PinochleRank::jack, Suit::diamonds);
  Card<PinochleRank, Suit> qSpade = Card<PinochleRank, Suit>(PinochleRank::queen, Suit::spades);
  return (hasCardTwice(cards, jDiamond) && hasCardTwice(cards, qSpade));
}

bool hasPinochle(vector<Card<PinochleRank, Suit>> cards)
{
  // Checks if there's a jack of diamonds and a queen of spades once in the hand
  Card<PinochleRank, Suit> jDiamond = Card<PinochleRank, Suit>(PinochleRank::jack, Suit::diamonds);
  Card<PinochleRank, Suit> qSpade = Card<PinochleRank, Suit>(PinochleRank::queen, Suit::spades);
  return (hasCardOnce(cards, jDiamond) && hasCardOnce(cards, qSpade));
}

bool hasInsuitDoubleRun(vector<Card<PinochleRank, Suit>> cards, Suit &suit)
{
  Card<PinochleRank, Suit> ace = Card<PinochleRank, Suit>(PinochleRank::ace, suit);
  Card<PinochleRank, Suit> ten = Card<PinochleRank, Suit>(PinochleRank::ten, suit);
  Card<PinochleRank, Suit> king = Card<PinochleRank, Suit>(PinochleRank::king, suit);
  Card<PinochleRank, Suit> queen = Card<PinochleRank, Suit>(PinochleRank::queen, suit);
  Card<PinochleRank, Suit> jack = Card<PinochleRank, Suit>(PinochleRank::jack, suit);
  return (hasCardTwice(cards, ace) && hasCardTwice(cards, ten) && hasCardTwice(cards, king) && hasCardTwice(cards, queen) && hasCardTwice(cards, jack));
};

bool hasInsuitRun(vector<Card<PinochleRank, Suit>> cards, Suit &suit)
{
  Card<PinochleRank, Suit> ace = Card<PinochleRank, Suit>(PinochleRank::ace, suit);
  Card<PinochleRank, Suit> ten = Card<PinochleRank, Suit>(PinochleRank::ten, suit);
  Card<PinochleRank, Suit> king = Card<PinochleRank, Suit>(PinochleRank::king, suit);
  Card<PinochleRank, Suit> queen = Card<PinochleRank, Suit>(PinochleRank::queen, suit);
  Card<PinochleRank, Suit> jack = Card<PinochleRank, Suit>(PinochleRank::jack, suit);
  return (hasCardOnce(cards, ace) && hasCardOnce(cards, ten) && hasCardOnce(cards, king) && hasCardOnce(cards, queen) && hasCardOnce(cards, jack));
};
bool hasInsuitMarriage(vector<Card<PinochleRank, Suit>> cards, Suit &suit)
{
  Card<PinochleRank, Suit> king = Card<PinochleRank, Suit>(PinochleRank::king, suit);
  Card<PinochleRank, Suit> queen = Card<PinochleRank, Suit>(PinochleRank::queen, suit);
  return (hasCardOnce(cards, king) && hasCardOnce(cards, queen));
};
bool hasOffsuitMarriage(vector<Card<PinochleRank, Suit>> cards, Suit &suit)
{
  for (Suit s = Suit::clubs; s != Suit::undefined; ++s)
  {
    if (s != suit)
    {
      Card<PinochleRank, Suit> king = Card<PinochleRank, Suit>(PinochleRank::king, s);
      Card<PinochleRank, Suit> queen = Card<PinochleRank, Suit>(PinochleRank::queen, s);
      return (hasCardOnce(cards, king) && hasCardOnce(cards, queen));
    }
  }
  return false;
};
bool hasDix(vector<Card<PinochleRank, Suit>> cards, Suit &suit)
{
  Card<PinochleRank, Suit> nine = Card<PinochleRank, Suit>(PinochleRank::nine, suit);
  return hasCardOnce(cards, nine);
};
//Find the total amount of the bid
int calculateBid(CardSet<PinochleRank, Suit> &hand, vector<PinochleMelds> &melds)
{
  int total_score = 0;
  auto cards = hand.*(CardSet<PinochleRank, Suit>::getCards());
  for (auto it = cards.begin(); it != cards.end(); ++it)
  {
    switch ((*it).rank)
    {
    case PinochleRank::nine:
      total_score += 0;
      break;
    case PinochleRank::jack:
      total_score += 2;
      break;
    case PinochleRank::queen:
      total_score += 3;
      break;
    case PinochleRank::king:
      total_score += 4;
      break;
    case PinochleRank::ten:
      total_score += 10;
      break;
    case PinochleRank::ace:
      total_score += 11;
      break;
    case PinochleRank::undefined:
      break;
    }
  }
  for (auto it = melds.begin(); it != melds.end(); ++it)
  {
    total_score += PinochleGame::getPointValue(*it);
  }
 
  return total_score;
}
//Calculates the total for the contract
int calculateContract(vector<PinochleMelds> &firstMelds, vector<PinochleMelds> &secondMelds)
{
  unsigned int contractTotal = 0;
  for (auto it = firstMelds.begin(); it != firstMelds.end(); ++it)
  {
    contractTotal += PinochleGame::getPointValue(*it);
  }
  for (auto it = secondMelds.begin(); it != secondMelds.end(); ++it)
  {
    contractTotal += PinochleGame::getPointValue(*it);
  }
  return contractTotal;
}
//Find the highest rank in a hand for a suit
int getHighestRank(vector<Card<PinochleRank, Suit>> &hand, Suit suit)
{

  int highestCard = 0;
  for (size_t i = 1; i < hand.size(); ++i)
  {
    if ((hand.at(highestCard).suit != suit) && (hand.at(i).suit == suit))
    {
      highestCard = i;
    }
    else if ((hand.at(highestCard).suit == suit) && (hand.at(i).suit == suit))
    {
      if (hand.at(highestCard).rank < hand.at(i).rank)
      {
        highestCard = i;
      }
    }
    else if ((hand.at(highestCard).suit != suit) && (hand.at(i).suit != suit))
    {
      if (hand.at(highestCard).rank < hand.at(i).rank)
      {
        highestCard = i;
      }
    }
  }
  return highestCard;
}
//Find the lowest rank in a hand that isn't the given suit
int getLowestRankExclude(vector<Card<PinochleRank, Suit>> &hand, Suit suit)
{
  int lowestCard = 0;
  for (size_t i = 1; i < hand.size(); ++i)
  {
    if ((hand.at(lowestCard).suit == suit) && (hand.at(i).suit != suit))
    {
      lowestCard = i;
    }
    else if ((hand.at(lowestCard).suit == suit) && (hand.at(i).suit == suit))
    {
      if (hand.at(lowestCard).rank > hand.at(i).rank)
      {
        lowestCard = i;
      }
    }
    else if ((hand.at(lowestCard).suit != suit) && (hand.at(i).suit != suit))
    {
      if (hand.at(lowestCard).rank > hand.at(i).rank)
      {
        lowestCard = i;
      }
    }
  }
  return lowestCard;
}
//Find the lowest rank in a hand that is the given suit
int getLowestRankInclude(vector<Card<PinochleRank, Suit>> &hand, Suit suit)
{
  int lowestCard = 0;
  for (size_t i = 1; i < hand.size(); ++i)
  {
    if ((hand.at(lowestCard).suit != suit) && (hand.at(i).suit == suit))
    {
      lowestCard = i;
    }
    else if ((hand.at(lowestCard).suit == suit) && (hand.at(i).suit == suit))
    {
      if (hand.at(lowestCard).rank > hand.at(i).rank)
      {
        lowestCard = i;
      }
    }
    else if ((hand.at(lowestCard).suit != suit) && (hand.at(i).suit != suit))
    {
      if (hand.at(lowestCard).rank > hand.at(i).rank)
      {
        lowestCard = i;
      }
    }
  }
  return lowestCard;
}
//Finds the total points of the trick cardset
int calculateTrick(vector<Card<PinochleRank, Suit>> &trickHand)
{
  unsigned int trickTotal = 0;
  for (auto it = trickHand.begin(); it != trickHand.end(); ++it)
  {
    switch ((*it).rank)
    {
    case PinochleRank::nine:
      trickTotal += 0;
      break;
    case PinochleRank::jack:
      trickTotal += 2;
      break;
    case PinochleRank::queen:
      trickTotal += 3;
      break;
    case PinochleRank::king:
      trickTotal += 4;
      break;
    case PinochleRank::ten:
      trickTotal += 10;
      break;
    case PinochleRank::ace:
      trickTotal += 11;
      break;
    case PinochleRank::undefined:
      break;
    }
  }
  return trickTotal;
}