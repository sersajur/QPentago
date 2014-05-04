/*
 * Referee.h
 *
 *  Created on: 13 лист. 2013
 *      Author: sersajur
 */

#ifndef REFEREE_H_
#define REFEREE_H_

#include"Board.h"
#include<vector>
enum winstatus {
	Second = -1, NoOne = 0, First = 1, Draw = 2
};

struct WinCombinationStruct {
	short beginRow;
	short beginCol;
	short endRow;
	short endCol;
	short PlayerNum; // -1, 1
};

struct ExtWinCombinationStruct {
	winstatus WinStatus;
	std::vector<WinCombinationStruct> winCombinations;
};

class Referee {
private:
	short _rowCount;
	short _colCount;
	short _minLength;
	ExtWinCombinationStruct _winState;

	short _FindCombFromPoint(Board &b, short i, short j, short fix_i = -1, short fix_j = -1,
			short player = 0); //
	void _AddCombination(short, short, short, short, short); //
	short* __GetDirectStep(short di, short dj, short num); //
public:
	Referee(short rowCount = 6, short colCount = 6, short minLength = 5); //
	virtual ~Referee(); //

	winstatus WinnerIs(void); //
	winstatus UpdateWinState(Board b); //
	void ShowCombinationsTemp(void); //
};

#endif /* REFEREE_H_ */
