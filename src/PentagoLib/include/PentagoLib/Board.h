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

    using TStoneId = short;
    using TIndex = short;
    using TSize = unsigned;

    Board(unsigned _rowCount = 6, unsigned _colCount = 6);
    virtual ~Board() { }

    /**
     * @brief Returns true if stone is set.
     * In case if stone is already set in the given coordinates -
     * returns false.
     */
    bool putStone(TIndex i_row, TIndex i_col, TStoneId i_stone_id);

    /**
     * @brief Rotate quadrant by 90 degree in target direction.
     */
    void Rotate(Quadrant, RotateDirection);
	
    const TStoneId& operator()(TIndex, TIndex);
    vector<TStoneId>& operator[](TIndex);
	
	void Clear();
    TSize getRowCount() const { return rowCount; }
    TSize getColCount() const { return colCount; }

    virtual GameState SaveGame();
    virtual void RestoreGame(GameState&);

private:
    typedef vector<vector<short>> TheBoard;
    TheBoard board;
    unsigned rowCount, colCount;
	unsigned stepNum;
};

#endif /* BOARD_H */
