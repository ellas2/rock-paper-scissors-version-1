/*
 * main.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: yevkirich
 */
#include "RPSGame.h"
#include <string>
#define M 10
#define N 10






int main(){
	RPSGame game(N,M);
	string reason;
	int winner = 0;
	int firstLineNum, secondLineNum;

	int initializer = game.initializeGameWithPlayersInput(winner, reason, firstLineNum, secondLineNum);
	if (initializer == 0)
		return 0;
	int move = game. executeMovesAccordingToPlayersInput(winner, reason, firstLineNum, secondLineNum);

	return move;
}


