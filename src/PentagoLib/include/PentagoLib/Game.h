#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>
#include <vector>

#include <QFile>
#include <QObject>

#include <PentagoLib/Board.h>
#include <PentagoLib/Player.h>
#include <PentagoLib/Referee.h>
#include <PentagoLib/View.h>

//#include "Network.h"
//#include "PentagoServer.h"
//#include "PlayerLocal.h"
//#include "PlayerNetwork.h"
//#include "UserInterface.h"

class Game : public QObject {
    Q_OBJECT
public:
    Game(std::shared_ptr<View> user_interface);
    //Network network;
    //bool must_shutdown;

    const std::unique_ptr<Player>& getPlayer(unsigned) const;
    const std::unique_ptr<Player>& getCurrentPlayer() const;
    //void SetPlayerName(PlayersNum player_num, const string& name);

public slots:
    // from View (main menu)
    void newGame(int);
    void saveGame(std::string);
    void loadGame(std::string);
    void joinGame(std::string);
    void hostGame(std::string);
    // from Player presenter
    void putStone(int, int);
    void rotate(View::Quadrant, View::Turn);
    void leave();

signals:
    // to View
    void setControlSettings(View::ControlSetting);
    void drawStone(int, int, View::Color);
    void message(std::string);

private:
    std::shared_ptr<View> user_interface;
    std::vector<std::unique_ptr<Player>> players;
    Board board;
    Referee referee;
    unsigned current_player;
    //PentagoServer *server;
};

#endif // GAME_H
