/*
 *  Created on: 10 ����. 2013
 *      Author: Gasper
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <mutex>

using std::string;
using std::mutex;

#include "Board.h"

class Player {
public:
	struct Step {
		short i,j;
		short quarter;
		Board::RotateDirection direction;
	};
	Player(std::string _name = "Player");
	virtual ~Player();
	virtual Player::Step MakeStep();

	string GetName() const;
	void SetName(const string name);
	int GetPlayerType();
protected:
	int playerType;
private:
	string name;
	mutex PlayerNameAccessMutex;
};

#endif /* PLAYER_H */
