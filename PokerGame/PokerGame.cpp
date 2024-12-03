#include "PokerGame.h"
#include <iostream>

void PokerGame::createDeck()
{
    const std::vector<std::string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    const std::vector<std::string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };
    for (const auto& rank : ranks) {
        for (const auto& suit : suits) {
            this->deck.push_back({ rank, suit });
        }
    }
}

PokerGame::PokerGame()
{
    initGame();

    for (const auto& player : players) {
        std::cout << player->name << std::endl;
    }

    createDeck();
    shuffleDeck();
}

void PokerGame::shuffleDeck()
{
    // Seed the rand function so its different everytime
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (size_t i = this->deck.size() - 1; i > 0; --i) {
        int j = std::rand() % (i + 1); // Generate a random index in range [0, i]
        std::swap(this->deck[i], this->deck[j]);// Swap the current element with the randomly chosen element
    }
}

void PokerGame::initGame()
{
    // Get umber of human players (0 - 7)
    int numHuman;
    do{
        std::cout << "Enter valid number of human players: ";
        numHuman = getValidUserInt();
    } while ( numHuman > MAX_NUM_PLAYERS );

    // Get Number of AI players (2 <= max players <= 7)
    int numAi;
    do {
        std::cout << "Enter valid number of AI players: ";
        numAi = getValidUserInt();
    } while ( (numHuman + numAi) > MAX_NUM_PLAYERS || (numHuman + numAi) < MIN_NUM_PLAYERS );
    
    // Create all player objects
    std::string name;
    for (int i = 0; i < numHuman; i++) { 
        std::cout << "Enter player " << i << "s name: ";
        std::cin >> name;
        players.push_back(std::make_shared<HumanPlayer>(name));
    }

    for (int i = 0; i < numAi; i++) {
        players.push_back(std::make_shared<AiPlayer>(i));
    }


}

void PokerGame::playPoker()
{
    // Deal 5 cards to each player
    std::cout << "Dealing 5 cards to each player." << std::endl;
    for (auto& player : players) {
        for (int i = 0; i < 5; ++i) {
            player->hand.push_back(this->deck.back());
            this->deck.pop_back();
        }
    }

    // First round of betting
    for (auto& player : players) {
        player->lookAtHand();
    }



}

int PokerGame::getValidUserInt()
{
    int value;
    while (true) {
        std::cin >> value;

        // Check if input is valid
        if (std::cin.fail() || value < 0) {
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a valid integer.\n";
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard extra input
            return value;
        }
    }
}
