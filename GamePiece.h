/*
 * GamePiece.h
 *
 *  Created on: Apr 2, 2018
 *      Author: yevkirich
 */

#ifndef GAMEPIECE_H_
#define GAMEPIECE_H_

#include "Player.h"
#include <iostream>

class GamePiece{
	char pieceRep;
	Player* owner;
	bool isJoker;
public:
	GamePiece(char piece = '_', Player* player = NULL, bool joker = false): pieceRep(piece), owner(player), isJoker(joker){};
	char getCharRep() const {return pieceRep;};
	void changePieceRep(char newPiece){ pieceRep = newPiece;};
	Player* getOwner() const {return owner;};
	bool pieceIsJoker() const {return isJoker;};
	int fight(const GamePiece& pieceB);
	int fight(char pieceB);
};


#endif /* GAMEPIECE_H_ */
