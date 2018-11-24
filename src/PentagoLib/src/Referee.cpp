/*
 * Referee.cpp
 *
 *  Created on: 13 лист. 2013
 *      Author: sersajur
 */
#include <stdlib.h>
#include <iostream>

#include <PentagoLib/Referee.h>

 // Macro of Signum()
#define SIGNUM(i) (((i)==0)?0:((i)/abs(i)))

// Array of directs (used by findCombFromPoint())
short direct[8][2] = { { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 },
        { 1, -1 }, { 1, 0 }, { 1, 1 } };

//= Constructor
Referee::Referee(short row_count, short col_count, short min_length)
{
    win_state.win_status = WinStatus::NoOne;
    this->row_count = row_count;
    this->col_count = col_count;
    this->min_length = min_length;
}

//= Destructor.
Referee::~Referee()
{ }

//= Return current winner number(first = 1, second = -1), or specific number for "noone is winner (0)" and for the draw (2)
WinStatus Referee::winnerIs(void)
{
    return win_state.win_status;
}

//= Global search function, that initiates recursion search function on the main points (actualy, there are 8 such ones)
WinStatus Referee::updateWinState(Board b)
{
    win_state.win_status = WinStatus::NoOne;
    win_state.win_combinations.clear();

    findCombFromPoint(b, 0, col_count - 1);
    findCombFromPoint(b, row_count - 1, 0);
    for (int i = 0; i < row_count; i++)
    {
        findCombFromPoint(b, i, i);
    }
    return win_state.win_status;
}

//= Used by findCombFromPoint for get relative directs:
short* Referee::getDirectStep(short di, short dj, short num)
{
    short primaryDirectNum = 0;
    for (int i = 0; i < 8; i++)
    {
        if ((direct[i][0] == di) && (direct[i][1] == dj))
        {
            primaryDirectNum = i;
            break;
        }
    }
    if (!num)
    {
        return direct[primaryDirectNum];
    }
    else
    {
        primaryDirectNum = (primaryDirectNum + ((num > 0) ? num : (4 - num))) % 8;
        return direct[primaryDirectNum];
    }
}

//= Recursion "fire on the fluff place"
short Referee::findCombFromPoint(Board &board, short i, short j, short fix_i, short fix_j, short player)
{
    if ((i < 0) || (j < 0) || (i >= row_count) || (j >= col_count)	//out of board
        || (!board[i][j])										//empty tokens place
        || (player * board[i][j] < 0))							//not apropriate token
    {
        return 0;
    }

    short left, right, *ldir, *hdir;
    if (!player)
    { 	//on the first valid iteration
        player = board[i][j] < 0 ? -1 : 1;
        board[i][j] = 0;
        //create direct & save if (sum >= min_length) :
        if ((left = findCombFromPoint(board, i, j - 1, i, j, player))
            + (right = findCombFromPoint(board, i, j + 1, i, j,
                player)) + 1 >= min_length) 							/* _ */
        {
            addCombination(i, j - left, i, j + right, player);
        }
        if ((left = findCombFromPoint(board, i + 1, j, i, j, player))
            + (right = findCombFromPoint(board, i - 1, j, i, j,
                player)) + 1 >= min_length) 							/* | */
        {
            addCombination(i - left, j, i + right, j, player);
        }
        if ((left = findCombFromPoint(board, i + 1, j - 1, i, j,
            player))
            + (right = findCombFromPoint(board, i - 1, j + 1, i, j,
                player)) + 1 >= min_length) 							/* / */
        {
            addCombination(i + left, j - left, i - right, j + right, player);
        }
        if ((left = findCombFromPoint(board, i - 1, j - 1, i, j,
            player))
            + (right = findCombFromPoint(board, i + 1, j + 1, i, j,
                player)) + 1 >= min_length) 							/* \ */
        {
            addCombination(i - left, j - left, i + right, j + right, player);
        }
        return 0;
    }
    else
    {
        board[i][j] = 0;
        //primary direct:
        ldir = getDirectStep(SIGNUM(i - fix_i), SIGNUM(j - fix_j), 0);
        short result = 1 + findCombFromPoint(board, i + ldir[0], j + ldir[1], fix_i, fix_j, player);

        //sub direct:
        for (short dir = 1; dir < 4; dir++)
        {
            ldir = getDirectStep(SIGNUM(i - fix_i), SIGNUM(j - fix_j), dir);
            hdir = getDirectStep(SIGNUM(i - fix_i), SIGNUM(j - fix_j), -dir);
            if ((left = findCombFromPoint(board, i + ldir[0], j + ldir[1], fix_i, fix_j, player))
                + (right = findCombFromPoint(board, fix_i + hdir[0],
                    fix_j + hdir[1], i, j, player)) + 1 >= min_length)
            {
                addCombination(i - left, j - left, i + right, j + right, player);
            }
        }
        return result;
    }
}

//= Write a win combination to the win_state.win_combinations[]
void Referee::addCombination(short bi, short bj, short ei, short ej, short pnum)
{
    WinCombinationStruct tmp = { bi, bj, ei, ej, pnum };
    win_state.win_combinations.push_back(tmp);
    if (win_state.win_status == WinStatus::Draw)
    {
        return;
    }
    win_state.win_status = ((short)(win_state.win_status) == -pnum) ? WinStatus::Draw : (WinStatus) pnum;
    return;
}

//= Display all current win combinations
void Referee::showCombinationsTemp(void)
{
    WinCombinationStruct t;
    for (unsigned i = 0; i < win_state.win_combinations.size(); i++)
    {
        t = win_state.win_combinations[i];
        std::cout << "Board[" << t.begin_row << "][" << t.begin_col << "] - Board["
            << t.end_row << "][" << t.end_col << "]:Player" << ((t.player_num == -1) ? 2 : 1) << std::endl;
    }

}

