#include <PentagoLib/Board.h>

#include <gtest/gtest.h>


TEST(Board, RowColGetters_Return_0x0_When_0x0)
  {
  const size_t row_count = 0;
  const size_t col_count = 0;

  const auto board = Board(row_count, col_count);

  EXPECT_EQ(row_count, board.getRowCount());
  EXPECT_EQ(col_count, board.getColCount());
  }

TEST(Board, RowColGetters_Return_7x0_When_7x0)
  {
  const size_t row_count = 7;
  const size_t col_count = 0;

  const auto board = Board(row_count, col_count);

  EXPECT_EQ(row_count, board.getRowCount());
  EXPECT_EQ(col_count, board.getColCount());
  }

TEST(Board, RowColGetters_Return_0x12_When_0x12)
  {
  const size_t row_count = 0;
  const size_t col_count = 12;

  const auto board = Board(row_count, col_count);

  EXPECT_EQ(row_count, board.getRowCount());
  EXPECT_EQ(col_count, board.getColCount());
  }

TEST(Board, RowColGetters_Return_6x6_When_6x6)
  {
  const size_t row_count = 6;
  const size_t col_count = 6;

  const auto board = Board(row_count, col_count);

  EXPECT_EQ(row_count, board.getRowCount());
  EXPECT_EQ(col_count, board.getColCount());
  }

TEST(Board, RowColGetters_Return_1x9_When_1x9)
  {
  const size_t row_count = 1;
  const size_t col_count = 9;

  const auto board = Board(row_count, col_count);

  EXPECT_EQ(row_count, board.getRowCount());
  EXPECT_EQ(col_count, board.getColCount());
  }

TEST(Board, PutStone_Returns_True_When_StoneIsPlacedInVacantPlace)
  {
  auto board = Board(6, 6);
  const Board::TIndex row = 2;
  const Board::TIndex col = 3;
  const Board::TStoneId stoneId = 1;

  EXPECT_TRUE(board.putStone(row, col, stoneId));
  }

TEST(Board, PutStone_Returns_False_When_StoneIsPlacedInOccupiedPlace)
  {
  auto board = Board(6, 6);
  const Board::TIndex row = 2;
  const Board::TIndex col = 3;
  const Board::TStoneId stoneId = 1;
  board.putStone(row, col, stoneId);

  EXPECT_FALSE(board.putStone(row, col, stoneId));
  }

TEST(Board, Rotate_Yields_0x0_When_0x0nInLeft)
  {
  auto board = Board(6, 6);
  const Board::TIndex row = 0;
  const Board::TIndex col = 0;
  const auto q = Board::Quadrant::I;
  const auto direction = Board::RotateDirection::Left;
  const Board::TStoneId stone_id = 1;
  const Board::TIndex expected_row = 0;
  const Board::TIndex expected_col = 0;

  ASSERT_TRUE(board.putStone(row, col, stone_id));
  ASSERT_NE(board(expected_row, expected_col), stone_id);
  board.Rotate(q, direction);

  EXPECT_EQ(board(expected_row, expected_col), stone_id);
  }
