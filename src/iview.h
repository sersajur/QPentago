#ifndef IVIEW_H
#define IVIEW_H

#include <string>
#include <QColor>
using std::string;
class IView
{
public:
    enum control_setting
    {
        MENU,          /*are enable: New_game(),Load_game(),Join_game()*/
        LOCAL_GAME,    /*are enable: Save_game(),Put_stone(),Rotate(),Leave()*/
        NETWORK_GAME   /*are enable: Put_stone(),Rotate(),Leave()*/
    };
    enum quadrant{I=1,II,III,IV};/* for    */
    enum turn{left, right};      /* Rotate() */
public:
    virtual void Set_control_settings(control_setting param)=0;/*enable/disable graphic control elements*/
    virtual void Draw_stone(int row, int col, int color)=0;/*draw stone at [row, col] with color*/
    virtual void Show_message(string text)=0;/*print some log text*/
public:
    virtual void New_game(int player_count)=0;
    virtual void Save_game(string file_name)=0;
    virtual void Load_game(string file_name)=0;
    virtual void Join_game(string connect_data)=0;
    virtual void Put_stone(int row, int col)=0; /*enable Rotate()*/
    virtual void Rotate(quadrant num, turn t)=0;/*disable Rotate() */
    virtual void Leave()=0;
};

#endif // IVIEW_H
