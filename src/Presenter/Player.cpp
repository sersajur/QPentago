/*
 *  Created on: 10 ����. 2013
 *      Author: Gasper
 */
#include "Player.h"

#ifdef DEBUG
#include "Game.h"
#include <iostream>
#include <sstream>
#endif

std::string Player::GetName() const{
	const_cast<mutex*>(&PlayerNameAccessMutex)->lock();
	string name = this->name;
	const_cast<mutex*>(&PlayerNameAccessMutex)->unlock();
	return name;
}

void Player::SetName(const string name) {
	PlayerNameAccessMutex.lock();
#ifdef DEBUG
	Game::Instance()->userInterface.ShowDebugInfo(("Name set: "+name).c_str());
#endif
	this->name = name;
	PlayerNameAccessMutex.unlock();
}

Player::Step Player::MakeStep() {
	//virtual method
	return Player::Step();
}

int Player::GetPlayerType() {
	return playerType;
}

Player::Player(std::string _name) {
	SetName(_name);
	playerType = -1;
}
Player::~Player() { }

