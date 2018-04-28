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
    using TIndex = unsigned;
    using TSize = size_t;

    static const TStoneId empty_stone_id = 0;

    explicit Board(TSize i_edge_size = 6);

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
	
    TStoneId operator()(TIndex i_row, TIndex i_col) const;
    vector<TStoneId>& operator[](TIndex);
	
    void Clear();
    TSize getEdgeSize() const;

    virtual GameState SaveGame();
    virtual void RestoreGame(GameState&);

private:
    typedef vector<vector<short>> TheBoard;
    TheBoard board;
};

#endif /* BOARD_H */
