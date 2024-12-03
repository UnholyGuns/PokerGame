#include "AiPlayer.h"

AiPlayer::AiPlayer(int index)
{
	this->name = "Ai Player " + std::to_string(index);
}

void AiPlayer::makeDecision()
{
}

void AiPlayer::lookAtHand()
{
    std::cout << this->name << "'s hand: " << std::endl;
    for (const auto& card : hand) {
        std::cout << card.rank << " of " << card.suit << std::endl;;
    }
}
