/*
 * Board.h
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */
#ifndef BOARD_H
#define BOARD_H

#include <PentagoLib/GameState.h>

class Board: public Originator
{
    typedef vector<vector<short>> TheBoard;

public:
    enum class RotateDirection
    {
        Left, Right
    };
    enum class Quadrant
    {
        I = 1, II, III, IV, V, VI, VII, VIII, IX
    };

    Board(unsigned row_count = 6, unsigned col_count = 6);
    virtual ~Board() 
    { }
    
    bool putStone(short, short, short);
    void rotate(Quadrant, RotateDirection);

    const short& operator()(short, short);
    vector<short>& operator[](short);

    void clear();
    unsigned getRowCount()
    {
        return row_count;
    }
    unsigned getColCount()
    {
        return col_count;
    }

    virtual GameState saveGame();
    virtual void restoreGame(GameState&);

private:
    TheBoard board;
    unsigned row_count, col_count;
    unsigned step_num;
};

#endif // BOARD_H
