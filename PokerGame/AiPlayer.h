#pragma once

#include "PlayerI.h"

class AiPlayer : public PlayerI {
public:
	AiPlayer(int index);
	int placeBet(int previousBet, bool& bettingIsOpen) override;
	void drawCards(std::vector<CardClass::Card> deck) override;
	void lookAtHand() override;

private:
	int getHandStrength();
};