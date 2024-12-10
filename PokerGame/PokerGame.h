#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

#include "Card.h"
#include "PlayerI.h"
#include "HumanPlayer.h"
#include "AiPlayer.h"
#include "pokerHand.h"

class PokerGame 
{
public:
	PokerGame();
	void shuffleDeck();
	void initGame();
	void playPoker();
	void createDeck();

private:
	std::vector<CardClass::Card> deck;
	std::vector<std::shared_ptr<PlayerI>> players;
	const int MAX_NUM_PLAYERS = 7;
	const int MIN_NUM_PLAYERS = 2;
	int moneyPot;
	int previousBet;
	int ante;
	bool bettingIsOpen;
	
	void dealCards();
	void showDown();
	bool checkFoldedWinner();
	void bettingRound();
	void drawRound();
	int getValidUserInt();

};