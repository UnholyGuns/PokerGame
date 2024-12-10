#include "pokerHand.h"
#include <iostream>
#include <algorithm>

pokerHand::pokerHand(const std::vector<CardClass::Card>& hand)
{
	try 
	{
		parseHandVector(hand);
	}
	catch (const std::invalid_argument& e) 
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

int pokerHand::compare(pokerHand& otherHand)
{
	int myHand = determineHandRank();
	int yourHand = otherHand.determineHandRank();
	int iWin = 3;// Initialize to a tie state

	if (myHand > yourHand) 
	{
		iWin = 1;
	}
	else if (myHand < yourHand)
	{
		iWin = 2;
	}
	else
	{
		// This is the tie condition.
		// Determine all ranks and quantity present in each hand
		int rankFreq1[numDifferentRanksInHand];
		int rankFreq2[numDifferentRanksInHand];
		countRankRepeats(rankFreq1);
		otherHand.countRankRepeats(rankFreq2);

		iWin = compareArrays(rankFreq1, rankFreq2);
	}

	return iWin;
}

std::string pokerHand::getHandString() const
{
	return handString;
}

std::string pokerHand::getHandRankString() const
{
	return handRankString;
}

int pokerHand::getHandRankInt() const
{
	return handRankType;
}

void pokerHand::parseHandVector(const std::vector<CardClass::Card>& hand)
{
	// Check the size of the input hand
	if (hand.size() != numCardsInHand) {
		throw std::invalid_argument("Incorrect hand size.");
	}

	// Clear the arrays to ensure they start fresh
	std::fill(std::begin(ranksInHand), std::end(ranksInHand), 0);
	std::fill(std::begin(suitsinHand), std::end(suitsinHand), '\0');

	for (size_t i = 0; i < hand.size(); i++) {
		const auto& card = hand[i];

		if (card.rank == "2")      ranksInHand[i] = 2;
		else if (card.rank == "3") ranksInHand[i] = 3;
		else if (card.rank == "4") ranksInHand[i] = 4;
		else if (card.rank == "5") ranksInHand[i] = 5;
		else if (card.rank == "6") ranksInHand[i] = 6;
		else if (card.rank == "7") ranksInHand[i] = 7;
		else if (card.rank == "8") ranksInHand[i] = 8;
		else if (card.rank == "9") ranksInHand[i] = 9;
		else if (card.rank == "10") ranksInHand[i] = 10;
		else if (card.rank == "J") ranksInHand[i] = 11;
		else if (card.rank == "Q") ranksInHand[i] = 12;
		else if (card.rank == "K") ranksInHand[i] = 13;
		else if (card.rank == "A") ranksInHand[i] = 14;
		else throw std::invalid_argument("Invalid hand rank.");

		if (card.suit == "Clubs")      suitsinHand[i] = 'C';
		else if (card.suit == "Diamonds") suitsinHand[i] = 'D';
		else if (card.suit == "Hearts") suitsinHand[i] = 'H';
		else if (card.suit == "Spades") suitsinHand[i] = 'S';
		else throw std::invalid_argument("Invalid hand suit.");
	}
}

int pokerHand::determineHandRank()
{
	if (isFlush() && isStraight())
	{
		handRankString = "Straight Flush";
		handRankType = STRAIGHT_FLUSH;
		return handRankType;
	}
	else if (isFlush()) 
	{
		handRankString = "Flush";
		handRankType = FLUSH;
		return handRankType;
	}
	else if (isStraight()) 
	{
		handRankString = "Straight";
		handRankType = STRAIGHT;
		return handRankType;
	}

	int rankCount[numDifferentRanksInHand];
	int maxFreq1 = 0;
	int MaxFreq2 = 0;
	countRankRepeats(rankCount);

	for (int i = 0; i < numDifferentRanksInHand; i++) 
	{
		if (rankCount[i] > maxFreq1) 
		{
			MaxFreq2 = maxFreq1;
			maxFreq1 = rankCount[i];
		}
		else if (rankCount[i] > MaxFreq2) 
		{
			MaxFreq2 = rankCount[i];
		}
	}

	if (maxFreq1 == 4) 
	{
		handRankString = "Four of a kind";
		handRankType = FOUR_OF_A_KIND;
	}
	else if (maxFreq1 == 3 && MaxFreq2 == 2) 
	{
		handRankString = "Full house";
		handRankType = FULL_HOUSE;
	}
	else if (maxFreq1 == 3) 
	{
		handRankString = "Three of a kind";
		handRankType = THREE_OF_A_KIND;
	}
	else if (maxFreq1 == 2 && MaxFreq2 == 2) 
	{
		handRankString = "Two pair";
		handRankType = TWO_PAIR;
	}
	else if (maxFreq1 == 2) 
	{
		handRankString = "One pair";
		handRankType = ONE_PAIR;
	}
	else 
	{
		handRankString = "High card";
		handRankType = HIGH_CARD;
	}

	return handRankType;
}

void pokerHand::countRankRepeats(int rankCount[numDifferentRanksInHand]) const
{
	std::memset(rankCount, 0, sizeof(int) * numDifferentRanksInHand);// Initialize array to all 0s
	for (int i = 0; i < numCardsInHand; i++) 
	{
		rankCount[ranksInHand[i] - 2]++;// Fit the ranks (2-14) into array indicies(0-12). Increment the ranks that are present
	}
}

bool pokerHand::isFourOfAKind()
{
	int rankCount[numDifferentRanksInHand];
	bool valFound = false;

	countRankRepeats(rankCount);

	// Check the formatted array and see if there are four of a kind present
	for (int i = 0; i < numDifferentRanksInHand; i++) 
	{
		if (4 == rankCount[i]) 
		{
			maxSigRank = i;
			valFound = true;
		}
		if (1 == rankCount[i]) 
		{
			maxKickerRank = i;
		}
	}

	return valFound;
}

bool pokerHand::isFlush()
{
	// Check each suit in the hand against the first suit in the hand
	for (int i = 1; i < numCardsInHand; i++) 
	{
		if (suitsinHand[0] != suitsinHand[i]) 
		{
			return false;
		}
	}

	return true;
}

bool pokerHand::isStraight() const
{
	int sortedRanksInHand[numCardsInHand];
	int temp;

	std::memcpy(sortedRanksInHand, ranksInHand, sizeof(ranksInHand));
	
	for (int i = 0; i < numCardsInHand; i++) 
	{
		for (int j = i+1; j < numCardsInHand; j++) 
		{
			if (sortedRanksInHand[j] > sortedRanksInHand[i]) 
			{
				temp = sortedRanksInHand[j];
				sortedRanksInHand[j] = sortedRanksInHand[i];
				sortedRanksInHand[i] = temp;
			}
		}
	}

	bool isAceHighStraight = true;

	for (int i = 1; i < numCardsInHand; ++i) 
	{
		if (sortedRanksInHand[i] != sortedRanksInHand[i - 1] - 1) 
		{
			isAceHighStraight = false;
			break;
		}
	}

	// Check for ace-low straight
	bool isAceLowStraight = false;

	if (sortedRanksInHand[0] == 14) // If the hand contains an ace
	{
		// Replace ace's value with 1 and re-check for straight
		sortedRanksInHand[0] = 1;

		// Re-sort in ascending order for ace-low straight check
		for (int i = 0; i < numCardsInHand; i++)
		{
			for (int j = i + 1; j < numCardsInHand; j++)
			{
				if (sortedRanksInHand[j] < sortedRanksInHand[i])
				{
					temp = sortedRanksInHand[j];
					sortedRanksInHand[j] = sortedRanksInHand[i];
					sortedRanksInHand[i] = temp;
				}
			}
		}

		isAceLowStraight = true;

		for (int i = 1; i < numCardsInHand; ++i)
		{
			if (sortedRanksInHand[i] != sortedRanksInHand[i - 1] + 1)
			{
				isAceLowStraight = false;
				break;
			}
		}
	}

	return isAceHighStraight || isAceLowStraight;
}

bool pokerHand::isThreeOfAKind() const
{
	int rankCount[numDifferentRanksInHand];

	countRankRepeats(rankCount);

	// Check the formatted array and see if there are three of a kind present
	for (int i = 0; i < numDifferentRanksInHand; i++) 
	{
		if (3 == rankCount[i]) 
		{
			return true;
		}
	}

	return false;
}

bool pokerHand::isTwoPair() const
{
	int rankCount[numDifferentRanksInHand];
	int pairCount = 0;

	countRankRepeats(rankCount);

	// Check the formatted array and see if there are two pairs
	for (int i = 0; i < numDifferentRanksInHand; i++) 
	{
		if (2 == rankCount[i]) 
		{
			pairCount++;
		}
	}

	return (2 == pairCount);
}

bool pokerHand::isOnePair() const
{
	int rankCount[numDifferentRanksInHand];
	int pairCount = 0;

	countRankRepeats(rankCount);

	// Check the formatted array and see if there are two pairs
	for (int i = 0; i < numDifferentRanksInHand; i++) 
	{
		if (2 == rankCount[i]) 
		{
			pairCount++;
		}
	}

	return (1 == pairCount);
}

// ChatGPT helped with this function, it is pretty clever and definitely more creative/concise than I could have been. 
int pokerHand::compareArrays(int arrayA[], int arrayB[]) const {

	std::vector<std::pair<int, int>> handA, handB; // (count, rank)

	// Extract all ranks and count
	for (int i = 0; i < 14; ++i) 
	{
		if (arrayA[i] > 0)
		{
			handA.emplace_back(arrayA[i], i); 
		}
		if (arrayB[i] > 0) 
		{
			handB.emplace_back(arrayB[i], i); 
		}
	}

	// Step 2: Sort by count descending, then rank descending
	auto compare = [](const std::pair<int, int>& a, const std::pair<int, int>& b)
		{
		if (a.first != b.first) 
		{
			return a.first > b.first; // Higher count first
		}
		return a.second > b.second; // Higher rank first
		};

	std::sort(handA.begin(), handA.end(), compare);
	std::sort(handB.begin(), handB.end(), compare);

	// Step 3: Compare sorted hands
	for (size_t i = 0; i < std::min(handA.size(), handB.size()); ++i) 
	{
		if (handA[i].second > handB[i].second) 
		{
			return 1; // Hand A wins
		}
		if (handA[i].second < handB[i].second) 
		{
			return 2; // Hand B wins
		}
	}

	// If all ranks are the same, it's a tie
	return 0;
}

