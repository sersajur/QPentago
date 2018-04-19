/*
 * Board.h
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */
#ifndef BOARD_H
#define BOARD_H

#include <PentagoLib/serialization.h>

class Board : public IOriginator {
public:
    enum class RotateDirection { Left, Right };
    enum class Quadrant { I = 1, II, III, IV, V, VI, VII, VIII, IX };

    Board(unsigned _rowCount = 6, unsigned _colCount = 6);
    virtual ~Board() { }
	bool putStone(short, short, short);
    void Rotate(Quadrant, RotateDirection);
	
    const short& operator()(short, short);
    vector<short>& operator[](short);
	
	void Clear();
    unsigned getRowCount() const { return rowCount; }
    unsigned getColCount() const { return colCount; }

    virtual GameState SaveGame();
    virtual void RestoreGame(GameState&);

private:
    typedef vector<vector<short>> TheBoard;
    TheBoard board;
    unsigned rowCount, colCount;
	unsigned stepNum;
};

#endif /* BOARD_H */
