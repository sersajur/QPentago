#include <PentagoLib/Game.h>
//#include "PentagoServer.h"

Game::Game(std::shared_ptr<View> view)
    : user_interface(std::move(view))
{
    Q_ASSERT(user_interface);

    players.push_back(std::make_unique<Player>("Player 1"));
    players.push_back(std::make_unique<Player>("Player 2"));

    auto v = user_interface.get();

    // connect View to Presenter
    connect(v, SIGNAL(newGame(int)), this, SLOT(newGame(int)));
    connect(v, SIGNAL(saveGame(std::string)), this, SLOT(saveGame(std::string)));
    connect(v, SIGNAL(loadGame(std::string)), this, SLOT(loadGame(std::string)));
    connect(v, SIGNAL(hostGame(std::string)), this, SLOT(hostGame(std::string)));
    connect(v, SIGNAL(joinGame(std::string)), this, SLOT(joinGame(std::string)));
    connect(v, SIGNAL(leave()), this, SLOT(leave()));
    connect(v, SIGNAL(putStone(int, int)), this, SLOT(putStone(int, int)));
    connect(v, SIGNAL(rotate(View::Quadrant, View::Turn)), this, SLOT(rotate(View::Quadrant, View::Turn)));

    // connect Presenter to View
    connect(this, SIGNAL(drawStone(int, int, View::Color)),v,SLOT(drawStone(int, int, View::Color)));
    connect(this, SIGNAL(setControlSettings(View::ControlSetting)), v, SLOT(setControlSettings(View::ControlSetting)));
    connect(this, SIGNAL(message(std::string)), v, SLOT(showMessage(std::string)));
}

const std::unique_ptr<Player>& Game::getPlayer(unsigned who) const
{
    static const std::unique_ptr<Player> null_player(nullptr);
    return who < players.size() ? players[who] : null_player;
}

const std::unique_ptr<Player>& Game::getCurrentPlayer() const
{
    return players[current_player];
}

void Game::newGame(int){
    board.clear();
    current_player = 0;
    emit setControlSettings(View::ControlSetting::LocalGame);
}

void Game::saveGame(std::string filename)
{
    QFile out(filename.c_str());
    board.saveGame().serialize(out);
}

void Game::loadGame(std::string filename){
    QFile in(filename.c_str());
    GameState gs(in);
    board.restoreGame(gs);
    for (unsigned i = 0; i < board.getRowCount(); i++)
    {
        for (unsigned j = 0; j < board.getColCount(); j++)
        {
            if (board[i][j] != 0)
            {
                emit drawStone(i, j, (board[i][j] > 0) ? View::Color::Black : View::Color::White);
            }
        }
    }
    emit setControlSettings(View::ControlSetting::LocalGame);
}

void Game::joinGame(std::string)
{ }

void Game::hostGame(std::string)
{ }

void Game::putStone(int x, int y)
{
    // get the sender == current_player
    if (board.putStone(x, y, current_player))
    {
        emit drawStone(x, y, current_player ? View::Color::Black : View::Color::White);
    }
    else
    {
        emit message("This cell is already occupied.");
    }
}

void Game::rotate(View::Quadrant quadrant, View::Turn direction)
{
    // get the sender == current_player
    board.rotate((Board::Quadrant)quadrant, (direction == View::Turn::Left) ? Board::RotateDirection::Left : Board::RotateDirection::Right);
    if (current_player < decltype(current_player)(players.size() - 1))
    {
        current_player++;
    }
    else
    {
        current_player = 0;
    }
    if (referee.updateWinState(board) != WinStatus::NoOne)
    {
        emit message(players[current_player]->getName() + " Win!");
    }
    /*string tmp = "\n";
    for (int i = 0; i<6; i++)
    {
        for (int j = 0; j<6; j++)
        {
            tmp += std::to_string(board[i][j]) + " ";
        }
        tmp += "\n";
    }
    emit message(tmp);*/
}

void Game::leave()
{
    // get the sender == current_player
    if (current_player > decltype(current_player)(players.size()))
    {
        current_player = 0;
    }
    else 
    {
        current_player++;
    }
    if (players.size() == 1)
    {
        emit message(players[current_player]->getName() + "Win!");
    }
}

/*void Game::TempTestReferee() {
 Referee referee;
 //pull board:
 for (int i = 0; i < 6; i++) {
 board[i][i] = -2;
 board[5][i] = 2;
 }
 //display board:
 board.DisplayTemp();
 //testing Referee methods:
 std::cout << "WinStatus=" << referee.updateWinState(board) << std::endl;
 std::cout << referee.winnerIs() << " is winner (-1=Second, 0=NoOne, 1=First, 2 = Draw)"
 << std::endl;
 referee.showCombinationsTemp();
 return;
 }*/

