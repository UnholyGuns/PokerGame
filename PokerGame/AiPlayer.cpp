#include "AiPlayer.h"

AiPlayer::AiPlayer(int index)
{
	this->name = "Ai Player " + std::to_string(index);
    moneyBet = 0;
    isDone = false;
    isFolded = false;
}

int AiPlayer::placeBet(int previousBet, bool& bettingIsOpen)
{
    std::cout << "The opponent studies their hand..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(6000));

    // AI opens the round if the previous bet was 0
    if (!bettingIsOpen) 
    {
        // Randomly check or bet
        int randomChoice = rand() % 2;
        if (randomChoice == 0) 
        {
            std::cout << this->name << " checks." << std::endl;
            return 0;
        }
        else 
        {
            // Place a random starting bet between 5 and 10
            int randomBet = 5 + 1 + (rand() % 6);         
            moneyBet += randomBet + previousBet;
            bettingIsOpen = true;
            return randomBet;
        }
    }

    int handStrength = getHandStrength();

    // Decide the bet based on hand strength
    int bet = 0;
    int randomChoice = rand() % 5;

    if (handStrength > 60)
    {
        // Randomly choose between a aggressive raise and a call
        if (0 <= randomChoice && randomChoice <= 2)// Call the previous bet
        {
            bet = previousBet;
        }
        else// Aggressive bet 
        {
            bet = previousBet * 2;
        }
    }
    else if (handStrength > 30)// Moderate Bet
    {
        // Randomly choose between a moderate raise and a call
        if (0 <= randomChoice && randomChoice <= 3)// Call the previous bet
        {
            bet = previousBet;
        }
        else// Moderate bet 
        {
            bet = previousBet * 1.3;          
        }
    }
    else 
    {
        bet = 0; 
    }

    moneyBet += bet;
    return bet;
}

void AiPlayer::drawCards(std::vector<CardClass::Card> deck)
{
    // Ensure the deck has enough cards to draw from
    if (deck.empty()) 
    {
        std::cout << "Deck is empty! Cannot replace cards." << std::endl;
        return;
    }

    // Delay so it feels more like a game
    std::cout << "The opponent studies their hand..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    // Randomly choose number of cards to discard
    int cardsToReplace = std::rand() % 6;
    if (cardsToReplace == 0) 
    {
        std::cout << this->name << " decides to keep all their cards." << std::endl;
        return;
    }

    // Select random indices from the AI's hand
    std::vector<int> indices(hand.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::random_shuffle(indices.begin(), indices.end()); 

    // Replace the selected cards
    for (int i = 0; i < cardsToReplace; i++) 
    {
        int index = indices[i];

        if (!deck.empty()) 
        {
            hand[index] = deck.back(); 
            deck.pop_back();          
        }
        else 
        {
            std::cout << "Not enough cards in the deck to replace all cards!" << std::endl;
            break;
        }
    }

    std::cout << this->name << " replaced " << cardsToReplace << " cards." << std::endl;
}

void AiPlayer::lookAtHand()
{
    std::cout << this->name << "'s hand: " << std::endl;

    for (const auto& card : hand) 
    {
        std::cout << card.rank << " of " << card.suit << ", ";
    }

    std::cout << std::endl << std::endl;
}

int AiPlayer::getHandStrength()
{
    int strength = 0;
    for (const auto& card : hand)
    {
        if (card.rank == "A" || card.rank == "K" || card.rank == "Q" || card.rank == "J") {
            strength += 20;
        }
        else 
        {
            strength += std::stoi(card.rank);
        }
    }

    return std::min(strength, 100); 
}
