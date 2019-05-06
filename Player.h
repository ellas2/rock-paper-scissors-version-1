/*
 * Player.h
 *
 *  Created on: Apr 2, 2018
 *      Author: yevkirich
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <cctype>
#include "RPSMessage.h"
#include <iostream>

using namespace std;

#define R 2
#define P 5
#define S 1
#define B 2
#define J 2
#define F 1

class Player{
	int id;
	int rows;
	int cols;
	int numOfFlags;
	int numOfBombs;
	int numOfRocks;
	int numOfPapers;
	int numOfScissors;
	int numOfJokers;
	int numOfJokerBombs; 
	string** piecesToLoad;
public:
	Player(int playerID, int Rows, int Cols);
	RPSMessage updatePieceCount(char pieceRep, bool addOne, bool joker = false);
	RPSMessage addInitialPiece(int row, int col, char piece, bool joker = false);
	int getPlayerId(){ return id;};
	void updateJokerBombsCount(bool addOne);
	string getPiece(int row, int col);
	bool hasFlags() {return numOfFlags? true:false;};
	bool hasRightAmountOfFlags() {return numOfFlags == F? true:false;};
	bool hasMovingPieces() {return (numOfRocks+numOfPapers+numOfScissors+numOfJokers-numOfJokerBombs != 0)? true:false;};
	~Player();
};



#endif /* PLAYER_H_ */
