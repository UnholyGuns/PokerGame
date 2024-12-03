#pragma once

#include "PlayerI.h"

class HumanPlayer : public PlayerI {
public:
	HumanPlayer(std::string name);
	void makeDecision() override;
	void lookAtHand() override;

};
