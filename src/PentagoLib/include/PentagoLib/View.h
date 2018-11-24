#ifndef VIEW_H
#define VIEW_H

// TODO: replace with QString
#include <string>

#include <QObject>

class View: public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    virtual ~View() = default;

    enum ControlSetting
    {
        Menu,         /* is enabling: newGame(), loadGame(), joinGame(), hostGame() */
        LocalGame,    /* is enabling: saveGame(), putStone(), rotate(), leave() */
        NetworkGame   /* is enabling: putStone(), rotate(), leave( ) */
    };
    /*  for rotate() */
    enum class Quadrant
    {
        I = 1, II, III, IV
    };
    enum class Turn
    {
        Left, Right
    };
    /* for drawStone() */
    enum class Color
    {
        Black, White
    };

signals:
    void newGame(int player_count);
    void saveGame(std::string file_name);
    void loadGame(std::string file_name);
    void joinGame(std::string join_connect_data);
    void hostGame(std::string host_connect_data);
    void putStone(int row, int col);              /* enable rotate() */
    void rotate(View::Quadrant, View::Turn);      /* disable rotate() */
    void leave();

    void messageSended(std::string); // user type some message

public slots:
    void setControlSettings(View::ControlSetting param)
    {
        doSetControlSettings(param);
    }
    void drawStone(int row, int col, View::Color c)
    {
        doDrawStone(row, col, c);
    }
    void showMessage(std::string text)
    {
        doShowMessage(text);
    }

protected:
    virtual void doSetControlSettings(ControlSetting param) = 0; // enable/disable graphic control elements
    virtual void doDrawStone(int row, int col, Color c) = 0; // draw stone at [row, col] with color
    virtual void doShowMessage(std::string text) = 0; // print some log text
};

#endif // VIEW_H
