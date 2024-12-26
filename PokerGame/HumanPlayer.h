#pragma once

#include "PlayerI.h"

class HumanPlayer : public PlayerI 
{
public:
	HumanPlayer(std::string name);
	int placeBet(int maxBet, bool& bettingIsOpen) override;
	void drawCards(std::vector<CardClass::Card> deck) override;
	void lookAtHand() override;
	void lookAtHandIndices();

};
