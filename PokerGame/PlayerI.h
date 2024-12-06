#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <numeric>   
#include <algorithm>
#include <sstream>
#include "Card.h"
#include <thread> 
#include <chrono>

class PlayerI {
public:
	std::string name;
	int moneyBet;
	std::vector<CardClass::Card> hand;

	bool isDone;
	bool isFolded;

	virtual void lookAtHand() = 0;
	virtual int placeBet(int previousBet, bool& bettingIsOpen) = 0;
	virtual void drawCards(std::vector<CardClass::Card> deck) = 0;
	
};
