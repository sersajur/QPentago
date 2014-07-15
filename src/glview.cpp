#include "glview.h"

#include <QtOpenGL>
#include <QDebug>
#include <QGLWidget>


#if !defined(HAVE_GLES)
#include <GL/gl.h>
#include "GL/glu.h"
#else
#include <GLES/gl.h>
#endif


#include "GLinterface/button.h"
#include "GLinterface/label.h"
#include "GLinterface/textedit.h"
#include "GLinterface/textures.h"
#include "GLinterface/renderobject.h"
#include "GLinterface/menu.h"
#include "GLinterface/pentagoboard.h"

#include <vector>
#include <stack>

class GLview::GLviewImpl: public QGLWidget, public IView {
public:

  struct Point3D {
    GLdouble x,y,z;
  };

private:
    GLview * parent;

    int  width, height;
    Texture2D texture_menu_background;

    Menu main_menu;
    Menu menu_new_game;
    Menu menu_n_players;

    Menu menu_load_game;
    Menu menu_save_game;

    Menu menu_join_game;
    Menu menu_host_game;

    std::shared_ptr<PentagoBoard> board;

    std::vector<RenderObject*> current_objects;

    std::stack<std::vector<RenderObject*>> view_history;

    int m_x, m_y;//mouse window coordinates
    Point3D m_w;//mouse world coordinates

  #ifdef QT_DEBUG
    float angle;
    int count;
  #endif

    RenderObject* clicked_object;

public:

    GLviewImpl(GLview * gl_parent=0): parent(gl_parent) {
      clicked_object = nullptr;
      setWindowIcon(QIcon(":/window/pentago.ico"));
      setMouseTracking(true);
      QDesktopWidget desktop;
      setGeometry(
        desktop.screenGeometry().right()/2-320,
        desktop.screenGeometry().bottom()/2-240,
        640,480);

#ifndef QT_DEBUG
      setMinimumSize(640,480);
#else //QT_DEBUG
      count = 0;
      angle = 0.2;
#endif //QT_DEBUG

      setFormat(QGLFormat(QGL::DoubleBuffer));
      setAutoBufferSwap(true);
    }

    GLviewImpl(const GLviewImpl&) = delete;
    GLviewImpl& operator=(const GLviewImpl&) = delete;

    GLviewImpl(GLviewImpl&&) = delete;
    GLviewImpl& operator=(GLviewImpl&&) = delete;

    virtual ~GLviewImpl() {
      Button::texture_blurr.release();//kind of bugfix
    }

    void setParent(GLview * gl_parent) {
      parent = gl_parent;
    }


protected:


//QGLWidget:
    virtual void initializeGL() override {
      Texture2D::setContext(this->context());
      qDebug() << "GL version: " << (char*)glGetString(GL_VERSION) << endl;

      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      Button::texture_blurr.load(":/graphics/glass_blurred.jpg");
      texture_menu_background.load(":/graphics/background.jpg");

      buildMenus();

      board.reset(new PentagoBoard(12,12,1000,1000,2));

      board->setStoneSetCallBack([&] (int x, int y) {
          parent->Request_put_stone(x,y);
        });
      board->setRotateCallBack([&](int quadrant_x, int quadrant_y, bool rotate_right){
          static const QUADRANT quadrants[3][3] = {
            {QUADRANT::II,  QUADRANT::I,  QUADRANT::IX},
            {QUADRANT::III, QUADRANT::IV, QUADRANT::VIII},
            {QUADRANT::V,   QUADRANT::VI, QUADRANT::VII}
          };
          parent->Request_rotate_quadrant(quadrants[quadrant_x][quadrant_y],rotate_right?DIRECTION::RIGHT:DIRECTION::LEFT);
        });

    }

    virtual void resizeGL(int w, int h) override {
      width = w;
      height = h;
      glViewport(0,0,w,h);
#if !defined(HAVE_GLES)
      int wh = std::min(w,h);
      int dx = w-wh;
      int dy = h-wh;
      glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho(0, w, h, 0, -1024, 1024 );
        glTranslatef(dx/2.0,dy/2.0,0);
        glScalef(wh/1024.0,wh/1024.0,1);
      glMatrixMode( GL_MODELVIEW );
#else // !defined(HAVE_GLES)
        glOrthof(0, w, h, 0, -1024, 1024 );
#endif // !defined(HAVE_GLES)
    }

    virtual void paintGL() override {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glColor4f(1,1,1,1);
      drawBackground(texture_menu_background);

      for(auto o: current_objects) {
        o->draw();
      }

#ifdef QT_DEBUG
#ifndef HAVE_GLES
      glColor4f(0.15,0.63,0.02,1);
      renderText(20,20,QString("Mouse pos: X:%1 Y:%2").arg(m_x).arg(m_y));
      renderText(20,40,QString("Mouse world pos:"));
      renderText(20,50,QString("X:%1").arg(m_w.x));
      renderText(20,60,QString("Y:%1").arg(m_w.y));
      renderText(20,70,QString("Z:%1").arg(m_w.z));
      renderText(20,80,QString("Press and hold T or press Y %1").arg(count));
#else
      qDebug() << QString("\nMouse pos: X:%1 Y:%2").arg(m_x).arg(m_y);
#endif //HAVE_GLES

#endif //QT_DEBUG
    }
// // QGLWidget

    virtual void mousePressEvent ( QMouseEvent * event ) override {
        mouseCoordTranslate(event->pos().x(),event->pos().y());
        for(auto o: current_objects) {
          if(o->underMouse(m_w.x,m_w.y)) {
            o->mouseDown(m_w.x,m_w.y);
            clicked_object = o;
            break;
          }
        }
        updateGL();
    }

    virtual void mouseReleaseEvent ( QMouseEvent * event ) override {
        mouseCoordTranslate(event->pos().x(),event->pos().y());
        if(clicked_object) {
            clicked_object->mouseUp(m_w.x,m_w.y);
            clicked_object = nullptr;
          } else {
            for(auto o: current_objects) {
                o->mouseUp(m_w.x,m_w.y);
              }
          }
        updateGL();
    }

    virtual void mouseDoubleClickEvent ( QMouseEvent * event ) override {
      (void)event;
    }

    virtual void mouseMoveEvent(QMouseEvent* event) override {
      mouseCoordTranslate(event->pos().x(),event->pos().y());
      if(clicked_object) {
          clicked_object->hover(m_w.x,m_w.y);
        } else {
            for(auto o: current_objects) {
                if(o->underMouse(m_w.x,m_w.y)) {
                    o->hover(m_w.x,m_w.y);
                  } else {
                    o->unHover();
                  }
              }
          }
      updateGL();
    }

    virtual void enterEvent(QEvent * event) override {
      updateGL();
      (void)event;
    }

    virtual void leaveEvent(QEvent * event) override {
      for(auto o: current_objects) {
        o->mouseUp(INT_MAX,INT_MAX);
      }
      updateGL();
      (void)event;
    }



    virtual void keyPressEvent(QKeyEvent * e) override {

      for(auto o: current_objects) {
        if (o->isActive()) {
          o->keyPress(e->key(),e->isAutoRepeat(),KeyboardModifier(int(e->modifiers())));
          if(e->text().length()!=0)
            o->charInput(e->text()[0].unicode());
        }
      }
      glMatrixMode( GL_PROJECTION );
      switch(e->key()) {
    #ifdef QT_DEBUG
        case Qt::Key_T: {
            glRotatef(angle+0.1*(count/10), 1,1,0);
            count++;
            break;
          }
        case Qt::Key_Y: {
            count=0;
            break;
          }
    #endif
        case Qt::Key_A: {
            glRotatef(0.5, 1,0,0);
            break;
          }
        case Qt::Key_Z: {
            glRotatef(0.5, 0,1,0);
            break;
          }
        case Qt::Key_Q: {
            glRotatef(0.5, 0,0,1);
            break;
          }
        case Qt::Key_Return: {
            if (e->modifiers()==Qt::AltModifier) {
                setWindowState(windowState() ^ Qt::WindowFullScreen);
              }
          }
        }
        glMatrixMode( GL_MODELVIEW );
        updateGL();
    }

    virtual void keyReleaseEvent(QKeyEvent * e) override {
        for(auto o: current_objects) {
          if (o->isActive())
            o->keyRelease(e->key(),KeyboardModifier(int(e->modifiers())));
        }
        updateGL();
    }

    void mouseCoordTranslate(int x, int y) {
      m_x = x;
      m_y = y;
      m_w = unProject(x,y);
    }

protected:

    void drawBackground(const Texture2D& texture) {
      glMatrixMode( GL_PROJECTION );
      glPushMatrix();
      glLoadIdentity();
    #if !defined(HAVE_GLES)
      glOrtho(0, width, height, 0, -1, 1 );
    #else
      glOrthof(0, width, height, 0, -1, 1 );
    #endif

      float back_ratio = texture.width()/float(texture.height());
      float window_ratio = width/float(height);
      if(window_ratio>back_ratio) {
        float tmp = (window_ratio/back_ratio*height-height)/2;
        texture.draw({GLint(0)    ,GLint(-tmp)},
                     {GLint(width),GLint(height+tmp)});
      } else {
        float tmp = (back_ratio/window_ratio*width-width)/2;
        texture.draw({GLint(-tmp)     ,GLint(0)},
                     {GLint(width+tmp),GLint(height)});
      }
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
    }

    Point3D unProject(int x, int y) {
#ifndef HAVE_GLES
        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winX, winY, winZ;
        GLdouble posX, posY, posZ;

        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        winX = x;
        winY = (float)viewport[3] - (float)y;
        glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

        gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

        return { posX, posY, posZ };
#else
        (void)x;
        (void)y;
        return {0,0,0};
#endif
    }

    void buildMenus() {
      Texture2D texture_button(":/graphics/button_menu.jpg");
      Texture2D texture_menu(":/graphics/dots.png");
      Texture2D texture_screwed_background(":/graphics/screwed_background.jpg");
      main_menu.setPos(200,150);
      main_menu.setSize(624,724);
      main_menu
          .setTexture(texture_menu)
          .addObject(Button(0,206,512,100,L"New game",texture_button).setClickCallBack(
                       [&]() {
                           goToMenu(menu_new_game);
                       }))
          .addObject(Button(0,316,512,100,L"Load game",texture_button).setClickCallBack(
                       [&]() {
                           parent->Request_get_saves_list();
                           parent->Request_enter_game_layout(GAME_LAYOUT::LOAD_GAME);
                       }))
          .addObject(Button(0,426,512,100,L"Join game",texture_button).setClickCallBack(
                       [&]() {
                           parent->Request_get_hosts_list();
                           parent->Request_enter_game_layout(GAME_LAYOUT::JOIN_GAME);
                       }))
          .addObject(Button(0,536,512,100,L"Host game",texture_button).setClickCallBack(
                       [&]() {
                           goToMenu(menu_host_game);
                       }))
          .addObject(Button(0,746,512,100,L"Exit",texture_button).setClickCallBack(
                       [&]() {
                           this->close();
                         })
          )
          .setKeyCallBack(Qt::Key_Escape,MenuItemClicker(4));

      menu_new_game.setPos(200,260);
      menu_new_game.setSize(624,504);
      menu_new_game
          .setTexture(texture_menu)
          .addObject(Button(0,311,512,100,L"Pentago",texture_button).setClickCallBack(
                       [&](){
                           parent->Request_set_game_mode(GAME_MODE::MODE_PENTAGO);
                           parent->Request_enter_game_layout(GAME_LAYOUT::LOBBY);
                        }))
          .addObject(Button(0,421,512,100,L"Pentago XL",texture_button).setClickCallBack(
                       [&]() {
                           parent->Request_set_game_mode(GAME_MODE::MODE_PENTAGO_XL);
                           goToMenu(menu_n_players);
                        }))
          .addObject(Button(0,631,512,100,L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MenuItemClicker(2));

      menu_n_players.setPos(200,220);
      menu_n_players.setSize(624,585);
      menu_n_players
          .setTexture(texture_menu)
          .addObject(Button(0,271,512,100,L"2 players",texture_button).setClickCallBack(
                       [&]() {
                           parent->Request_show_lobby(2);
                         }))
          .addObject(Button(0,381,512,100,L"3 players",texture_button).setClickCallBack(
                       [&]() {
                           parent->Request_show_lobby(3);
                         }))
          .addObject(Button(0,491,512,100,L"4 players",texture_button).setClickCallBack(
                       [&]() {
                           parent->Request_show_lobby(4);
                         }))
          .addObject(Button(0,672,512,100,L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MenuItemClicker(3));

      menu_load_game.setPos(200,260);
      menu_load_game.setSize(624,504);
      menu_load_game
          .setTexture(texture_menu)
          .addObject(Button(0,311,512,100,L"Autosave",texture_button))
          .addObject(Button(0,631,512,100,L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MenuItemClicker(1));

      menu_save_game.setPos(200,260);
      menu_save_game.setSize(624,504);
      menu_save_game
          .setTexture(texture_menu)
          .addObject(Button(0,311,512,100,L"Autosave",texture_button))
          .addObject(Button(0,631,512,100,L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MenuItemClicker(1));



      menu_join_game.setPos(200,260);
      menu_join_game.setSize(624,504);
      menu_join_game
          .setTexture(texture_menu)
          .addObject(Label(L"Enter Host IP:",0,280, QFont("Snap ITC", 32, 40, false))
                    .setBackground(texture_screwed_background)
                    )
          .addObject(TextEdit(0,360,512,80,texture_button).setMaxTextLength(1024).setText(L"localhost"))
          .addObject(Button(0,450,512,100,L"Connect",texture_button))
          .addObject(Button(0,631,512,100,L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MenuItemClicker(3));

      menu_host_game.setPos(110,150);
      menu_host_game.setSize(804,724);
      menu_host_game
          .setTexture(texture_menu)
          .addObject(Label(L"Enter game name:",0,180,QFont("Snap ITC", 32, 40, false))
                     .setBackground(texture_screwed_background)
                     )
          .addObject(TextEdit(0,250,710,80,texture_button).setMaxTextLength(512).setText(L"New game"))

          .addObject(Button(400,771,400,100,L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MenuItemClicker(0));
    }

    void goMenuBack() {
      if(!view_history.empty()) {
        current_objects = view_history.top();
        view_history.pop();
      }
    }

    void goToMenu(Menu& menu) {
      view_history.push(current_objects);
      current_objects.clear();
      current_objects.push_back(&menu);
    }

    void openLobby() {
      //todo
    }

//    IView: (see iview.h)
public: //some kind of slots
    virtual void Show_game_ended(WINNER winner, const string& winner_name) override {

    }

    virtual void Set_saves_list(const str_array& save_names,const str_array& saves_info) override {

    }

    virtual void Enable_chat() override  {

    }

    virtual void Disable_chat() override  {

    }

    virtual void Set_game_mode(GAME_MODE mode) {

    }

    virtual void Set_game_layout(GAME_LAYOUT layout) override  {
      switch(layout) {
        case GAME_LAYOUT::MAIN_MENU: {
            while(view_history.size()) {
                view_history.pop();
              }
            current_objects.clear();
            current_objects.push_back(&main_menu);
            break;
          }
        case GAME_LAYOUT::SAVE_GAME: {
            goToMenu(menu_save_game);
            break;
          }
        case GAME_LAYOUT::LOAD_GAME: {
            goToMenu(menu_load_game);
            break;
          }
        case GAME_LAYOUT::LOBBY: {
            openLobby();
            break;
          }
        case GAME_LAYOUT::JOIN_GAME: {
            goToMenu(menu_join_game);
            break;
          }
        case GAME_LAYOUT::GAME: {
            while(view_history.size()) {
                view_history.pop();
              }
            current_objects.clear();
            current_objects.push_back(&*board);
            break;
          }
      }
      updateGL();
    }

    virtual void Set_lobby_params(LOBBY_STATUS status, const string& lobby_name = L"", int player_count=-1) override {

    }

    virtual void Set_lobby_player_name(int player_num, const string& name) override {

    }

    virtual void Set_lobby_player_color(int player_num, uint32_t rgb) override {

    }

    virtual void Set_lobby_player_color_charge_enable(int player_num, bool enabled) override {

    }

    virtual void Set_lobby_player_avatar(int player_num, const char* image) override {

    }

    virtual void Set_hosts_list(const str_array& hosts) override  {

    }

    virtual void Clear_board() override {
      qDebug() << "Clear_board emited";
    }

    virtual void Put_stone(int row, int col, uint32_t rgb) override {

    }

    virtual void Rotate_quadrant(QUADRANT quadrant, DIRECTION direction) override {

    }

    virtual void Disable_rotate_quadrant() override {

    }

    virtual void Enable_rotate_quadrant() override {

    }

    virtual void Show_quick_message(string text, MESSAGE_TYPE type = MESSAGE_TYPE::M_INFO, int mili_sec=0) override {

    }

    virtual void Show_message(string text, MESSAGE_BUTTONS buttons = MESSAGE_BUTTONS::OK, MESSAGE_ICON icon=MESSAGE_ICON::I_NONE) override {

    }

    virtual void Hide_message() override {

    }

    virtual void Ask_user_text_input(const string& question, const string& button_accept_text) override {

    }

    virtual void Clear_chat() override {

    }

    virtual void Add_message_to_chat(string from, string text, time_t message_time) override {

    }
};

GLview::GLview(): impl(new GLviewImpl(this)) {
  impl->show();
}


GLview::GLview(GLview&& right): impl(new GLviewImpl(&right)) {
  std::swap(impl, right.impl);
  impl->setParent(this);
}

GLview& GLview::operator=(GLview&&right) {
  std::swap(impl, right.impl);
  impl->setParent(this);
  right.impl->setParent(&right);
  return *this;
}

GLview::~GLview() {
  delete impl;
}

//
// IView:
//
void GLview::Show_game_ended(WINNER winner, const string& winner_name) {
  impl->Show_game_ended(winner,winner_name);
}

void GLview::Set_saves_list(const str_array& save_names,const str_array& saves_info) {
  impl->Set_saves_list(save_names,saves_info);
}

void GLview::Set_game_mode(GAME_MODE mode) {
  impl->Set_game_mode(mode);
}

void GLview::Set_game_layout(GAME_LAYOUT layout) {
  impl->Set_game_layout(layout);
}

void GLview::Set_lobby_params(LOBBY_STATUS status, const string& lobby_name, int player_count) {
  impl->Set_lobby_params(status,lobby_name,player_count);
}

void GLview::Set_lobby_player_name(int player_num, const string& name) {
  impl->Set_lobby_player_name(player_num,name);
}

void GLview::Set_lobby_player_color(int player_num, uint32_t rgb) {
  impl->Set_lobby_player_color(player_num,rgb);
}

void GLview::Set_lobby_player_color_charge_enable(int player_num, bool enabled) {
  impl->Set_lobby_player_color_charge_enable(player_num,enabled);
}

void GLview::Set_lobby_player_avatar(int player_num, const char* image) {
  impl->Set_lobby_player_avatar(player_num,image);
}

void GLview::Set_hosts_list(const str_array& hosts) {
  impl->Set_hosts_list(hosts);
}

void GLview::Clear_board() {
  impl->Clear_board();
}

void GLview::Put_stone(int row, int col, uint32_t rgb) {
  impl->Put_stone(row,col,rgb);
}

void GLview::Rotate_quadrant(QUADRANT quadrant, DIRECTION direction) {
  impl->Rotate_quadrant(quadrant,direction);
}

void GLview::Disable_rotate_quadrant() {
  impl->Disable_rotate_quadrant();
}

void GLview::Enable_rotate_quadrant() {
  impl->Enable_rotate_quadrant();
}

void GLview::Show_quick_message(string text, MESSAGE_TYPE type, int mili_sec) {
  impl->Show_quick_message(text,type,mili_sec);
}

void GLview::Show_message(string text, MESSAGE_BUTTONS buttons, MESSAGE_ICON icon) {
  impl->Show_message(text,buttons,icon);
}

void GLview::Hide_message() {
  impl->Hide_message();
}

void GLview::Ask_user_text_input(const string& question, const string &button_accept_text) {
  impl->Ask_user_text_input(question,button_accept_text);
}

void GLview::Enable_chat() {
  impl->Enable_chat();
}

void GLview::Disable_chat() {
  impl->Disable_chat();
}

void GLview::Clear_chat() {
  impl->Clear_chat();
}

void GLview::Add_message_to_chat(string from, string text, time_t message_time) {
  impl->Add_message_to_chat(from,text,message_time);
}

//old, debug version:
//signals
//void GLview::Request_set_game_mode(GAME_MODE mode) {
//  qDebug() << "Request_set_game_mode: "
//           << "mode ="
//           << (mode==GAME_MODE::MODE_PENTAGO?"PENTAGO":"PENTAGO_XL");
//}

//void GLview::Request_enter_game_layout(GAME_LAYOUT layout) {
//  static const char *layouts[] =
//  {"MAIN_MENU","LOBBY","JOIN_GAME",
//   "SAVE_GAME","LOAD_GAME","GAME"};
//  qDebug() << "Request_enter_game_layout: "
//           << "layout =" << layouts[int(layout)];
//}

//void GLview::Request_show_lobby(int player_count) {
//  qDebug() << "Request_show_lobby: "
//           << "player_count =" << player_count;
//}

//void GLview::Request_lobby_ready() {
//  qDebug() << "Request_lobby_ready";
//}

//void GLview::Request_leave_lobby() {
//  qDebug() << "Request_leave_lobby";
//}

//void GLview::Request_get_saves_list() {
//  qDebug() << "Request_get_saves_list";
//}

//void GLview::Request_save_game(const string& save_name) {
//  qDebug() << "Request_save_game: "
//           << "save_name =" << QString::fromStdWString(save_name);
//}

//void GLview::Request_load_game(const string& save_name) {
//  qDebug() << "Request_load_game: "
//           << "save_name =" << QString::fromStdWString(save_name);
//}

//void GLview::Request_get_hosts_list() {
//  qDebug() << "Request_get_hosts_list";
//}

//void GLview::Request_join_game(const string& host_address) {
//  qDebug() << "Request_join_game: "
//           << "host_address =" << QString::fromStdWString(host_address);
//}

//void GLview::Request_host_game(const string& lobby_name, int player_count, const string& password) {
//  qDebug() << "Request_host_game: "
//           << "name =" <<  QString::fromStdWString(lobby_name) << ' '
//           << "player_count =" << player_count << ' '
//           << "password =" <<  QString::fromStdWString(password);
//}

//void GLview::Request_put_stone(int row, int col) {
//  qDebug() << "Request_put_stone: "
//           << "row =" << row << ' '
//           << "col =" << col;
//}

//void GLview::Request_rotate_quadrant(QUADRANT quadrant, DIRECTION direction) {
//  qDebug() << "Request_rotate_quadrant: "
//           << "quadrant =" << int(quadrant) << ' '
//           << "direction =" << (direction==DIRECTION::RIGHT?"RIGHT":"LEFT");
//}

//void GLview::Request_send_to_chat(const string& message) {
//  qDebug() << "Request_send_to_chat: "
//           << "message =" << QString::fromStdWString(message);
//}

//void GLview::Request_massage_answer(MESSAGE_ANSWER answer) {
//  qDebug() << "Request_massage_answer: "
//           << "answer =" << int(answer);
//}

//void GLview::Request_user_text_input(bool accepted, const string& text) {
//  qDebug() << "Request_user_text_output: "
//           << "accepted =" << accepted
//           << "text =" << QString::fromStdWString(text);
//}

//void GLview::Request_leave_game() {
//  qDebug() << "Request_leave_game";
//}


//void GLview::Requset_change_ivew_to_next() {
//  qDebug() << "Requset_change_ivew_to_next";
//}

