#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <numeric>   
#include <algorithm>
#include <sstream>
#include <thread> 
#include <chrono>

#include "Card.h"

class PlayerI 
{
public:
	std::string name;
	int moneyBetInRound;
	int moneyBetTotal;
	std::vector<CardClass::Card> hand;
	bool isDone;
	bool isFolded;

	virtual void lookAtHand() = 0;
	virtual int placeBet(int previousBet, bool& bettingIsOpen) = 0;
	virtual void drawCards(std::vector<CardClass::Card> deck) = 0;
	
};
