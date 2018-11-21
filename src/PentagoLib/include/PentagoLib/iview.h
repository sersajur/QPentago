#ifndef IVIEW_H
#define IVIEW_H

// TODO: replace with QString
#include <string>

#include <QObject>

using std::string;

class IView: public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    virtual ~IView() = default;

    enum control_setting
    {
        MENU,          /*are enable: New_game(),Load_game(),Join_game(),Host_game()*/
        LOCAL_GAME,    /*are enable: Save_game(),Put_stone(),Rotate(),Leave()*/
        NETWORK_GAME   /*are enable: Put_stone(),Rotate(),Leave()*/
    };
    enum quadrant{I=1,II,III,IV};/* for         */
    enum turn{LEFT, RIGHT};      /*  Rotate()   */
    enum color{BLACK, WHITE};    /* for Draw_stone() */

signals:
    void New_game(int player_count);
    void Save_game(std::string file_name);
    void Load_game(std::string file_name);
    void Join_game(std::string join_connect_data);
    void Host_game(std::string host_connect_data);
    void Put_stone(int row, int col); /*enable Rotate()*/
    void Rotate(IView::quadrant, IView::turn); /*disable Rotate() */
    void Leave();

    void Mesage_sended(std::string);//user type some message

public slots:
    void Set_control_settings(IView::control_setting param) { Do_Set_control_settings(param); }
    void Draw_stone(int row, int col, IView::color c)  { Do_Draw_stone(row, col, c); }
    void Show_message(string text)  { Do_Show_message(text); }

protected:
    virtual void Do_Set_control_settings(control_setting param)=0;/*enable/disable graphic control elements*/
    virtual void Do_Draw_stone(int row, int col, color c)=0;/*draw stone at [row, col] with color*/
    virtual void Do_Show_message(string text)=0;/*print some log text*/
};

#endif // IVIEW_H
