// Game.cpp: This file contains the definitions of the methods, constructor, and member variable for the Game class.

#include "Game.h"

Game::Game(int argc, const char *argv[])
{
    // Stores all the player names that the user inputs in a vector of strings
    for (int i = 2; i < argc; ++i)
    {
        player_names.push_back(argv[i]);
        scores.push_back(0);
    }
    dealer = player_names.size() - 1;
}

bool Game::endGame()
{
    // Checks to see if the user wants to the end and returns a boolean based on the answer
    string answer;
    cout << "Would you like to end the game? Type 'yes' to end the game." << endl;
    cin >> answer;
    if (answer == "yes")
    {
        return true;
    }
    else
    {
        dealer = (dealer + 1) % player_names.size();
        return false;
    }
}