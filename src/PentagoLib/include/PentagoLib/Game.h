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
using std::shared_ptr;
#include <PentagoLib/Board.h>
#include <PentagoLib/Referee.h>
#include <PentagoLib/Player.h>
#include <PentagoLib/iview.h>
#include "View.h"

class Game : public QObject {
    Q_OBJECT
public:
    shared_ptr<IView> userInterface;
    //Network network;
    //bool mustShutdown;

    virtual ~Game() { }
    static Game& GetInstance();
    void Show() { std::dynamic_pointer_cast<View>(userInterface)->show(); }

    const unique_ptr<Player>& GetPlayer(unsigned who) const;
    const unique_ptr<Player>& GetCurrentPlayer() const;
    //void SetPlayerName(PlayersNum playerNum, const string& name);

public slots:
    // from View (main menu)
    void new_game(int);
    void save_game(std::string);
    void load_game(std::string);
    void join_game(std::string);
    void host_game(std::string);
    // from Player presenter
    void put_stone(int, int);
    void rotate(IView::quadrant, IView::turn);
    void leave();

signals:
    // to View
    void set_control_settings(IView::control_setting);
    void draw_stone(int, int, IView::color);
    void message(string);

private:
    vector<unique_ptr<Player>> players;
    Board board;
    Referee referee;
    unsigned currentPlayer;
    //PentagoServer *server;

    Game();
    Game(const Game&) = delete;
};

#endif /* GAME_H */
