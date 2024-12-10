#include <iostream>
#include "PokerGame.h"

using namespace std;

int main() 
{
	PokerGame mainGame;

	mainGame.initGame();
	mainGame.createDeck();
	mainGame.shuffleDeck();
	mainGame.playPoker();

	return 0;
}