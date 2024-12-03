#pragma once

#include "PlayerI.h"
#include <string>

class AiPlayer : public PlayerI {
public:

	AiPlayer(int index);
	void makeDecision() override;
	void lookAtHand() override;
};