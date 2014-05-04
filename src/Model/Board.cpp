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

void Board::Rotate(short quadrant, RotateDirection direction) {
	unsigned i = quadrant == 0 || quadrant == 1 ? 0 : 3;
	unsigned j = quadrant == 0 || quadrant == 3 ? 3 : 0;
	short tmp;

	switch (direction) {
	case Left:
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
	case Right:
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

short& Board::operator()(short i, short j) {
	return board[i][j];
}

short* Board::operator[](short i) {
	return board[i];
}

void Board::Clear() {
	for (short i = 0; i < 6; i++)
		for (short j = 0; j < 6; j++)
			board[i][j] = 0;
	stepNum = 1;
}

Board::Board() {
	Clear();
}

Board::~Board() {
}
