#include <PentagoLib/Player.h>

Player::Player(std::string name) 
    : player_type { -1 }
{
    setName(name);
}

Player::~Player()
{ }

std::string Player::getName() const
{
    const_cast<std::mutex*>(&player_name_access_mutex)->lock();
    std::string name = this->name;
    const_cast<std::mutex*>(&player_name_access_mutex)->unlock();
    return name;
}

void Player::setName(const std::string name)
{
    player_name_access_mutex.lock();
    this->name = name;
    player_name_access_mutex.unlock();
}

int Player::getPlayerType()
{
    return player_type;
}

void Player::stonePuted(int x, int y)
{
    emit putStone(x, y);
}

void Player::rotated(int direction)
{
    emit rotate(direction);
}

void Player::leaved()
{
    emit leave();
}
