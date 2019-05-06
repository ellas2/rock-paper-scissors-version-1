/*
 * RPSGame.h
 *
 *  Created on: Apr 2, 2018
 *      Author: yevkirich
 */

#ifndef RPSGAME_H_
#define RPSGAME_H_

#include "GamePiece.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <regex>

#define out_file_path "./rps.output"
#define player_one_positioning_file_path "./player1.rps_board"
#define player_two_positioning_file_path "./player2.rps_board"
#define player_one_moves_file_path "./player1.rps_moves"
#define player_two_moves_file_path "./player2.rps_moves"

using namespace std;
//TODO: changed firstPlayer and secondPlayer to public in order to print properly to output - should change?
class RPSGame{
	Player* firstPlayer;
	Player* secondPlayer;
	Player* currentPlayer;
	int rows;
	int cols;
	GamePiece** board;
public:
	RPSGame(int Rows, int Cols);
	GamePiece& getPiece(int i, int j){return board[i][j];};
	Player* getFirstPlayer(){return firstPlayer;};
	Player* getSecondPlayer(){return secondPlayer;};

	/*
	 * Place pieceRep that belongs to player at the given location. joker == true in case its a joker piece
	 */
	RPSMessage movePieceToLocation(int row, int col, char pieceRep, Player* player, bool joker = false);
	
	void printBoard();

	RPSMessage loadPiecesFromPlayer(Player* player);

	/*
	 * Make a move from source location to dest location
	 */
	RPSMessage makePieceMove(int fromRow, int fromCol, int toRow, int toCol);

	void switchCurrentPlayer();

	RPSMessage checkWinner();

	/*
	 * Check if both players loaded enough flags from the positioning files, and return appropriate message
	 */
	RPSMessage checkPlayersBeforeStart();
	RPSMessage jokerChange(int row, int col, char newPiece);
	RPSMessage loadInitalLocationToPlayer(string fileName, int playerNumber, int& lineNumber);
	RPSMessage loadAndExecuteMoves(string firstFileName, string secondFileName, int& firstLineNum, int& secondLineNum);
	RPSMessage loadAndExecuteSingleMove(ifstream& fin, bool& isFileDone, int playerNum);
	void printBoardToFile(ostream& fout);
	void printGameOutputToFile(string reason, int winner, bool isBadPositioning);
	int initializeGameWithPlayersInput(int& winner, string& reason, int& firstLineNum, int& secondLineNum);
	int loadInitialLocationsFromInput(int& winner, string& reason, int& firstLineNum, int& secondLineNum);
	int checkAndPrintFlagsToOutputFile(int& winner, string& reason, int& firstLineNum, int& secondLineNum);
	int loadPiecesFromPlayers(int& winner, string& reason);
	int checkAndPrintWinnerToOutputFile(RPSMessage msg, string& reason, int& winner, bool posFiles = false);
	int executeMovesAccordingToPlayersInput(int& winner, string& reason, int& firstLineNum, int& secondLineNum);
	~RPSGame();
};


#endif /* RPSGAME_H_ */
