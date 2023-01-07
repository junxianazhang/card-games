// Junxian Zhang - junxian@wustl.edu 

// Suit.h: This file contains the enum class of Suit and the declarations for its operators.

#pragma once

#include <iostream>
using namespace std;

enum class Suit { clubs, diamonds, hearts, spades, undefined };

ostream& operator<< (ostream& out, const Suit& suit);

Suit& operator++ (Suit&);
