//#include "PentagoServer.h"
#include "Presenter/Game.h"

Game& Game::GetInstance() {
    static Game _instance;
    return _instance;
}

const unique_ptr<Player>& Game::GetPlayer(unsigned who) const {
    if (who < players.size())
        return players[who];
    return unique_ptr<Player>(nullptr);
}

const unique_ptr<Player>& Game::GetCurrentPlayer() const {
	return players[currentPlayer];
}

Game::Game() {
    players.push_back(unique_ptr<Player>(new Player("Hello")));
    players.push_back(unique_ptr<Player>(new Player("world!")));
    // init here
}

void Game::new_game(int){
    board.Clear();
}

void Game::save_game(string filename){
    QFile out(filename.c_str());
    board.SaveGame().Serialize(out);
}

void Game::load_game(string filename){
    QFile in(filename.c_str());
    GameState gs(in);
    board.RestoreGame(gs);
}

void Game::join_game(string){

}

void Game::host_game(){

}

void Game::put_stone(int x, int y){
    // get the sender == currentPlayer
    if (board.putStone(x, y, currentPlayer))
        emit draw_stone(x, y, (IView::color)currentPlayer);
    else
        emit message("This cell is already occupied.");
}

void Game::rotate(IView::quadrant quadrant, IView::turn direction){
    // get the sender == currentPlayer
    board.Rotate((short)quadrant, (Board::RotateDirection)direction);
    if (currentPlayer > players.size()) currentPlayer = 0; else currentPlayer++;
    if (referee.UpdateWinState(board) != NoOne)
        emit message("Win!");
}

void Game::leave(){
    // get the sender == currentPlayer
    if (currentPlayer > players.size()) currentPlayer = 0; else currentPlayer++;
    if (players.size() == 1)
        emit message("Win!");
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

