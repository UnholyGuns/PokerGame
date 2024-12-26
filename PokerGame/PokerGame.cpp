#include "PokerGame.h"

void PokerGame::createDeck()
{
    const std::vector<std::string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    const std::vector<std::string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };

    for (const auto& rank : ranks) 
    {
        for (const auto& suit : suits) 
        {
            this->deck.push_back({ rank, suit });
        }
    }
}

PokerGame::PokerGame(){
    moneyPot = 0;
    maxBet = 0;
    ante = 0;
    bettingIsOpen = false;
}

void PokerGame::shuffleDeck()
{
    // Seed the rand function so its different every time
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (size_t i = this->deck.size(); i > 1; i--)
    {
        int j = std::rand() % i; // Generate a random index within bounds
        std::swap(this->deck[i - 1], this->deck[j]); // Swap the last element with a random one
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

    // Get number of AI players (2 <= max players <= 7)
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
        std::getline(std::cin, name);
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
    maxBet = ante;
    bettingIsOpen = false;
    bettingRound();

    // See if all but one person folded
    if (checkFoldedWinner()) {
        return;
    }

    // Draw Round
    drawRound();

    // Fresh betting round, per standard rules
    maxBet = ante;
    bettingIsOpen = false;

    // Reset money bet in current round counter
    for (auto& player : players)
    {
        player->moneyBetInRound = 0;
    }

    bettingRound();

    // Its showtime
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
    if (players.empty()) 
    {
        std::cout << "No players to create hands for." << std::endl;
        return;
    }

    // Create a vector to store pokerHand objects and track active (non-folded) players
    std::vector<pokerHand> activeHands;
    std::vector<std::shared_ptr<PlayerI>> activePlayers;

    for (const auto& player : players) 
    {
        if (!player->isFolded)
        {
            activeHands.emplace_back(player->hand);
            activePlayers.push_back(player); // Track the player corresponding to the hand
        }
    }

    std::cout << "It's showtime." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::cout << "Everyone drops their cards..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(6000));

    // Determine the best hand and the corresponding player
    size_t bestIndex = 0;

    for (size_t i = 1; i < activeHands.size(); i++) 
    {
        if (activeHands[i].compare(activeHands[bestIndex]) == 1)
        {
            bestIndex = i;
        }
    }

    std::cout << "The winner is: " << activePlayers[bestIndex]->name << "!" << std::endl; 
    activePlayers[bestIndex]->lookAtHand(); 
    std::cout << "Which is a " << activeHands[bestIndex].getHandRankString() << std::endl << std::endl;
    std::cout << "Pot won: $ " << moneyPot << std::endl << std::endl;
    std::cout << "Player Stats: " << std::endl;

    for (const auto& player : players) 
    {
        std::cout << player->name << std::endl << "Money bet: $" << player->moneyBetTotal << std::endl;
        player->lookAtHand();
    }

    std::cout << std::endl << "Thanks for playing!" << std::endl;
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
                int prevMaxBet = maxBet;

                std::cout << player->name << "'s turn to place a bet!" << std::endl;

                int bet = player->placeBet(maxBet, bettingIsOpen);

                if (maxBet > prevMaxBet)
                {
                    std::cout << player->name << " raises to $" << maxBet << std::endl;

                    // Since someone raised, give everyone a chance to play again, except for the person who just raised
                    for (auto& remainingPlayer : players)
                    {
                        remainingPlayer->isDone = false;
                    }

                    player->isDone = true;
                }
                else if (bet == -1)
                {
                    std::cout << player->name << " folds." << std::endl;
                    player->isFolded = true;
                    bet = 0;
                }
                else if (bet != 0)
                {
                    std::cout << player->name << " calls." << std::endl;
                    player->isDone = true;
                }
                else
                {
                    std::cout << player->name << " checks." << std::endl;
                }

                moneyPot += bet; // Add valid bets to the pot
                std::cout << std::endl << "The pot contains: $" << moneyPot << std::endl << std::endl;
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
    while (true) 
    {
        std::cin >> value;

        // Check if input is valid
        if (std::cin.fail() || value < 0) {
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a valid integer." << std::endl;
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard extra input
            return value;
        }
    }
}
