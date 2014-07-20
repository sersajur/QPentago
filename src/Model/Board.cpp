/*
 * Board.cpp
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */
#include "Board.h"

bool Board::putStone(short row, short column, short player) {
	if (board[row][column])
		return false;
	board[row][column] = player ? -1 * stepNum : stepNum;
	if (player)
		stepNum++;
	return true;
}

void Board::Rotate(Quadrant quadrant, RotateDirection direction) {
    unsigned i = quadrant == Quadrant::I || quadrant == Quadrant::II ? 0 : 3;
    unsigned j = quadrant == Quadrant::I || quadrant == Quadrant::IV ? 3 : 0;
	short tmp;

	switch (direction) {
    case RotateDirection::Left:
		tmp = board[i][j];
		board[i][j] = board[i][j + 2];
		board[i][j + 2] = board[i + 2][j + 2];
		board[i + 2][j + 2] = board[i + 2][j];
		board[i + 2][j] = tmp;
		tmp = board[i][j + 1];
		board[i][j + 1] = board[i + 1][j + 2];
		board[i + 1][j + 2] = board[i + 2][j + 1];
		board[i + 2][j + 1] = board[i + 1][j];
		board[i + 1][j] = tmp;
		break;
    case RotateDirection::Right:
		tmp = board[i][j];
		board[i][j] = board[i + 2][j];
		board[i + 2][j] = board[i + 2][j + 2];
		board[i + 2][j + 2] = board[i][j + 2];
		board[i][j + 2] = tmp;
		tmp = board[i][j + 1];
		board[i][j + 1] = board[i + 1][j];
		board[i + 1][j] = board[i + 2][j + 1];
		board[i + 2][j + 1] = board[i + 1][j + 2];
		board[i + 1][j + 2] = tmp;
		break;
	}
}

const short& Board::operator()(short i, short j) {
	return board[i][j];
}

vector<short>& Board::operator[](short i) {
	return board[i];
}

void Board::Clear() {
    board.clear();
    board.reserve(rowCount);
    for (auto row : board)
        row.reserve(colCount);
    for (unsigned i = 0; i < rowCount; i++)
        for (unsigned j = 0; j < colCount; j++)
			board[i][j] = 0;
	stepNum = 1;
}

Board::Board(unsigned _rowCount, unsigned _colCount) : rowCount{_rowCount}, colCount{_colCount} {
	Clear();
}

GameState Board::SaveGame() { return GameState(board, stepNum); }

void Board::RestoreGame(GameState& gs){
    rowCount = gs.getRowCount();
    colCount = gs.getColCount();
    Clear();
    for (unsigned i = 0; i < rowCount; i++)
        for (unsigned j = 0; j < colCount; j++)
            board[i][j] = gs.getBoard()[i][j];
    stepNum = gs.getStepNum();
}
