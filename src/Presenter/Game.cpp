/*
 * Game.cpp
 *
 *  Created on: 10 ����. 2013
 *      Author: Gasper
 */
#include "Game.h"
#include "Player.h"
#include "PentagoServer.h"

Game* Game::instance = 0;

Game* Game::GetInstance() {
	if (!instance)
		instance = new Game();
	return instance;
}

const Player* Game::GetPlayer(PlayersNum who) const {
	if (who != PlayerBoth)
		return players[who];
	return NULL;
}

void Game::Run() {
	int iResult;
	userInterface = UserInterface(true);
	std::string myName = userInterface.InputPlayerName("your");
	while (!mustShutdown) {
		Network::RemoteAddress param;
		switch (userInterface.MenuDialog()) {
		case UserInterface::ExitGame: {
			return;
		}
		case UserInterface::LocalGame: {
			server = new PentagoServer(PentagoServer::DEFAULT_PORT);
			if (!server->IsGood()) {
				userInterface.Show_CanNotStartServer();
				delete server;
				server = 0;
				continue;
			}
			delete players[Player1];
			delete players[Player2];
			players[Player1] = new PlayerLocal(myName);
			players[Player2] = new PlayerLocal(userInterface.InputPlayerName("player 2"));
			param.addr = "127.0.0.1";
			param.port = PentagoServer::DEFAULT_PORT;
			iResult = network.Connect(&param, players, (char) PlayerBoth);
			if (iResult == 0) {
				PlayGame();
			} else {
				userInterface.Show_CanNotConnect(&param);
			}
			delete server;
			server = 0;
			break;
		}
		case UserInterface::StartHost: {
			server = new PentagoServer(PentagoServer::DEFAULT_PORT);
			if (!server->IsGood()) {
				userInterface.Show_CanNotStartServer();
				delete server;
				server = 0;
				continue;
			}
			delete players[Player1];
			delete players[Player2];
			players[Player1] = new PlayerLocal(myName);
			players[Player2] = new PlayerNetwork("");
			param.addr = "127.0.0.1";
			param.port = PentagoServer::DEFAULT_PORT;
			iResult = network.Connect(&param, players, Player1);
			if (iResult == 0) {
				if (network.WaitForConnection()) {
					PlayGame();
				} else {
					//TODO: complete it
				}
			} else {
				userInterface.Show_CanNotConnect(&param);
			}
			delete server;
			server = 0;
			break;
		}
		case UserInterface::ConnectToServer: {
			if (userInterface.GetHostAddress(&param)) {
				delete players[Player2];
				delete players[Player1];
				players[Player1] = new PlayerNetwork("");
				players[Player2] = new PlayerLocal(myName);
				iResult = network.Connect(&param, players, Player2);
				if (iResult == 0) {
					PlayGame();
				} else {
					userInterface.Show_CanNotConnect(&param);
				}
			}
			break;
		}
		}
	}
}

void Game::SetPlayerName(PlayersNum playerNum, const string& name) {
	if (playerNum != PlayerBoth) {
		players[playerNum]->SetName(name);
	} else {
		players[Player1]->SetName(name);
		players[Player2]->SetName(name);
	}
}
void Game::PlayGame() {
	board.Clear();
	currentPlayer = Player1;
	userInterface.Show_GameBegins();
	userInterface.PaintBoard();
	while ((!mustShutdown) && (referee.UpdateWinState(board) == NoOne)) {
		bool flag;
		Player::Step step;
		do {
			step = players[currentPlayer]->MakeStep();
			if (step.i < 0) {
				userInterface.Show_PlayerDisconnected(players[currentPlayer]);
				break;
			}
			flag = board.putStone(step.i, step.j, currentPlayer);
			if (!flag) {
				userInterface.Show_StepIsNotAllowed();
			}
		} while (!flag);
		if (players[currentPlayer]->GetPlayerType() == 0)
			network.SendPlayerStep(&step);
		if (step.i < 0) {
			break;
		}
		board.Rotate(step.quarter, step.direction);
		currentPlayer = currentPlayer ? Player1 : Player2;

		// displaying the board
		userInterface.PaintBoard();
	}

	userInterface.ShowWinner(referee.WinnerIs());
}

const Player * Game::GetCurrentPlayer() const {
	return players[currentPlayer];
}

Game::Game() {
	players[0] = new Player("Hello");
	players[1] = new Player("world!");
	server = NULL;
	currentPlayer = Player1;
	mustShutdown = false;
}

Game::~Game() {
	delete players[Player1];
	delete players[Player2];
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

