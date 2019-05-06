/*
 * RPSGame.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: yevkirich
 */

#include "RPSGame.h"
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>



RPSGame::RPSGame(int Rows, int Cols)
{
	rows = Rows;
	cols = Cols;
	board = new GamePiece*[Rows];
	for (int i = 0; i < Rows; i++)
	{
		board[i] = new GamePiece[Cols];
	}
	firstPlayer = new Player(1,Rows,Cols);
	secondPlayer = new Player(2,Rows,Cols);
	currentPlayer = firstPlayer;

}
RPSGame::~RPSGame()
{
	delete firstPlayer;
	delete secondPlayer;
	for (int i = 0; i < rows; i++)
	{
		delete[] board[i];
	}
	delete []board;
}

void RPSGame::switchCurrentPlayer(){
	if (currentPlayer == firstPlayer)
		currentPlayer = secondPlayer;
	else
		currentPlayer= firstPlayer;
}



RPSMessage RPSGame::checkWinner(){


	if (!firstPlayer->hasFlags() && !secondPlayer->hasFlags())
		return RPSTieNoFlags;
	else if (!firstPlayer->hasFlags())
			return RPSPlayer2WonNoOpponentFlags;
	else if (!secondPlayer->hasFlags())
		return RPSPlayer1WonNoOpponentFlags;

	else if (!firstPlayer->hasMovingPieces() && !secondPlayer->hasMovingPieces()){
		return RPSTieNoMovingPieces;
	}
	else if (!firstPlayer->hasMovingPieces()){
		return RPSPlayer2WonNoOpponentMovingPieces;
	}

	else if (!secondPlayer->hasMovingPieces())
		return RPSPlayer1WonNoOpponentMovingPieces;

	return RPSSuccess;
}


RPSMessage RPSGame::loadPiecesFromPlayer(Player* player)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++ )
		{
			string currChar = player->getPiece(i,j);

			if (currChar.compare("_") != 0)
			{
				if (currChar.at(0) == 'j' || currChar.at(0) == 'J'){

					movePieceToLocation(i, j ,currChar.at(1), player, true);
				}

				else
					movePieceToLocation(i, j ,currChar.at(0), player);
			}

		}
	}
	if (!player->hasFlags())
		return RPSNoFlags;
	return RPSSuccess;
}
RPSMessage RPSGame::movePieceToLocation(int row, int col, char pieceRep, Player* player, bool joker)
{
	GamePiece* currPieceAtLocation = &(board[row][col]);
	if (currPieceAtLocation->getOwner() == player)
	{
		return RPSTargetLocationContainsPlayerPiece;
	}
	else if (currPieceAtLocation->getOwner() == NULL)
	{

		//delete(currPieceAtLocation);
		GamePiece newPiece(pieceRep, player, joker);
		board[row][col] = newPiece;
	}
	else
	{
		int fightResult = currPieceAtLocation->fight(pieceRep);
		if (fightResult == 0)
		{
			currPieceAtLocation->getOwner()->updatePieceCount(currPieceAtLocation->getCharRep(), false, currPieceAtLocation->pieceIsJoker());
			player->updatePieceCount(pieceRep, false, joker);
			//delete(currPieceAtLocation);
			//currPieceAtLocation->isJoker = false;
			//currPieceAtLocation->owner = NULL;
			//currPieceAtLocation->pieceRep = '_';
			GamePiece newPiece('_', NULL);
			board[row][col] = newPiece;
		}
		else if (fightResult == -1)
		{
			currPieceAtLocation->getOwner()->updatePieceCount(currPieceAtLocation->getCharRep(), false, currPieceAtLocation->pieceIsJoker());
			//delete(currPieceAtLocation);
			if (tolower(pieceRep) == 'b')
			{
				GamePiece newPiece('_', NULL);
				board[row][col] = newPiece;
				player->updatePieceCount('b', false, joker);
			}
			else
			{
				GamePiece newPiece(pieceRep, player, joker);
				board[row][col] = newPiece;
			}


		}
		else
		{
			player->updatePieceCount(pieceRep, false, joker);
			if (tolower(currPieceAtLocation->getCharRep()) == 'b')
			{
				GamePiece newPiece('_', NULL);
				board[row][col] = newPiece;
				if (player == firstPlayer)
					secondPlayer->updatePieceCount('b', false, currPieceAtLocation->pieceIsJoker());
				else
					firstPlayer->updatePieceCount('b', false, currPieceAtLocation->pieceIsJoker());
			}
		}

	}
	return RPSSuccess;
}
bool isNumber(string str)
{
	int length = str.length();
	for (int i = 0; i < length; i++)
	{
		if (!isdigit(str.at(i)))
		{
			return false;
		}
	}
	return true;
}
RPSMessage RPSGame::loadInitalLocationToPlayer(string fileName, int playerNumber, int& lineNumber)
{
	string currentString;
	char piece;
	int X, Y;
	RPSMessage msg;
	ifstream fin(fileName);
	if (!fin)
		return RPSFileFailure;
	Player* currPlayer = playerNumber == 1? firstPlayer:secondPlayer;
	int currLineNumber = 0;
	while (getline(fin, currentString))
	{
		currLineNumber++;
		lineNumber = currLineNumber;
		if (currentString.find_first_not_of("\r\n\t") == string::npos)
			continue;
		istringstream iss(currentString);
		vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());
		int length = results.size();
		if (length < 3 || length > 4)
			return RPSWrongInputLine;
		//format is: J <X> <Y> <PIECE_CHAR>
		if (length == 4)
		{
			if (results[3].length() != 1)
				return RPSWrongInputLine;
			if (results[0].compare("J") != 0)
				return RPSWrongInputLine;
			//checking if piece/X/Y are invalid is done in addInitalPiece
			piece = results[3][0];
			if (isNumber(results[1]))
				X = stoi(results[1]);
			else
				return RPSWrongInputLine;
			if (isNumber(results[2]))
				Y = stoi(results[2]);
			else
				return RPSWrongInputLine;
			//cout << X << " " << Y << piece << endl;
			if (playerNumber == 1)
				msg = currPlayer->addInitialPiece(Y-1, X-1, piece, true);
			else
				msg = currPlayer->addInitialPiece(Y-1, X-1, tolower(piece), true);
			if (msg != RPSSuccess)
			{
				fin.close();
				//printMSG(msg);
				return msg;
			}
		}
		//format is: <PIECE_CHAR> <X> <Y>
		else
		{
			//checking if piece/X/Y are invalid is done in addInitalPiece
			if (results[0].length() != 1)
				return RPSWrongInputLine;
			piece = results[0][0];
			if (isNumber(results[1]))
				X = stoi(results[1]);
			else
				return RPSWrongInputLine;
			if (isNumber(results[2]))
				Y = stoi(results[2]);
			else
				return RPSWrongInputLine;
			//cout << X << " " << Y << piece << endl;
			if (playerNumber == 1)
				msg = currPlayer->addInitialPiece(Y-1, X-1, piece);
			else
				msg = currPlayer->addInitialPiece(Y-1, X-1, tolower(piece));
			if (msg != RPSSuccess)
			{
				fin.close();
				//printMSG(msg);
				return msg;
			}
		}
	}
	fin.close();
	return RPSSuccess;

}


void RPSGame::printGameOutputToFile(string reason, int winner, bool isBadPositioning){
	ofstream fout(out_file_path);
		fout << "Winner: " << winner << endl;
		fout << "Reason: " << reason << endl;
		fout << endl;
		if (!isBadPositioning || isBadPositioning)
			printBoardToFile(fout);
		fout.close();
}


RPSMessage RPSGame::loadAndExecuteMoves(string firstFileName, string secondFileName, int& firstLineNum, int& secondLineNum){

	RPSMessage msg;
	ifstream firstFin(firstFileName);
	if (!firstFin)
		return RPSFileFailure;
	ifstream secondFin(secondFileName);
	if (!secondFin)
		return RPSFileFailure;
	firstLineNum = 1;
	secondLineNum = 1;
	bool isFirstFileDone = false;
	bool isSecondFileDone = false;
	while (!isFirstFileDone || !isSecondFileDone)
	{
		if (!isFirstFileDone)
		{
			msg = loadAndExecuteSingleMove(firstFin, isFirstFileDone, 1);
			if (msg != RPSSuccess)
			{
				firstFin.close();
				secondFin.close();
				return msg;
			}
			firstLineNum++;
		}
		if (isFirstFileDone) { switchCurrentPlayer(); }
		if (!isSecondFileDone)
		{
			msg = loadAndExecuteSingleMove(secondFin, isSecondFileDone, 2);
			if (msg != RPSSuccess)
			{
				firstFin.close();
				secondFin.close();
				return msg;
			}
			secondLineNum++;
		}
		if (isSecondFileDone) { switchCurrentPlayer(); }
	}
	firstFin.close();
	secondFin.close();
	return RPSSuccess;
}

int RPSGame::executeMovesAccordingToPlayersInput(int& winner, string& reason, int& firstLineNum, int& secondLineNum){
	RPSMessage msg = loadAndExecuteMoves(player_one_moves_file_path, player_two_moves_file_path , firstLineNum, secondLineNum);
	if (msg != RPSSuccess)
	{
		if (msg == RPSFileFailure){
			cout << "File failure in one of the moves files - Game couldn't be started" << endl;
			return 0;
		}
		else if (msg == RPSWrongMoveLinePlayer1){
			reason = "Bad moves input file for player 1 - line " + std::to_string(firstLineNum);
			winner = 2;
			printGameOutputToFile(reason , 2, true);
			return 0;
		}
		else if (msg == RPSWrongMoveLinePlayer2){
			reason = "Bad moves input file for player 2 - line " + std::to_string(secondLineNum);
			printGameOutputToFile(reason , 1, true);
			/*if (winner == 2)
				printGameOutputToFile(reason , 0, true);
			else
				printGameOutputToFile(reason , 1, true);*/
			return 0;
		}
		else if (isWinnerRelated(msg)){
			int isEndResult = checkAndPrintWinnerToOutputFile(msg, reason, winner);
			if (isEndResult)
				return 1;
		} else if (isBadPositioning(msg)){
			if (currentPlayer == firstPlayer){
				reason = "Bad Moves input file for player 1 - line " + std::to_string(firstLineNum);
				winner = 2;
				printGameOutputToFile(reason , winner, true);
			}
			else{
				reason = "Bad Moves input file for player 2 - line " + std::to_string(secondLineNum);
				winner = 1;
				printGameOutputToFile(reason , winner, true);
			}
		}
		printMSG(msg);
		//return 0;
	}
	if (winner == 2){
		printGameOutputToFile(reason , 2, true);
		return 0;
	}
	reason = "A tie - both Moves input files done without a winner";
	printGameOutputToFile(reason , 0, false);
	return 1;
}
int RPSGame::initializeGameWithPlayersInput(int& winner, string& reason, int& firstLineNum, int& secondLineNum){
	int init = loadInitialLocationsFromInput(winner, reason, firstLineNum, secondLineNum);
	if (init == 0)
		return 0;
	int checkFlags = checkAndPrintFlagsToOutputFile(winner, reason, firstLineNum, secondLineNum);
	if (checkFlags == 0)
		return 0;
	int loadPieces = loadPiecesFromPlayers(winner, reason);
	if (loadPieces == 0)
		return 0;
	return 1;
}

int RPSGame::loadPiecesFromPlayers(int& winner, string& reason){
	//bool gameOver = false;
	RPSMessage msg = loadPiecesFromPlayer(firstPlayer);
	/*if (isBadPositioning(msg)){
		winner = 2;
		reason = "All flags of Plater 1 are captured in the position files";
		//printMSG(msg);
		cout << reason << endl;
		gameOver = true;
	}
	 */
	msg = loadPiecesFromPlayer(secondPlayer);
	/*if (isBadPositioning(msg)){
		if (winner == 2){
			reason  = "A tie - all flags are eaten by both players in the position files";
			winner = 0;
		}
		else{
			winner = 1 ;
			reason = "All flags of Plater 2 are captured in the position files";
		}
		//printMSG(msg);
		cout << reason << endl;
		printGameOutputToFile(reason , winner, true);
		gameOver = true;
		return 0;
	}
	//check if there is already a winner
	if (gameOver)
		return 0;*/
	msg = checkWinner();
	int checkWinner = checkAndPrintWinnerToOutputFile(msg, reason, winner, true);
	if (checkWinner == 1)
		return 0;
	return 1;
}

int RPSGame::checkAndPrintFlagsToOutputFile(int& winner, string& reason, int& firstLineNum, int& secondLineNum){
	RPSMessage msg = checkPlayersBeforeStart();
	if (msg != RPSSuccess){
		if (msg == RPSBothPlayersNotEnoughFlags){
			reason  = "Bad Positioning input file for both players - player 1: line " +  std::to_string(firstLineNum) +
						", player 2: line " +  std::to_string(secondLineNum);
			winner = 0;
		}
		else if (msg ==  RPSPlayer1NotEnoughFlags){
			reason = "Bad Positioning input file for player 1 - line " + std::to_string(firstLineNum);
			winner = 2;
		} else if (msg ==  RPSPlayer2NotEnoughFlags){
			reason = "Bad Positioning input file for player 2 - line " +  std::to_string(secondLineNum);
			winner = 1;

		}
		cout << reason << endl;
		printGameOutputToFile(reason , winner, true);
		printMSG(msg);
		return 0;
	}
	return 1;
}
int RPSGame::checkAndPrintWinnerToOutputFile(RPSMessage msg, string& reason, int& winner, bool posFiles){
	if (msg != RPSSuccess){
		if (msg == RPSPlayer1WonNoOpponentFlags){
			reason = "All flags of the opponent are captured";
			winner = 1;
		} else if (msg == RPSPlayer1WonNoOpponentMovingPieces){
			reason = "All moving PIECEs of the opponent are eaten";
			winner = 1;
		} else if (msg == RPSPlayer2WonNoOpponentFlags){
			reason = "All flags of the opponent are captured";
			winner = 2;
		} else if (msg == RPSPlayer2WonNoOpponentMovingPieces){
			reason = "All moving PIECEs of the opponent are eaten";
			winner = 2;
		//TODO: in checkWinner we check more cases for tie - Change???
		} else if (msg == RPSTieNoFlags){
			reason = "A tie - all flags are eaten by both players";
			winner = 0;
		}
		else if (msg == RPSTieNoMovingPieces){
			reason = "A tie - no moving pieces left for both players";
			winner = 0;
		}
		if (posFiles)
		{
			reason = reason.append(" in the position files");
		}
		printGameOutputToFile(reason , winner, false);
		return 1;
	}
	return 0;
}

int RPSGame::loadInitialLocationsFromInput(int& winner, string& reason, int& firstLineNum, int& secondLineNum){
	RPSMessage msg = loadInitalLocationToPlayer(player_one_positioning_file_path, 1, firstLineNum);
	if (msg == RPSFileFailure){
		cout << "File failure in Player 1's positioning file - Game couldn't be started" << endl;
		return 0;
	} else if (msg == RPSWrongInputLine || isBadPositioning(msg)){
		winner = 2;
		reason = "Bad Positioning input file for player 1 - line " + std::to_string(firstLineNum);
		printMSG(msg);
		cout << reason << endl;
	}

	msg = loadInitalLocationToPlayer(player_two_positioning_file_path, 2, secondLineNum);
	if (msg == RPSFileFailure){
		cout << "File failure in Player 2's positioning file - Game couldn't be started" << endl;
		return 0;
	} else if (msg == RPSWrongInputLine || isBadPositioning(msg)){
		if (reason.length() == 0){
			winner = 1;
			reason = "Bad Positioning input file for player 2 - line " +  std::to_string(secondLineNum);
		}
		else {
			winner = 0;
			reason = "Bad Positioning input file for both players - player 1: line " +  std::to_string(firstLineNum) +
						", player 2: line " +  std::to_string(secondLineNum);
		}
		printMSG(msg);
		cout << reason << endl;
		printGameOutputToFile(reason, winner, true);
		return 0;
	}
	if (winner == 2){
		printGameOutputToFile(reason, winner, true);
		return 0;
	}
	return 1;
}

RPSMessage RPSGame::loadAndExecuteSingleMove(ifstream& fin, bool& isFileDone, int playerNum)
{
	string currString;
	char jokerChar;
	int fromX, fromY, toX, toY, jokerX, jokerY;
	RPSMessage msg;
	//Player* currPlayer = playerNumber == 1? firstPlayer:secondPlayer;
	//read move line
	getline(fin, currString);
	while (fin && currString.find_first_not_of("\n\r\t") == string::npos)
		getline(fin, currString);
	if (!fin)
		isFileDone = true;
	else
	{
		istringstream iss(currString);
		vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());
		//move fomatt - <FROM_X> <FROM_Y> <TO_X> <TO_Y>
		int length = results.size();
		if (length != 4 && results.size() != 8){
			if (playerNum == 1)
				return RPSWrongMoveLinePlayer1;
			else
				return RPSWrongMoveLinePlayer2;
		} else if (results.size() == 4){
			//move format - <FROM_X> <FROM_Y> <TO_X> <TO_Y>
			if (!isNumber(results[0]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;

			else
				fromX = stoi(results[0]);

			if (!isNumber(results[1]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;
			else
				fromY = stoi(results[1]);

			if (!isNumber(results[2]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;

			else
				toX = stoi(results[2]);

			if (!isNumber(results[3]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;

			else
				toY = stoi(results[3]);

			msg = makePieceMove(fromY-1, fromX-1, toY-1, toX-1);
			//currentPlayer changed -> Need to change back in case we want to make the Joker change
			if (msg != RPSSuccess)
				return msg;
			msg = checkWinner();
			if (msg != RPSSuccess)
				return msg;
		} else if (length == 8){
			//move + joker format - <FROM_X> <FROM_Y> <TO_X> <TO_Y> J: <Joker_X> <Joker_Y> <NEW_REP>
			if (!isNumber(results[0]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;

			else
				fromX = stoi(results[0]);

			if (!isNumber(results[1]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;
			else
				fromY = stoi(results[1]);

			if (!isNumber(results[2]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;

			else
				toX = stoi(results[2]);

			if (!isNumber(results[3]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;

			else
				toY = stoi(results[3]);

			msg = makePieceMove(fromY-1, fromX-1, toY-1, toX-1);
			//currentPlayer changed -> Need to change back in case we want to make the Joker change
			if (msg != RPSSuccess)
				return msg;
			if (results[4].compare("J:") != 0){
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;
			}
			//change back after makemove change
			switchCurrentPlayer();
			if (!isNumber(results[5]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;
			else
				jokerX = stoi(results[5]);

			if (!isNumber(results[6]))
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;

			else
				jokerY = stoi(results[6]);
			if (results[7].length() != 1){
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;
			}
			jokerChar = results[7][0];

			msg = jokerChange(jokerY-1, jokerX-1, jokerChar);
			if (msg != RPSSuccess){
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;
			}
			msg = checkWinner();
			if (msg != RPSSuccess)
				return msg;
		}
		else{
			if (playerNum == 1)
				return RPSWrongMoveLinePlayer1;
			else
				return RPSWrongMoveLinePlayer2;
		}
		/*
		//store the current position
		streampos oldpos = fin.tellg();
		if (!getline(fin, currString))
			isFileDone = true;
		else
		{
			istringstream jokerIiss(currString);
			vector<string> jokerResults(istream_iterator<string>{jokerIiss}, istream_iterator<string>());
			if (jokerResults.size() != 4){
				if (playerNum == 1)
					return RPSWrongMoveLinePlayer1;
				else
					return RPSWrongMoveLinePlayer2;
			}
			if (jokerResults[0].compare("J:") == 0)
			{
				//change back after makemove change
				switchCurrentPlayer();
				//joker fomat - J: <Joker_X> <Joker_Y> <NEW_REP>
				if (!isNumber(jokerResults[1]))
					if (playerNum == 1)
						return RPSWrongMoveLinePlayer1;
					else
						return RPSWrongMoveLinePlayer2;

				else
					jokerX = stoi(jokerResults[1]);

				if (!isNumber(jokerResults[2]))
					if (playerNum == 1)
						return RPSWrongMoveLinePlayer1;
					else
						return RPSWrongMoveLinePlayer2;

				else
					jokerY = stoi(jokerResults[2]);

				jokerChar = jokerResults[3][0];

				msg = jokerChange(jokerY-1, jokerX-1, jokerChar);

				if (msg != RPSSuccess){
					if (playerNum == 1)
						return RPSWrongMoveLinePlayer1;
					else
						return RPSWrongMoveLinePlayer2;
				}

			}
			else
			{
				//go back one line
				fin.seekg (oldpos);
			}
		}
	}
	*/
	}
	return RPSSuccess;
}


RPSMessage RPSGame::makePieceMove(int fromRow, int fromCol, int toRow, int toCol)
{
	if (fromRow >= rows || fromCol >= cols)
	{
		return RPSWrongSourceCoords;
	}
	if (toRow >= rows || toCol >= cols)
	{
		return RPSWrongDestCoords;
	}
	int rowChange = abs(toRow - fromRow);
	int colChange = abs(toCol - fromCol);
	if (rowChange + colChange != 1)
		return RPSInvalidPieceMove;
	GamePiece piece = getPiece(fromRow, fromCol);
	char pieceRep = tolower(piece.getCharRep());
	if (pieceRep == '_')
		return RPSLocationIsEmpty;
	if (piece.getOwner() != currentPlayer)
	{
		return RPSSourceLocationDoesntContainsPlayerPiece;
	}
	if (pieceRep == 'f' || pieceRep == 'b')
	{
		return RPSPieceCannotMove;
	}
	RPSMessage msg = movePieceToLocation(toRow, toCol, piece.getCharRep(), currentPlayer, piece.pieceIsJoker());
	if (msg == RPSSuccess)
	{
		GamePiece newEmptyPiece('_', NULL);
		board[fromRow][fromCol] = newEmptyPiece;
		switchCurrentPlayer();
		//return checkWinner();
	}
	return msg;
}

RPSMessage RPSGame::jokerChange(int row, int col, char newPiece)
{

	if (!(tolower(newPiece) == 'r' || tolower(newPiece) == 'p' || tolower(newPiece) == 's' || tolower(newPiece) == 'b'))
		return RPSWrongPiece;
	if (row >= rows || col >= cols)
	{
		return RPSWrongSourceCoords;
	}
	char pieceRep = tolower(getPiece(row, col).getCharRep());
	if (pieceRep == '_')
			return RPSLocationIsEmpty;
	if (!getPiece(row, col).pieceIsJoker())
			return RPSNotAJokerPiece;
	if (getPiece(row, col).getOwner() != currentPlayer)
	{
		return RPSSourceLocationDoesntContainsPlayerPiece;
	}
	if (pieceRep == 'b')
		getPiece(row, col).getOwner()->updateJokerBombsCount(false);
	getPiece(row, col).changePieceRep(newPiece);
	if (tolower(newPiece) == 'b')
		getPiece(row, col).getOwner()->updateJokerBombsCount(true);
	switchCurrentPlayer();
	return RPSSuccess;
}
RPSMessage RPSGame::checkPlayersBeforeStart()
{
	if (!firstPlayer->hasRightAmountOfFlags() && !secondPlayer->hasRightAmountOfFlags())
		return RPSBothPlayersNotEnoughFlags;
	else if (!firstPlayer->hasRightAmountOfFlags())
		return RPSPlayer1NotEnoughFlags;
	else if (!secondPlayer->hasRightAmountOfFlags())
		return RPSPlayer2NotEnoughFlags;
	else
		return RPSSuccess;
}
void RPSGame::printBoard()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (board[i][j].getCharRep() == '_')
				cout << "_";

			else
			{
				if (board[i][j].pieceIsJoker() ){
					string out = (board[i][j].getOwner() == firstPlayer)? "J": "j";
					cout << out;
				}

				else
					cout << board[i][j].getCharRep();
			}

		}
		cout << endl;
	}

}

void RPSGame::printBoardToFile(ostream& fout){
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (board[i][j].getCharRep() == '_')
				fout << "_";

			else
			{
				if (board[i][j].pieceIsJoker() ){
					string out = (board[i][j].getOwner() == firstPlayer)? "J": "j";
					fout << out;
				}
				else
					fout << board[i][j].getCharRep();
			}
		}
		fout << endl;
	}

}

