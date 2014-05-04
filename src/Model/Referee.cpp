/*
 * Referee.cpp
 *
 *  Created on: 13 лист. 2013
 *      Author: sersajur
 */
#include "Referee.h"
#include <stdlib.h>
#include <iostream>

//Macro of Signum()
#define SIGNUM(i) (((i)==0)?0:((i)/abs(i)))

// Array of directs (used by _FindCombFromPoint())
short direct[8][2] = { { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 },
		{ 1, -1 }, { 1, 0 }, { 1, 1 } };

//= Constructor
Referee::Referee(short rowCount, short colCount, short minLength) {
	_winState.WinStatus = NoOne;
	this->_rowCount = rowCount;
	this->_colCount = colCount;
	this->_minLength = minLength;
}

//= Destructor.
 Referee::~Referee() {
}
;

//= Return current winner number(first = 1, second = -1), or specific number for "noone is winner (0)" and for the draw (2)
winstatus Referee::WinnerIs(void) {
	return _winState.WinStatus;
}

//= Global search function, that initiates recursion search function on the main points (actualy, there are 8 such ones)
winstatus Referee::UpdateWinState(Board b) {
	_FindCombFromPoint(b, 0, _colCount-1);
	_FindCombFromPoint(b, _rowCount-1, 0);
	for(int i=0;i<_rowCount;i++)
		_FindCombFromPoint(b, i, i);
	return _winState.WinStatus;
}

//= Used by _FindCombFromPoint for get relative directs:
short* Referee::__GetDirectStep(short di, short dj, short num) {
	short primaryDirectNum;
	for (int i = 0; i < 8; i++)
		if ((direct[i][0] == di) && (direct[i][1] == dj)) {
			primaryDirectNum = i;
			break;
		}
	if (!num)
		return direct[primaryDirectNum];
	else {
		primaryDirectNum = (primaryDirectNum + ((num > 0) ? num : (4 - num)))
				% 8;
		return direct[primaryDirectNum];
	}
}

//= Recursion "fire on the fluff place"
short Referee::_FindCombFromPoint(Board &board,
		short i, short j, short fix_i, short fix_j, short player) {

	if ((i < 0) || (j < 0) || (i >= _rowCount) || (j >= _colCount)	//out of board
			|| (!board[i][j])										//empty tokens place
			|| (player * board[i][j] < 0))							//not apropriate token
		return 0;

	short left, right, *ldir, *hdir;
	if (!player) { 								//on the first valid iteration
		player = board[i][j] < 0 ? -1 : 1;
		board[i][j] = 0;
												//create direct & save if (sum >= _minLength) :
		if ((left = _FindCombFromPoint(board, i, j - 1, i, j, player))
				+ (right = _FindCombFromPoint(board, i, j + 1, i, j,
						player)) + 1 >= _minLength) 							/* _ */
			_AddCombination(i, j - left, i, j + right, player);
		if ((left = _FindCombFromPoint(board, i + 1, j, i, j, player))
				+ (right = _FindCombFromPoint(board, i - 1, j, i, j,
						player)) + 1 >= _minLength) 							/* | */
			_AddCombination(i - left, j, i + right, j, player);
		if ((left = _FindCombFromPoint(board, i + 1, j - 1, i, j,
				player))
				+ (right = _FindCombFromPoint(board, i - 1, j + 1, i, j,
						player)) + 1 >= _minLength) 							/* / */
			_AddCombination(i + left, j - left, i - right, j + right, player);
		if ((left = _FindCombFromPoint(board, i - 1, j - 1, i, j,
				player))
				+ (right = _FindCombFromPoint(board, i + 1, j + 1, i, j,
						player)) + 1 >= _minLength) 							/* \ */
			_AddCombination(i - left, j - left, i + right, j + right, player);
		return 0;
	} else {
		board[i][j] = 0;
												//primary direct:
		ldir = __GetDirectStep(SIGNUM(i-fix_i), SIGNUM(j-fix_j), 0);
		short result = 1 + _FindCombFromPoint(board, i + ldir[0], j + ldir[1],  fix_i, fix_j, player);

												//sub direct:
		for (short dir = 1; dir < 4; dir++) {
			ldir = __GetDirectStep(SIGNUM(i-fix_i), SIGNUM(j-fix_j), dir);
			hdir = __GetDirectStep(SIGNUM(i-fix_i), SIGNUM(j-fix_j), -dir);
			if ((left = _FindCombFromPoint(board, i + ldir[0], j + ldir[1], fix_i, fix_j, player))
					+ (right = _FindCombFromPoint(board, fix_i + hdir[0],
							fix_j + hdir[1], i, j, player)) + 1 >= _minLength)
				_AddCombination(i - left, j - left, i + right, j + right,
						player);
		}
		return result;
	}
}

//= Write a win combination to the _winState.winCombinations[]
void Referee::_AddCombination(short bi, short bj, short ei, short ej,
		short pnum) {
	WinCombinationStruct tmp = { bi, bj, ei, ej, pnum };
	_winState.winCombinations.push_back(tmp);
	if (_winState.WinStatus == Draw)
		return;
	_winState.WinStatus =
			(_winState.WinStatus == -pnum) ? Draw : (winstatus) pnum;
	return;
}

//= Display all current win combinations
void Referee::ShowCombinationsTemp(void){
	WinCombinationStruct t;
	for(unsigned i=0;i< _winState.winCombinations.size();i++){
		t=_winState.winCombinations[i];
		std::cout<<"Board["<<t.beginRow<<"]["<<t.beginCol<<"] - Board["
				<<t.endRow<<"]["<<t.endCol<<"]:Player"<<((t.PlayerNum==-1)?2:1)<<std::endl;
	}

}

