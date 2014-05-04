/*
 * Game.h
 *
 *  Created on: 10 лист. 2013
 *      Author: Gasper
 */

#ifndef GAME_H
#define GAME_H

#include "UserInterface.h"
#include "Board.h"
#include "PlayerLocal.h"
#include "PlayerNetwork.h"
#include "Referee.h"
#include "Network.h"
#include "PentagoServer.h"

class Game {
public:
	enum PlayersNum {
		Player1 = 0, Player2 = 1, PlayerBoth=-1
	};
	virtual ~Game();
	static Game* GetInstance();

	//менюшка, тощо
	void Run();

	void SetPlayerName(PlayersNum playerNum, const string& name);
	const Player* GetPlayer(PlayersNum who) const;
	const Player* GetCurrentPlayer() const;
private:
	//а тут вже буде вся гра, аж поки не вийде назад в меню
	void PlayGame();

	Player* players[2];
	Referee referee;
	PentagoServer *server;
	static Game* instance;
	PlayersNum currentPlayer;
	Game();
	Game(const Game&);
public:
	Network network;
	Board board;
	UserInterface userInterface;
	bool mustShutdown;
};

#endif /* GAME_H */
