#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <string>
#include <mutex>
using std::string;
using std::mutex;

#include "Model/Board.h"

class Player : public QObject {
    Q_OBJECT
public:
    Player(std::string _name = "Player");
	virtual ~Player();

	string GetName() const;
	void SetName(const string name);
	int GetPlayerType();

protected:
	int playerType;

protected slots:
    // retranslate from View
    virtual void stone_puted(int, int);
    virtual void rotated(int);
    virtual void leaved();

signals:
    // to Presenter
    virtual void put_stone(int, int);
    virtual void rotate(int);
    virtual void leave();

private:
	string name;
	mutex PlayerNameAccessMutex;
};

#endif /* PLAYER_H */
