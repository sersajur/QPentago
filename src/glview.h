#ifndef GLVIEW_H
#define GLVIEW_H


#include "iview.h"

class GLview : public IView
{
  class GLviewImpl;
  GLviewImpl *impl;//everithing is here
public:

    GLview();

    GLview(GLview&& right);

    GLview& operator=(GLview&&right);

    GLview(const GLview&) = delete;//can not be copied
    GLview& operator=(const GLview&) = delete;//can not be copied

    ~GLview();

//    IView: (see iview.h)
public slots:
    virtual void Show_game_ended(WINNER winner, const string& winner_name) override;

    virtual void Set_saves_list(const str_array& save_names,const str_array& saves_info) override;

    virtual void Enable_chat() override;
    virtual void Disable_chat() override;

    virtual void Set_game_mode(GAME_MODE mode) override;
    virtual void Set_game_layout(GAME_LAYOUT layout) override;

    virtual void Set_lobby_params(LOBBY_STATUS status, const string& lobby_name = L"", int player_count=-1) override;
    virtual void Set_lobby_player_name(int player_num, const string& name) override;
    virtual void Set_lobby_player_color(int player_num, uint32_t rgb) override;
    virtual void Set_lobby_player_color_charge_enable(int player_num, bool enabled) override;
    virtual void Set_lobby_player_avatar(int player_num, const char* image) override;


    virtual void Set_hosts_list(const str_array& hosts) override;

    virtual void Clear_board() override;
    virtual void Put_stone(int row, int col, uint32_t rgb) override;
    virtual void Rotate_quadrant(QUADRANT quadrant, DIRECTION direction) override;

    virtual void Disable_rotate_quadrant() override;
    virtual void Enable_rotate_quadrant() override;

    virtual void Show_quick_message(string text, MESSAGE_TYPE type = MESSAGE_TYPE::M_INFO, int mili_sec=0) override; /*show some text to player during the game*/

    virtual void Show_message(string text, MESSAGE_BUTTONS buttons = MESSAGE_BUTTONS::OK, MESSAGE_ICON icon=MESSAGE_ICON::I_NONE) override;

    virtual void Hide_message() override;

    virtual void Ask_user_text_input(const string& question, const string& button_accept_text) override;

    virtual void Clear_chat() override;
    virtual void Add_message_to_chat(string from, string text, time_t message_time) override;
};

#endif // GLVIEW_H
