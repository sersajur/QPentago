/*
 * Board.h
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#ifndef BOARD_H
#define BOARD_H

class Board {
public:
	enum RotateDirection { Left, Right };

	Board();
	virtual ~Board();
	bool putStone(short, short, short);
	void Rotate(short, RotateDirection);
	
	//TODO: треба зробити один з них const
	short& operator()(short, short);
	short* operator[](short);
	
	void Clear();

private:
	typedef short TheBoard[6][6];
	TheBoard board;
	unsigned stepNum;
};

#endif /* BOARD_H */
