/*
 * RPSMessage.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: yevkirich
 */

#include "RPSMessage.h"
bool isBadPositioning(RPSMessage msg){
	if (msg == RPSTooManyFlags || msg == RPSTooManyBombs || msg == RPSTooManyRocks || msg == RPSTooManyPapers || msg == RPSTooManyScissors || msg == RPSNoFlags ||
			msg == RPSTargetLocationContainsPlayerPiece || msg == RPSWrongSourceCoords || msg == RPSWrongPiece || msg == RPSLocationIsTaken)
		return true;
	return false;
}

bool isWinnerRelated(RPSMessage msg){
	if (msg == RPSTieNoFlags || msg == RPSTieNoMovingPieces || msg == RPSPlayer1WonNoOpponentFlags || msg == RPSPlayer1WonNoOpponentMovingPieces || msg == RPSPlayer2WonNoOpponentFlags ||
	msg == RPSPlayer2WonNoOpponentMovingPieces)
		return true;
	return false;
}


void printMSG(RPSMessage msg)
{
	if (msg == RPSPieceCannotMove)
	{
		cout << "ERROR: flags and bombs cannot move" << endl;
	}
	else if (msg == RPSTooManyFlags)
	{
		cout << "ERROR: too many flags" << endl;
	}
	else if (msg == RPSTooManyBombs)
	{
		cout << "ERROR: too many bombs" << endl;
	}
	else if (msg == RPSTooManyRocks)
	{
		cout << "ERROR: too many rocks" << endl;
	}
	else if (msg == RPSTooManyPapers)
	{
		cout << "ERROR: too many papers" << endl;
	}
	else if (msg == RPSTooManyScissors)
	{
		cout << "ERROR: too many scissors" << endl;
	}
	else if (msg == RPSNotEnoughFlags)
	{
		cout << "ERROR: not enough flags" << endl;
	}
	else if (msg == RPSLocationIsTaken)
	{
		cout << "ERROR: the location already contains a piece" << endl;
	}
	else if (msg == RPSTooManyJokers)
	{
		cout << "ERROR: too many jokers" << endl;
	}
	else if (msg == RPSNoFlags)
	{
		cout << "ERROR: no flags" << endl;
	}
	else if (msg == RPSWrongDestCoords)
	{
		cout << "ERROR: bad destination coordinates" << endl;
	}
	else if (msg == RPSWrongSourceCoords)
	{
		cout << "ERROR: bad source coordinates" << endl;
	}
	else if (msg == RPSSourceLocationDoesntContainsPlayerPiece)
	{
		cout << "ERROR: source location does't contain a player piece" << endl;
	}
	else if (msg == RPSTargetLocationContainsPlayerPiece)
	{
		cout << "ERROR: target already contains same player's piece" << endl;
	}
	else if (msg == RPSLocationIsEmpty)
	{
		cout << "ERROR: source location is empty" << endl;
	}
	else if (msg == RPSInvalidPieceMove)
	{
		cout << "ERROR: incorrect piece move" << endl;

	}
	else if (msg == RPSBothPlayersNotEnoughFlags)
	{
		cout << "Both Players dont have enough flags" << endl;
	}
	else if (msg == RPSPlayer1NotEnoughFlags)
	{
		cout << "Player 1 doesnt have enough flags" << endl;
	}
	else if (msg == RPSPlayer2NotEnoughFlags)
	{
		cout << "Player 2 doesnt have enough flags" << endl;
	}
	else if (msg == RPSTieNoFlags)
	{
		cout << "Its a Tie! No flags left for both players" << endl;
	}
	else if (msg == RPSTieNoMovingPieces)
	{
		cout << "Its a Tie! No moving pieces left for both players" << endl;
	}
	else if (msg == RPSPlayer1WonNoOpponentFlags)
	{
		cout << "Player 1 Wins - All opponent flags are captured! " << endl;
	}
	else if (msg == RPSPlayer1WonNoOpponentMovingPieces)
	{
		cout << "Player 1 Wins - All opponent moving pieces are eaten! " << endl;
	}
	else if (msg == RPSPlayer2WonNoOpponentFlags)
	{
		cout << "Player 2 Wins - All opponent flags are captured! " << endl;
	}
	else if (msg == RPSPlayer2WonNoOpponentMovingPieces)
	{
		cout << "Player 2 Wins - All opponent moving pieces are eaten! " << endl;
	}
	else if (msg == RPSWrongPiece)
	{
		cout << "ERROR: wrong piece" << endl;
	}
	else if (msg == RPSWrongInputLine)
	{
		cout << "ERROR: wrong input line" << endl;
	}
	else if (msg == RPSFileFailure)
	{
		cout << "ERROR: File failure" << endl;
	}
}
