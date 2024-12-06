#include "PokerGame.h"

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

PokerGame::PokerGame(){
    moneyPot = 0;
    previousBet = 0;
    ante = 0;
    bettingIsOpen = false;
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
    do
    {
        std::cout << "Enter valid number of human players: ";
        numHuman = getValidUserInt();
    } while ( numHuman > MAX_NUM_PLAYERS );

    // Get Number of AI players (2 <= max players <= 7)
    int numAi;
    do 
    {
        std::cout << "Enter valid number of AI players: ";
        numAi = getValidUserInt();
    } while ( (numHuman + numAi) > MAX_NUM_PLAYERS || (numHuman + numAi) < MIN_NUM_PLAYERS );
    
    // Create all player objects
    std::string name;

    for (int i = 0; i < numHuman; i++) 
    { 
        std::cout << "Enter player " << i << "s name: ";
        std::cin >> name;
        players.push_back(std::make_shared<HumanPlayer>(name));
    }

    for (int i = 0; i < numAi; i++) 
    {
        players.push_back(std::make_shared<AiPlayer>(i+1));
    }

    // New player to start each time
    std::random_shuffle(players.begin(), players.end());

    std::cout << "Whats the ante?: ";
    ante = getValidUserInt();
    moneyPot += (numAi + numHuman) * ante;
    std::cout << "The pot contains $" << moneyPot << std::endl;
}

void PokerGame::playPoker()
{
    // Deal 5 cards to each player
    dealCards();

    // First round of betting
    previousBet = ante;
    bettingRound();

    // See if all but one person folded
    if (checkFoldedWinner()) {
        return;
    }

    // Draw Round
    drawRound();

    // Fresh betting round, per standard rules
    previousBet = ante;
    bettingRound();

    showDown();
    showDown();
}

void PokerGame::dealCards()
{
    std::cout << std::endl << "Dealing 5 cards to each player." << std::endl << std::endl;

    for (auto& player : players)
    {
        for (int i = 0; i < 5; ++i)
        {
            player->hand.push_back(this->deck.back());
            this->deck.pop_back();
        }
    }
}

void PokerGame::showDown()
{
    std::cout << "Its showtime." << std::endl;
}

bool PokerGame::checkFoldedWinner()
{
    PlayerI* remainingPlayer = nullptr;

    for (auto& player : players)
    {
        if (!player->isFolded)
        {
            if (remainingPlayer != nullptr)
            {
                return false;
            }
            remainingPlayer = player.get(); 
        }
    }

    if (remainingPlayer != nullptr)
    {
        std::cout << remainingPlayer->name << " wins the hand! Congratulations!" << std::endl;
        return true;
    }

    return false;
}


void PokerGame::bettingRound()
{
    std::cout << "Begin the betting round!" << std::endl << std::endl;

    // First reset the status of all players
    for (auto& player : players)
    {
        player->isDone = false;
    }

    bool bettingDone = false;

    while (!bettingDone) 
    {
        bettingDone = true;

        for (auto& player : players) 
        {
            if (!player->isDone && !player->isFolded) 
            {
                std::cout << player->name << "'s turn to place a bet!" << std::endl;
                int bet = player->placeBet(previousBet, bettingIsOpen);

                if (bet > previousBet)
                {
                    std::cout << player->name << " raises with $" << bet << std::endl;
                    previousBet = bet;

                    // Since someone raised, give everyone a chance to play again, except for the person who just raised
                    for (auto& remainingPlayer : players)
                    {
                        remainingPlayer->isDone = false;
                    }
                    player->isDone = true;
                }
                else if (bet == 0 && bettingIsOpen)
                {
                    std::cout << player->name << " folds." << std::endl;
                    player->isFolded = true;
                }
                else if (bet == previousBet)
                {
                    std::cout << player->name << " calls." << std::endl;
                    player->isDone = true;
                }

                moneyPot += bet; // Add valid bets to the pot
                std::cout << "The pot contains: $" << moneyPot << std::endl << std::endl;
            }
        }

        // Edge case for if everyone checks continuously
        if (!bettingIsOpen) 
        {
            bettingDone = false;
        }

        // Check if the round is over
        for (auto& player : players)
        {
            if (!player->isDone && !player->isFolded)
            {
                bettingDone = false;
            }
        }
    }
    std::cout << "The betting round is over!" << std::endl << std::endl;
}

void PokerGame::drawRound()
{
    std::cout << "Begin the draw round!" << std::endl << std::endl;

    for (auto& player : players)
    {
        if (!player->isFolded)
        {
            player->drawCards(this->deck);
        }  
    }
    std::cout << "The draw round is over!" << std::endl << std::endl;
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
