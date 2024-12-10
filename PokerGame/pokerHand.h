#ifndef POKERHAND_H
#define POKERHAND_H

#include <string>
#include <vector>

#include "Card.h"

class pokerHand 
{
public:
    // Define public member functions
	pokerHand(const std::vector<CardClass::Card>& hand);// Constructor 
    int compare(pokerHand& otherHand);// Member function used to compare two hands
    std::string getHandString() const;// Member function to print out the parsed hand string DEBUGGGG
    std::string getHandRankString() const;
    int getHandRankInt() const;

private:
    // Create an enum for the different hand types
    enum HandRank { HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH };

    // Define constants so we dont have magic numbers
    static const int numCardsInHand = 5;// Number of cards in a hand
    static const int numDifferentRanksInHand = 13;
    static const int numDifferentSuits = 4;
    static const int numInputStringChars = 14;// The valid string input has 14 characters including white space

    // Define private member data
	std::string handString;// Class data member representing a poker hand
    std::string handRankString;
    int handRankType;
    int maxSigRank;
    int maxKickerRank;
    int flushValues[numCardsInHand];
    int ranksInHand[numCardsInHand];// Contains the rank values
    char suitsinHand[numCardsInHand];// Contains the suit values
    
    // Define private member functions
    void parseHandVector(const std::vector<CardClass::Card>& hand);
    int determineHandRank();
    void countRankRepeats(int rankCount[numDifferentRanksInHand]) const;// populates the count array with total number of each rank present in the hand
    bool isFourOfAKind();// Determines if a hand is a four of a kind
    bool isFlush();// Determines if a hand is a flush
    bool isStraight() const;// Determines if a hand is a straight
    bool isThreeOfAKind() const;// Determines if a hand is a three of a kind
    bool isTwoPair() const;// Determines if a hand is a two pair
    bool isOnePair() const;// Determines if a hand is a one pair
    int compareArrays(int arrayA[], int arrayB[]) const;

};

#endif
