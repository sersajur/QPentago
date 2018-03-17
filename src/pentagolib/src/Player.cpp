#include "Player.h"

Player::Player(std::string _name) : playerType{-1} { SetName(_name); }
Player::~Player() { }

std::string Player::GetName() const{
	const_cast<mutex*>(&PlayerNameAccessMutex)->lock();
	string name = this->name;
	const_cast<mutex*>(&PlayerNameAccessMutex)->unlock();
	return name;
}

void Player::SetName(const string name) {
	PlayerNameAccessMutex.lock();
	this->name = name;
	PlayerNameAccessMutex.unlock();
}

int Player::GetPlayerType() {
	return playerType;
}

void Player::stone_puted(int x, int y) {
    emit put_stone(x, y);
}

void Player::rotated(int direction){
    emit rotate(direction);
}

void Player::leaved(){
    emit leave();
}
