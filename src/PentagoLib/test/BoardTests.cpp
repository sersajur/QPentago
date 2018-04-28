#include <PentagoLib/Board.h>

#include <gtest/gtest.h>


TEST(Board, GetEdgeSize_Return_0_When_0x0)
  {
  const Board::TSize edge_size = 0;

  const auto board = Board{edge_size};

  EXPECT_EQ(edge_size, board.getEdgeSize());
  }

TEST(Board, RowColGetters_Return_7_When_7x7)
  {
  const Board::TSize edge_size = 7;

  const auto board = Board{ edge_size };

  EXPECT_EQ(edge_size, board.getEdgeSize());
  }

TEST(Board, RowColGetters_Return_6_When_6x6)
  {
  const Board::TSize edge_size = 6;

  const auto board = Board{ edge_size };

  EXPECT_EQ(edge_size, board.getEdgeSize());
  }

TEST(Board, PutStone_Returns_True_When_StoneIsPlacedInVacantPlace)
  {
  auto board = Board{6};
  const Board::TIndex row = 2;
  const Board::TIndex col = 3;
  const Board::TStoneId stoneId = 1;

  EXPECT_TRUE(board.putStone(row, col, stoneId));
  }

TEST(Board, PutStone_Returns_False_When_StoneIsPlacedInOccupiedPlace)
  {
  auto board = Board{6};
  const Board::TIndex row = 2;
  const Board::TIndex col = 3;
  const Board::TStoneId stoneId = 1;
  board.putStone(row, col, stoneId);

  EXPECT_FALSE(board.putStone(row, col, stoneId));
  }

TEST(Board, Rotate_Yields_0x0_When_0x0nInLeft)
  {
  auto board = Board{6};
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

TEST(Board, CreatedInstance_Should_BeFilledWithInvalidStoneId)
  {
  const auto board = Board{6};
  const auto expected_id = Board::empty_stone_id;

  for (Board::TIndex i = 0; i < board.getEdgeSize(); ++i)
    {
    for (Board::TIndex j = 0; j < board.getEdgeSize(); ++j)
      {
      EXPECT_EQ(board(i, j), expected_id);
      }
    }
  }
