/*
 * Board.cpp
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */
#include <PentagoLib/Board.h>

#include <cassert>


const Board::TStoneId Board::empty_stone_id;

bool Board::putStone(TIndex row, TIndex column, TStoneId player) {
  assert(player != empty_stone_id);
  
  if (board[row][column] != empty_stone_id)
    {
    return false;
    }
	board[row][column] = player;
	return true;
}

void Board::Rotate(Quadrant quadrant, RotateDirection direction) {
    unsigned i = quadrant == Quadrant::I || quadrant == Quadrant::II || quadrant == Quadrant::IX
            ? 0 : (quadrant == Quadrant::III || quadrant == Quadrant::IV || quadrant == Quadrant::VIII ? 3 : 6);
    unsigned j = quadrant == Quadrant::I || quadrant == Quadrant::IV || quadrant == Quadrant::VI
            ? 3 : (quadrant == Quadrant::IX || quadrant == Quadrant::VIII || quadrant == Quadrant::VII ? 6 : 0);
	short tmp;

	switch (direction) {
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

Board::TStoneId Board::operator()(TIndex i, TIndex j) const {
	return board[i][j];
}

vector<short>& Board::operator[](TIndex i) {
	return board[i];
}

Board::TSize Board::getEdgeSize() const
  {
  return board.size();
  }

void Board::Clear()
{
  for (auto& col : board)
    {
    std::fill(col.begin(), col.end(), empty_stone_id);
    }
}

Board::Board(TSize i_edge_size)
  : board{std::vector<std::vector<TStoneId>>{i_edge_size, std::vector<TStoneId>(i_edge_size, empty_stone_id)}}
{
}

GameState Board::SaveGame() { return GameState(board, -1); }

void Board::RestoreGame(GameState& gs)
  {
  board.clear();
  for (unsigned i = 0; i < gs.getRowCount(); i++)
    {
    std::vector<TStoneId> row;
    for (unsigned j = 0; j < gs.getColCount(); j++)
      row.push_back(gs.getBoard()[i][j]);
    board.emplace_back(std::move(row));
    }
  }
