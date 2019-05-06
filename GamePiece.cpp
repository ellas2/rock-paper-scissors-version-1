/*
 * GamePiece.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: yevkirich
 */

#include "GamePiece.h"
#include <cctype>


int GamePiece::fight(const GamePiece& pieceB)
{
	char opponent = pieceB.getCharRep();
	return fight(opponent);
}

int GamePiece::fight(char pieceB)
{
	char opponent = tolower(pieceB);
	char current = tolower(pieceRep);
	if (current == opponent)
	{
		return 0;
	}
	else if (current == 'b' || opponent == 'f')
		return 1;
	else if (opponent == 'b' || current == 'f')
		return -1;
	else if ((current == 'r' && opponent == 's') || (current == 'p' && opponent == 'r') || (current == 's' && opponent == 'p'))
		return 1;
	else return -1;
}


