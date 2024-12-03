#include "HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(std::string name)
{
	this->name = name;
}

void HumanPlayer::makeDecision()
{
}

void HumanPlayer::lookAtHand()
{
    std::cout << this->name << "'s hand: " << std::endl;
    for (const auto& card : hand) {
        std::cout << card.rank << " of " << card.suit << std::endl;;
    }
}
