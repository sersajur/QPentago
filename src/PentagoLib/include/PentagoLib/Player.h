#ifndef PLAYER_H
#define PLAYER_H

#include <mutex>
#include <string>

#include <QObject>

#include <PentagoLib/Board.h>

class Player: public QObject
{
    Q_OBJECT
public:
    Player(std::string name = "Player");
	virtual ~Player();

	std::string getName() const;
	void setName(const std::string);
	int getPlayerType();

protected:
	int player_type;

protected slots:
    // retranslate from View
    virtual void stonePuted(int, int);
    virtual void rotated(int);
    virtual void leaved();

signals:
    // to Presenter
    void putStone(int, int);
    void rotate(int);
    void leave();

private:
	std::string name;
	std::mutex player_name_access_mutex;
};

#endif // PLAYER_H
