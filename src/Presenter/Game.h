#ifndef GAME_H
#define GAME_H

//#include "UserInterface.h"
//#include "PlayerLocal.h"
//#include "PlayerNetwork.h"
//#include "Network.h"
//#include "PentagoServer.h"

#include <QObject>
#include <QFile>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <memory>
using std::unique_ptr;
#include "Model/Board.h"
#include "Model/Referee.h"
#include "Presenter/Player.h"
#include "iview.h"

class Game : public QObject {
    Q_OBJECT
public:
    unique_ptr<IView> userInterface;
    //Network network;
    //bool mustShutdown;

    virtual ~Game() { }
    static Game& GetInstance();

    const unique_ptr<Player>& GetPlayer(unsigned who) const;
    const unique_ptr<Player>& GetCurrentPlayer() const;
    //void SetPlayerName(PlayersNum playerNum, const string& name);

public slots:
    // from View (main menu)
    void new_game(int);
    void save_game(string);
    void load_game(string);
    void join_game(string);
    void host_game();
    // from Player presenter
    void put_stone(int, int);
    void rotate(IView::quadrant, IView::turn);
    void leave();

signals:
    // to View
    void set_control_settings(IView::control_setting);
    void draw_stone(int, int, IView::color);
    void message(QString);

private:
    vector<unique_ptr<Player>> players;
    Board board;
    Referee referee;
    int currentPlayer;
    //PentagoServer *server;

    Game();
    Game(const Game&) = delete;
};

#endif /* GAME_H */
