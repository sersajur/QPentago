/*
 * Referee.h
 *
 *  Created on: 13 лист. 2013
 *      Author: sersajur
 */

#ifndef REFEREE_H
#define REFEREE_H

#include <vector>

#include <PentagoLib/Board.h>

enum class WinStatus
{
    Second = -1, NoOne = 0, First = 1, Draw = 2
};

struct WinCombinationStruct
{
    short begin_row;
    short begin_col;
    short end_row;
    short end_col;
    short player_num; // -1, 1
};

struct ExtWinCombinationStruct
{
    WinStatus win_status;
    std::vector<WinCombinationStruct> win_combinations;
};

class Referee
{
private:
    short row_count;
    short col_count;
    short min_length;
    ExtWinCombinationStruct win_state;

    short findCombFromPoint(Board &b, short i, short j, short fix_i = -1, short fix_j = -1,
        short player = 0); //
    void addCombination(short, short, short, short, short); //
    short* getDirectStep(short, short, short); //
public:
    Referee(short row_count = 6, short col_count = 6, short min_length = 5); //
    virtual ~Referee(); //

    WinStatus winnerIs(void); //
    WinStatus updateWinState(Board); //
    void showCombinationsTemp(void); //
};

#endif // REFEREE_H
