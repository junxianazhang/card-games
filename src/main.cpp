// main.cpp: This file contains the main method that executes the program and starts it all.

#include "HoldEmGame.h"
#include "PinochleGame.h"
#include <memory>
#include <cstring>

enum inputArgs {EXECUTABLE_FILE_NAME, GAME_NAME};

enum argsSeperator {FILE_NAME_ARG = 1, TARGET_PINOCHLE_ARGS = 6, LOWER_HOLDEM_BOUND = 4, UPPER_HOLDEM_BOUND = 11};

shared_ptr<Game> create(int argc, const char *argv[]){
	shared_ptr<Game> game_ptr;
	if(strcmp(argv[GAME_NAME], "Pinochle") == 0){
		game_ptr = make_shared<PinochleGame>(argc, argv);
	} else if (strcmp(argv[GAME_NAME], "HoldEm") == 0){
		game_ptr = make_shared<HoldEmGame>(argc, argv);
	}
	return game_ptr;
}

int main(int argc, const char *argv[]) {
	if(argc == FILE_NAME_ARG) {
		cout << "Correct Usage: " << argv[EXECUTABLE_FILE_NAME] << " [GAME_NAME: Pinochle | HoldEm] [PLAYER_NAMES: 4 players for Pinochle and 2-9 players for HoldEm]" << endl;
		return BAD_COMMAND;
	}

	if(strcmp(argv[GAME_NAME], "Pinochle") == 0){
		if(argc == TARGET_PINOCHLE_ARGS){
			shared_ptr<Game> game_ptr = create(argc, argv);
			if(game_ptr){
				return (*game_ptr).play();
			} else {
				cout << "Failed to create a game (Shared Pointer Does Not Point To Anything)." << endl;
				return SHARED_POINTER_FAILED;
		}
		} else {
			cout << "There are an incorrect amoung of players, please input (4) player names." << endl;
			return INCORRECT_NUMBER_OF_PLAYERS;
		}
	} else if (strcmp(argv[GAME_NAME], "HoldEm") == 0) {
		if((argc >= LOWER_HOLDEM_BOUND) && (argc <= UPPER_HOLDEM_BOUND)){
			shared_ptr<Game> game_ptr = create(argc, argv);
			if(game_ptr){
				return (*game_ptr).play();
			} else {
				cout << "Failed to create a game (Shared Pointer Does Not Point To Anything)." << endl;
				return SHARED_POINTER_FAILED;
		}
		} else {
			cout << "There are an incorrect amoung of players, please input (2-9) player names." << endl;
			return INCORRECT_NUMBER_OF_PLAYERS;
		}
	} else {
		cout << "Correct Usage: " << argv[EXECUTABLE_FILE_NAME] << " [GAME_NAME: Pinochle | HoldEm] [PLAYER_NAMES: 4 players for Pinochle and 2-9 players for HoldEm]" << endl;
		return BAD_COMMAND;
	}
}