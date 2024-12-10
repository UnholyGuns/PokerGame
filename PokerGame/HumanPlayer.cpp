#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(std::string name)
{
	this->name = name;
    moneyBet = 0;
    isDone = false;
    isFolded = false;
}

int HumanPlayer::placeBet(int previousBet, bool& bettingIsOpen)
{
    lookAtHand();

    if (bettingIsOpen) 
    {
        std::cout << "The current bet is $" << previousBet << std::endl;
        std::cout << "You can choose to:" << std::endl;
        std::cout << "1. Call (match the bet)" << std::endl;
        std::cout << "2. Raise (bet more)" << std::endl;
        std::cout << "3. Fold (exit the round)" << std::endl;
    }
    else 
    {
        std::cout << "No bets have been placed yet. You can:" << std::endl;
        std::cout << "1. Check" << std::endl;
        std::cout << "2. Bet a starting amount (Ante is: $" << previousBet << ")" << std::endl;
    }

    int choice = 0;
    int betAmount = 0;

    while (true) 
    {
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        if (choice == 1)
        {
            // Call or check
            if (bettingIsOpen)
            {
                moneyBet += previousBet;
                return previousBet;
            }
            else 
            {
                std::cout << "You checked." << std::endl;
                return 0;
            }
        }
        else if (choice == 2) 
        {
            // Raise or place a starting bet
            if (bettingIsOpen)
            {
                std::cout << "Enter the amount to raise: ";
            }
            else 
            {
                std::cout << "Enter your starting bet: ";
            }

            std::cin >> betAmount;
            if (betAmount > previousBet ) 
            {
                if (!bettingIsOpen) 
                {
                    bettingIsOpen = true;
                }
                moneyBet += betAmount;
                return betAmount;
            }
            else 
            {
                std::cout << "Invalid bet. Try again." << std::endl;
            }
        }
        else if (choice == 3 && bettingIsOpen)
        {
            // Fold
            isFolded = true;
            return 0;
        }
        else 
        {
            std::cout << "Invalid choice. Try again." << std::endl;
        }
    }
}


void HumanPlayer::drawCards(std::vector<CardClass::Card> deck)
{
    std::cout << this->name << "'s turn to draw new cards!" << std::endl;

    lookAtHandIndices();

    std::cout << "Enter the indices of the cards you want to replace (space-separated).";
    std::cout << "Press Enter without inputting anything to keep all cards: ";

    // Clear the input buffer only if necessary
    if (std::cin.peek() == '\n') 
    {
        std::cin.ignore(); 
    }

    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) 
    {
        std::cout << "No cards will be replaced." << std::endl;
        return;
    }

    std::vector<int> indicesToReplace;
    std::istringstream iss(input);
    int index;

    while (iss >> index) 
    {
        if (index >= 0 && static_cast<size_t>(index) < hand.size()) 
        {
            indicesToReplace.push_back(index);
        }
        else 
        {
            std::cout << "Invalid index: " << index << ". Skipping." << std::endl;
        }
    }

    // Replace the selected cards
    for (int indexx : indicesToReplace) 
    {
        if (!deck.empty()) {
            hand[indexx] = deck.back(); 
            deck.pop_back();        
        }
        else 
        {
            std::cout << "Deck is empty! Cannot draw more cards." << std::endl;
            break;
        }
    }

    // Display the updated hand
    std::cout << std::endl << this->name << "'s updated hand:" << std::endl;
    for (size_t i = 0; i < hand.size(); i++) 
    {
        std::cout << i << ": " << hand[i].rank << " of " << hand[i].suit << std::endl;
    }

    std::cout << std::endl;
}

void HumanPlayer::lookAtHand()
{
    std::cout << this->name << "'s hand: " << std::endl;

    for (const auto& card : hand) 
    {
        std::cout << card.rank << " of " << card.suit << ", ";
    }

    std::cout << std::endl << std::endl;
}

void HumanPlayer::lookAtHandIndices()
{
    std::cout << this->name << "'s hand:" << std::endl;

    for (size_t i = 0; i < hand.size(); ++i) 
    {
        std::cout << i << ": " << hand[i].rank << " of " << hand[i].suit << std::endl;
    }

    std::cout << std::endl;
}
