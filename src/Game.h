// Junxian Zhang - junxian@wustl.edu 

// Game.cpp: This file contains the definitions of the methods, constructor, and member variable for the Game class.

#pragma once

#include <iostream>
#include <vector>
#include <string>

enum RETURN_VALUE {PROGRAM_SUCCESSFUL,SUCCESSFULLY_ENDED_GAME, SHARED_POINTER_FAILED, BAD_COMMAND, INCORRECT_NUMBER_OF_PLAYERS};

using namespace std;
class Game {
public:
    Game(int argc, const char * argv[]);
    virtual int play() = 0;
    bool endGame();
    
protected:
    vector<string> player_names;
    size_t dealer = 0;
    vector<unsigned int> scores;
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE /* test whether guard symbol is defined */
#include "Game.cpp"
#endif