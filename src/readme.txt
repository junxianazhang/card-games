Junxian Zhang - junxian@wustl.edu - Lab 3

Errors/Warnings:
In file included from PinochleGame.h:87,
                 from lab3.cpp:6:
PinochleGame.cpp: In member function \u2018virtual int PinochleGame::play()\u2019:
PinochleGame.cpp:75:13: error: \u2018gameScores\u2019 was not declared in this scope
     while ((gameScores.at(TEAM_ONE) < 1500) && (gameScores.at(TEAM_TWO) < 1500))
             ^~~~~~~~~~
PinochleGame.cpp:75:13: note: suggested alternative: \u2018scores\u2019
     while ((gameScores.at(TEAM_ONE) < 1500) && (gameScores.at(TEAM_TWO) < 1500))
             ^~~~~~~~~~
             scores
PinochleGame.cpp: In member function \u2018void PinochleGame::determineNextCard(const CardSet<PinochleRank, Suit>&, Suit)\u2019:
PinochleGame.cpp:129:51: error: binding reference of type \u2018std::vector<Card<PinochleRank, Suit>, std::allocator<Card<PinochleRank, Suit> > >&\u2019 to \u2018const std::vector<Card<PinochleRank, Suit>, std::allocator<Card<PinochleRank, Suit> > >\u2019 discards qualifiers
   vector<Card<PinochleRank, Suit>> &hand = handset.*(CardSet<PinochleRank, Suit>::getCards());
In file included from HoldEmGame.h:79,
                 from lab3.cpp:5:
HoldEmGame.cpp: In member function \u2018void HoldEmGame::evaluateRiver()\u2019:
HoldEmGame.cpp:201:10: error: expected \u2018;\u2019 before numeric constant
         3 4 5 6 7 * /
          ^~
          ;
HoldEmGame.cpp:202:51: warning: statement has no effect [-Wunused-value]
             CardSet<HoldEmRank, Suit> table(board);
                                                   ^
HoldEmGame.cpp:203:17: error: \u2018table\u2019 was not declared in this scope
         while (!table.is_empty())
                 ^~~~~
HoldEmGame.cpp:203:17: note: suggested alternative: \u2018tanl\u2019
         while (!table.is_empty())
                 ^~~~~
                 tanl
HoldEmGame.cpp: In function \u2018HoldEmHandRank getBestComboForSix(std::vector<Card<HoldEmRank, Suit>, std::allocator<Card<HoldEmRank, Suit> > >)\u2019:
HoldEmGame.cpp:773:25: error: \u2018holdem_hand_evaluation\u2019 was not declared in this scope
     handRanks.push_back(holdem_hand_evaluation(cardSet1));
                         ^~~~~~~~~~~~~~~~~~~~~~
HoldEmGame.cpp: In function \u2018HoldEmHandRank getBestComboForSeven(std::vector<Card<HoldEmRank, Suit>, std::allocator<Card<HoldEmRank, Suit> > >)\u2019:
HoldEmGame.cpp:826:1: warning: no return statement in function returning non-void [-Wreturn-type]
 }
 ^
HoldEmGame.cpp:801:68: warning: unused parameter \u2018allCards\u2019 [-Wunused-parameter]
 HoldEmHandRank getBestComboForSeven(vector<Card<HoldEmRank, Suit>> allCards)
                                     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~
make: *** [lab3] Error 1

In file included from lab3.cpp:5:
HoldEmGame.h: In function \u2018bool operator<(const HoldEmGame::PlayerStatus&, const HoldEmGame::PlayerStatus&)\u2019:
HoldEmGame.h:86:34: error: \u2018struct HoldEmGame::PlayerStatus\u2019 is private within this context
 bool operator<(const HoldEmGame::PlayerStatus &p1, const HoldEmGame::PlayerStatus &p2);
                                  ^~~~~~~~~~~~
In file included from lab3.cpp:5:
HoldEmGame.h:35:12: note: declared private here
     struct PlayerStatus
            ^~~~~~~~~~~~
In file included from HoldEmGame.h:102,
                 from lab3.cpp:5:
HoldEmGame.cpp: In member function \u2018virtual int HoldEmGame::play()\u2019:
HoldEmGame.cpp:103:16: error: \u2018players\u2019 was not declared in this scope
         while (players.size() != 1)
                ^~~~~~~
HoldEmGame.cpp:103:16: note: suggested alternative: \u2018play\u2019
         while (players.size() != 1)
                ^~~~~~~
                play
HoldEmGame.cpp:125:26: error: no matching function for call to \u2018HoldEmGame::evaluateFlop()\u2019
             evaluateFlop();
                          ^
In file included from lab3.cpp:5:
HoldEmGame.h:49:10: note: candidate: \u2018void HoldEmGame::evaluateFlop(std::vector<HoldEmGame::PlayerStatus>&)\u2019
     void evaluateFlop(vector<PlayerStatus> &players);
          ^~~~~~~~~~~~
HoldEmGame.h:49:10: note:   candidate expects 1 argument, 0 provided
In file included from HoldEmGame.h:102,
                 from lab3.cpp:5:
HoldEmGame.cpp:132:26: error: no matching function for call to \u2018HoldEmGame::evaluateTurn()\u2019
             evaluateTurn();
                          ^
In file included from lab3.cpp:5:
HoldEmGame.h:50:10: note: candidate: \u2018void HoldEmGame::evaluateTurn(std::vector<HoldEmGame::PlayerStatus>&)\u2019
     void evaluateTurn(vector<PlayerStatus> &players);
          ^~~~~~~~~~~~
HoldEmGame.h:50:10: note:   candidate expects 1 argument, 0 provided
In file included from HoldEmGame.h:102,
                 from lab3.cpp:5:
HoldEmGame.cpp:139:27: error: no matching function for call to \u2018HoldEmGame::evaluateRiver()\u2019
             evaluateRiver();
                           ^
In file included from lab3.cpp:5:
HoldEmGame.h:51:10: note: candidate: \u2018void HoldEmGame::evaluateRiver(std::vector<HoldEmGame::PlayerStatus>&)\u2019
     void evaluateRiver(vector<PlayerStatus> &players);
          ^~~~~~~~~~~~~
HoldEmGame.h:51:10: note:   candidate expects 1 argument, 0 provided
In file included from HoldEmGame.h:102,
                 from lab3.cpp:5:
HoldEmGame.cpp:148:17: error: \u2018players\u2019 was not declared in this scope
         cout << players.at(0).playerName << " wins the game with a final chip count of " << scores.at(players.pos) << endl;
                 ^~~~~~~
HoldEmGame.cpp:148:17: note: suggested alternative: \u2018play\u2019
         cout << players.at(0).playerName << " wins the game with a final chip count of " << scores.at(players.pos) << endl;
                 ^~~~~~~
                 play
HoldEmGame.cpp: In member function \u2018bool HoldEmGame::hasRaised(HoldEmGame::PlayerStatus&)\u2019:
HoldEmGame.cpp:186:30: warning: comparison of integer expressions of different signedness: \u2018int\u2019 and \u2018unsigned int\u2019 [-Wsign-compare]
     return player.currentBet < largestBet;
            ~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~
HoldEmGame.cpp: In member function \u2018bool HoldEmGame::endBettingRound(std::vector<HoldEmGame::PlayerStatus>&)\u2019:
HoldEmGame.cpp:203:17: error: \u2018currentBet\u2019 was not declared in this scope
             if (currentBet != largestBet)
                 ^~~~~~~~~~
HoldEmGame.cpp:203:17: note: suggested alternative: \u2018largestBet\u2019
             if (currentBet != largestBet)
                 ^~~~~~~~~~
                 largestBet
HoldEmGame.cpp: In member function \u2018void HoldEmGame::determineWinner(std::vector<HoldEmGame::PlayerStatus>&)\u2019:
HoldEmGame.cpp:219:21: error: \u2018class std::vector<HoldEmGame::PlayerStatus>\u2019 has no member named \u2018folded\u2019
         if (players.folded == false)
                     ^~~~~~
HoldEmGame.cpp: In member function \u2018void HoldEmGame::turnBetStrat(HoldEmGame::PlayerStatus&)\u2019:
HoldEmGame.cpp:346:122: error: \u2018cards\u2019 was not declared in this scope
 ) || (player.handRank == HoldEmHandRank::twopair) || isFourSameSuit(cards) || isFourConsecutive(cards))
                                                                     ^~~~~

HoldEmGame.cpp:346:122: note: suggested alternative: \u2018hands\u2019
 ) || (player.handRank == HoldEmHandRank::twopair) || isFourSameSuit(cards) || isFourConsecutive(cards))
                                                                     ^~~~~
                                                                     hands
HoldEmGame.cpp: In member function \u2018void HoldEmGame::fillPlayerStatus(std::vector<HoldEmGame::PlayerStatus>&)\u2019:
HoldEmGame.cpp:439:17: error: \u2018class std::vector<HoldEmGame::PlayerStatus>\u2019 has no member named \u2018hand\u2019; did you mean \u2018end\u2019?
         players.hand = hands.at(i);
                 ^~~~
                 end
HoldEmGame.cpp: In function \u2018bool operator<(const HoldEmGame::PlayerStatus&, const HoldEmGame::PlayerStatus&)\u2019:
HoldEmGame.cpp:824:34: error: \u2018struct HoldEmGame::PlayerStatus\u2019 is private within this context
 bool operator<(const HoldEmGame::PlayerStatus &p1, const HoldEmGame::PlayerStatus &p2)
                                  ^~~~~~~~~~~~
In file included from lab3.cpp:5:
HoldEmGame.h:35:12: note: declared private here
     struct PlayerStatus
            ^~~~~~~~~~~~
make: *** [lab3] Error 1


Design:
I used a struct to represent a player in HoldEm so that I can store all sorts of values that are related to the player in a single vector.

Trials:

Trial 1:

Command: ./lab3

Output: Correct Usage: ./lab3 [GAME_NAME: Pinochle | HoldEm] [PLAYER_NAMES: 4 players for Pinochle and 2-9 players for HoldEm]

Trial 2:

Command: ./lab3 randomGame

Output: Correct Usage: ./lab3 [GAME_NAME: Pinochle | HoldEm] [PLAYER_NAMES: 4 players for Pinochle and 2-9 players for HoldEm]

Trial 3:

Command: ./lab3 Pinochle
Output: There are an incorrect amoung of players, please input (4) player names.

Trial 4:

Command: ./lab3 HoldEm

Output: There are an incorrect amoung of players, please input (2-9) player names.

Trial 5:

Command: ./lab3 Pinochle Jim

Output: There are an incorrect amoung of players, please input (4) player names.

Trial 6:

Command: ./lab3 HoldEm Jim

Output: There are an incorrect amoung of players, please input (2-9) player names.

Trial 7:

Command: ./lab3 Pinochle Jim Bob Peter Ana

Output: 
Jim's Hand: 
JH 10D AH KD AD JS 
9D AC AD JD KC QH 

Bob's Hand: 
10H KH 9C KS AC 10C 
JC JC QS AH JD 9H 

Peter's Hand: 
10C QD 10D AS 9D 9C 
9H KD 9S KH QS JS 

Ana*'s Hand: 
QD JH 10H QH 10S QC 
AS 9S 10S KS QC KC 

Jim's Meld: 
NONE
Bob's Meld: 
Pinochle 40 Dix 10 
Peter's Meld: 
Off Suit Marriage 20 Dix 10 
Ana's Meld: 
In Suit Marriage 40 
Bob and Ana are awarded the contract.
Bob and Ana have met the contract.
...
...
Too much to copy...

Trial 8:

Command: ./lab3 HoldEm Jim Bob

Output:
Too much was printed...
...
...
Bob raises the bet by 2.
Jim calls.
Bob raises the bet by 2.
Jim calls.
Bob raises the bet by 2.
Jim calls.
Bob raises the bet by 2.

BOARD (flop):
KC 3S 5H 

[Bob]
Cards: AD AH 5H 3S KC 
Hand Rank: One Pair

[Jim]
Cards: 9C 6C 5H 3S KC 
Hand Rank: High Card

BOARD (turn):
KC 3S 5H 6S 

[Bob]
Cards: AH AD 5H 3S KC 
Hand Rank: One Pair

[Jim]
Cards: 6C 6S 5H 3S KC 
Hand Rank: One Pair

BOARD (river):
KC 3S 5H 6S QS 


[Bob]
Cards: AH AD 5H 3S KC 
Hand Rank: One Pair

[Jim]
Cards: 6C 6S 5H 3S KC 
Hand Rank: One Pair

Final Board:
KC 3S 5H 6S QS 

Jim have busted out. Place No.2

Bob wins the game with a final chip count of 120
Would you like to end the game? Type 'yes' to end the game.
