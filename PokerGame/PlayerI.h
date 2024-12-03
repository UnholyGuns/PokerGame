#pragma once

#include <vector>
#include "Card.h"
#include <string>

class PlayerI {
public:
	std::string name;
	virtual void makeDecision() = 0;
	virtual void lookAtHand() = 0;
	std::vector<CardClass::Card> hand;

};
