/*
 * Player.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: yevkirich
 */
#include "Player.h"
#include <string>


Player::Player(int playerID, int Rows, int Cols)
{
	id = playerID;
	rows = Rows;
	cols = Cols;
	numOfFlags = 0;
	numOfBombs = 0;
	numOfRocks = 0;
	numOfPapers = 0;
	numOfScissors = 0;
	numOfJokers = 0;
	numOfJokerBombs = 0;
	piecesToLoad = new string*[rows];
	for (int i = 0; i < rows; i++)
	{
		piecesToLoad[i] = new string[cols];
	}
	for (int i = 0; i < rows; i++)
	{
		 for(int j =0 ; j < cols; j++)
			piecesToLoad[i][j] = "_";
	}
}
Player::~Player()
{
	for (int i = 0; i < rows; i++)
	{
		delete[] piecesToLoad[i];
	}
	delete []piecesToLoad;
}

void Player::updateJokerBombsCount(bool addOne)
{
	numOfJokerBombs = addOne? numOfJokerBombs+1:numOfJokerBombs-1;
}
RPSMessage Player::updatePieceCount(char pieceRep, bool addOne, bool joker)
{
	char currPiece = tolower(pieceRep);
	if (joker){
		numOfJokers = addOne? numOfJokers+1:numOfJokers-1;
		if (currPiece == 'b'){
			numOfJokerBombs = addOne? numOfJokerBombs +1:numOfJokerBombs -1;
		}

		if (numOfJokers > J)
			return RPSTooManyJokers;
	}
	else if (currPiece == 'f'){
		numOfFlags = addOne? numOfFlags+1:numOfFlags-1;
		if (numOfFlags > F)
			return RPSTooManyFlags;
	}

	else if (currPiece == 'b'){
		numOfBombs = addOne? numOfBombs+1:numOfBombs-1;
		if (numOfBombs > B)
			return RPSTooManyBombs;
	}

	else if (currPiece == 'r'){
		numOfRocks = addOne? numOfRocks+1:numOfRocks-1;
		if (numOfRocks > R)
			return RPSTooManyRocks;
	}

	else if (currPiece == 'p'){
		numOfPapers = addOne? numOfPapers+1:numOfPapers-1;
		if (numOfPapers > P)
			return RPSTooManyPapers;
	}

	else if (currPiece == 's'){
		numOfScissors = addOne? numOfScissors+1:numOfScissors-1;
		if (numOfScissors > S)
			return RPSTooManyScissors;
	}

	return RPSSuccess;

}

RPSMessage Player::addInitialPiece(int row, int col, char piece, bool joker)
{
	if (row > rows || col > cols)
	{
		return RPSWrongSourceCoords;
	}
	if (piecesToLoad[row][col].compare("_") != 0)
		return RPSLocationIsTaken;
	char currPiece = tolower(piece);
	if (!(currPiece == 'r' || currPiece == 'p' || currPiece == 's' || currPiece == 'b' || currPiece == 'f')){
		return RPSWrongPiece;
	}

	if (!joker){
		piecesToLoad[row][col] = piece;
		return updatePieceCount(piece, true);
	}
	else{
		string newSTR = "j";
		newSTR += piece;
		piecesToLoad[row][col] = newSTR;
	}

	return updatePieceCount(piece, true, true);


}

string Player::getPiece(int row, int col)
{
	return piecesToLoad[row][col];
}

