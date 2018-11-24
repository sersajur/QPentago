/*
 * Board.cpp
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */
#include <PentagoLib/Board.h>

bool Board::putStone(short row, short column, short player)
{
    if (board[row][column])
    {
        return false;
    }
    board[row][column] = player ? -step_num : step_num;
    if (player)
    {
        step_num++;
    }
    return true;
}

void Board::rotate(Quadrant quadrant, RotateDirection direction)
{
    unsigned i = quadrant == Quadrant::I || quadrant == Quadrant::II || quadrant == Quadrant::IX
        ? 0 : (quadrant == Quadrant::III || quadrant == Quadrant::IV || quadrant == Quadrant::VIII ? 3 : 6);
    unsigned j = quadrant == Quadrant::I || quadrant == Quadrant::IV || quadrant == Quadrant::VI
        ? 3 : (quadrant == Quadrant::IX || quadrant == Quadrant::VIII || quadrant == Quadrant::VII ? 6 : 0);
    short tmp;

    switch (direction)
    {
    case RotateDirection::Left:
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
    case RotateDirection::Right:
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

const short& Board::operator()(short i, short j)
{
    return board[i][j];
}

vector<short>& Board::operator[](short i)
{
    return board[i];
}

void Board::clear()
{
    board = std::move(vector<vector<short>>(row_count, vector<short>(col_count, 0)));
    step_num = 1;
}

Board::Board(unsigned row_count, unsigned col_count) 
    : row_count { row_count }
    , col_count { col_count } 
{
    clear();
}

GameState Board::saveGame()
{
    return GameState(board, step_num);
}

void Board::restoreGame(GameState& gs)
{
    row_count = gs.getRowCount();
    col_count = gs.getColCount();
    clear();
    for (unsigned i = 0; i < row_count; i++)
        for (unsigned j = 0; j < col_count; j++)
            board[i][j] = gs.getBoard()[i][j];
    step_num = gs.getStepNum();
}
