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
