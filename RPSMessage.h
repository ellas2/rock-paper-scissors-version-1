/*
 * RPSMessage.h
 *
 *  Created on: Apr 3, 2018
 *      Author: yevkirich
 */

#ifndef RPSMESSAGE_H_
#define RPSMESSAGE_H_

#include <iostream>

using namespace std;

typedef enum rsp_game_message{
	RPSFileFailure,
	RPSWrongPiece,
	RPSNotAJokerPiece,
	RPSTooManyFlags,
	RPSTooManyBombs,
	RPSTooManyRocks,
	RPSTooManyPapers,
	RPSTooManyScissors,
	RPSNotEnoughFlags,
	RPSLocationIsTaken,
	RPSTargetLocationContainsPlayerPiece,
	RPSTooManyJokers,
	RPSNoFlags,
	RPSWrongInputLine,
	RPSWrongDestCoords,
	RPSWrongSourceCoords,
	RPSSourceLocationDoesntContainsPlayerPiece,
	RPSLocationIsEmpty,
	RPSInvalidPieceMove,
	RPSPieceCannotMove,
	RPSBothPlayersNotEnoughFlags,
	RPSPlayer1NotEnoughFlags,
	RPSPlayer2NotEnoughFlags,
	RPSTieNoFlags,
	RPSTieNoMovingPieces,
	RPSPlayer1WonNoOpponentFlags,
	RPSPlayer1WonNoOpponentMovingPieces,
	RPSPlayer2WonNoOpponentFlags,
	RPSPlayer2WonNoOpponentMovingPieces,
	RPSWrongMoveLinePlayer1,
	RPSWrongMoveLinePlayer2,
	RPSSuccess
} RPSMessage;

void printMSG(RPSMessage msg);
bool isBadPositioning(RPSMessage msg);
bool isWinnerRelated(RPSMessage msg);

#endif /* RPSMESSAGE_H_ */
