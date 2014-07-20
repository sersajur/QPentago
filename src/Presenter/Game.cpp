//#include "PentagoServer.h"
#include "Presenter/Game.h"

Game& Game::GetInstance() {
    static Game _instance;
    return _instance;
}

Game::Game() {
    players.push_back(unique_ptr<Player>(new Player("Player 1")));
    players.push_back(unique_ptr<Player>(new Player("Player 2")));

    // init View here
    userInterface.reset(new View());
    shared_ptr<View> v = std::dynamic_pointer_cast<View>(userInterface);

    // connect View to Presenter
    connect(v.get(), SIGNAL(New_game(int)), this, SLOT(new_game(int)));
    connect(v.get(), SIGNAL(Save_game(std::string)), this, SLOT(save_game(std::string)));
    connect(v.get(), SIGNAL(Load_game(std::string)), this, SLOT(load_game(std::string)));
    connect(v.get(), SIGNAL(Host_game(std::string)), this, SLOT(host_game(std::string)));
    connect(v.get(), SIGNAL(Join_game(std::string)), this, SLOT(join_game(std::string)));
    connect(v.get(), SIGNAL(Leave()), this, SLOT(leave()));
    connect(v.get(), SIGNAL(Put_stone(int,int)), this, SLOT(put_stone(int,int)));
    connect(v.get(), SIGNAL(Rotate(IView::quadrant,IView::turn)), this, SLOT(rotate(IView::quadrant,IView::turn)));

    // connect Presenter to View
    connect(this, SIGNAL(draw_stone(int,int,IView::color)),v.get(),SLOT(Draw_stone(int,int,IView::color)));
    connect(this, SIGNAL(set_control_settings(IView::control_setting)),v.get(),SLOT(Set_control_settings(IView::control_setting)));
    connect(this, SIGNAL(message(string)), v.get(), SLOT(Show_message(string)));
}

const unique_ptr<Player>& Game::GetPlayer(unsigned who) const {
    static const unique_ptr<Player> null_player(nullptr);
    if (who < players.size())
        return players[who];
    return null_player;
}

const unique_ptr<Player>& Game::GetCurrentPlayer() const {
    return players[currentPlayer];
}

void Game::new_game(int){
    board.Clear();
    currentPlayer = 0;
    emit set_control_settings(IView::control_setting::LOCAL_GAME);
}

void Game::save_game(string filename){
    QFile out(filename.c_str());
    board.SaveGame().Serialize(out);
}

void Game::load_game(string filename){
    QFile in(filename.c_str());
    GameState gs(in);
    board.RestoreGame(gs);
    for (int i = 0; i<6;i++)
        for (int j = 0; j<6; j++)
            if (board[i][j]!=0)
                emit draw_stone(i, j, (board[i][j] > 0) ? IView::BLACK : IView::WHITE);
    emit set_control_settings(IView::control_setting::LOCAL_GAME);
}

void Game::join_game(string){

}

void Game::host_game(string){

}

void Game::put_stone(int x, int y){
    // get the sender == currentPlayer
    if (board.putStone(x, y, currentPlayer))
        emit draw_stone(x, y, currentPlayer ? IView::BLACK : IView::WHITE);
    else
        emit message("This cell is already occupied.");
}

void Game::rotate(IView::quadrant quadrant, IView::turn direction){
    // get the sender == currentPlayer
    board.Rotate((Board::Quadrant)quadrant, (direction == IView::LEFT) ? Board::RotateDirection::Left : Board::RotateDirection::Right);
    if (currentPlayer < decltype(currentPlayer)(players.size()-1))
        currentPlayer++;
    else
        currentPlayer = 0;
    if (referee.UpdateWinState(board) != NoOne)
        emit message(players[currentPlayer]->GetName() + " Win!");

    /*string tmp = "\n";
    for (int i = 0; i<6;i++){
        for (int j = 0; j<6; j++)
            tmp += std::to_string(board[i][j]) + " ";
        tmp += "\n";
    }
    emit message(tmp);*/
}

void Game::leave(){
    // get the sender == currentPlayer
    if (currentPlayer > decltype(currentPlayer)(players.size()))
        currentPlayer = 0;
    else
        currentPlayer++;
    if (players.size() == 1)
        emit message(players[currentPlayer]->GetName() + "Win!");
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
 std::cout << "WinStatus=" << referee.UpdateWinState(board) << std::endl;
 std::cout << referee.WinnerIs() << " is winner (-1=Second, 0=NoOne, 1=First, 2 = Draw)"
 << std::endl;
 referee.ShowCombinationsTemp();
 return;
 }*/

