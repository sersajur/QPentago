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


#include "GLinterface/GLbutton.h"
#include "GLinterface/GLlabel.h"
#include "GLinterface/GLtextedit.h"
#include "GLinterface/GLtextures.h"
#include "GLinterface/GLrenderobject.h"
#include "GLinterface/GLmenu.h"
#include "GLinterface/GLradiogroup.h"

#include "GLinterface/pentagoboard.h"

#include <vector>
#include <stack>
#include <set>
#include <limits>

using std::vector;
using std::stack;
using std::set;

class GLView::GLviewImpl: public QGLWidget, public IView {
public:

private:
    GLView * parent;

    int  width, height;
    GLTexture2D texture_background;
    GLRectangleCoord texture_background_pos;

    GLMenu main_menu;
    GLMenu menu_new_game;
    GLMenu menu_n_players;

    GLMenu menu_load_game;
    GLMenu menu_save_game;

    GLMenu menu_join_game;
    GLMenu menu_host_game;

    PentagoBoard board;

    vector<GLRenderObject*> current_objects;

    stack<vector<GLRenderObject*>> view_history;

    int m_x, m_y;//mouse window coordinates
    WorldPos m_w;//mouse world coordinates

    set<Qt::MouseButton> buttons_pressed;

  #ifdef QT_DEBUG
    float angle;
    int count;
  #endif

    GLRenderObject* clicked_object = nullptr;
    GLRenderObject* hovered_object = nullptr;

public:

    explicit GLviewImpl(GLView * gl_parent=0):
        parent(gl_parent) {

      setWindowIcon(QIcon(":/window/pentago.ico"));
      setMouseTracking(true);
      QDesktopWidget desktop;
      setGeometry(
        desktop.screenGeometry().right()/2-400,
        desktop.screenGeometry().bottom()/2-300,
        800,600);

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
      GLButton::texture_blurr.release();//kind of bugfix
      GLRadioGroup::texture_blurr.release();
    }

    void setParent(GLView * gl_parent) {
      parent = gl_parent;
    }


protected:


//QGLWidget:
    virtual void initializeGL() override {
      GLTexture2D::setContext(this->context());
      qDebug() << "GL version: " << (char*)glGetString(GL_VERSION) << endl;

      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      GLButton::texture_blurr.load(":/graphics/glass_blurred.jpg");
      GLRadioGroup::texture_blurr.load(":/graphics/glass_blurred.jpg");
      texture_background.load(":/graphics/background.jpg");

      buildMenus();

      board.initTextures().setSize({1000,1000}).setPos({12,12});

      board.setStoneSetCallBack([&] (int x, int y) {
          emit parent->Request_put_stone(y,x);
        });
      board.setRotateCallBack([&](int quadrant_x, int quadrant_y, bool rotate_right){
          static const QUADRANT quadrants[3][3] = {
            {QUADRANT::II,  QUADRANT::I,  QUADRANT::IX},
            {QUADRANT::III, QUADRANT::IV, QUADRANT::VIII},
            {QUADRANT::V,   QUADRANT::VI, QUADRANT::VII}
          };
          emit parent->Request_rotate_quadrant(quadrants[quadrant_y][quadrant_x],rotate_right?DIRECTION::RIGHT:DIRECTION::LEFT);
        });

    }

    virtual void resizeGL(int w, int h) override {
      width = w;
      height = h;
      { //background texture coordinates calculation
        WorldPos::COORD_TYPE back_ratio = double(texture_background.width())/texture_background.height();
        WorldPos::COORD_TYPE window_ratio = double(width)/height;
        if(window_ratio>back_ratio) {
            WorldPos::COORD_TYPE tmp = (window_ratio/back_ratio*height-height)/2;
            texture_background_pos.setPos({0,-tmp});
            texture_background_pos.setRight(width);
            texture_background_pos.setBottom(height+tmp);
          } else {
            WorldPos::COORD_TYPE tmp = (back_ratio/window_ratio*width-width)/2;
            texture_background_pos.setPos({-tmp,0});
            texture_background_pos.setRight(width+tmp);
            texture_background_pos.setBottom(height);
          }
      }
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
      drawBackground();

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
//      renderText(20,70,QString("Z:%1").arg(m_w.z));
      renderText(20,80,QString("Press and hold T or press Y %1").arg(count));
#else
      qDebug() << QString("\nMouse pos: X:%1 Y:%2").arg(m_x).arg(m_y);
#endif //HAVE_GLES

#endif //QT_DEBUG
    }
// // QGLWidget

    virtual void mousePressEvent ( QMouseEvent * event ) override {
      if(buttons_pressed.insert(event->button()).second) {
          mouseCoordTranslate(event->pos().x(),event->pos().y());
          for(auto o: current_objects) {
            if(o->underMouse(m_w)) {
              o->mouseDown(MouseEvent{m_w,MouseButton((int)event->button()),MouseButton((int)event->buttons())});
              if(event->button()==Qt::LeftButton) {
                  clicked_object = o;
                }
              break;
            }
          }
        }
      updateGL();
    }

    virtual void mouseReleaseEvent ( QMouseEvent * event ) override {
      if(buttons_pressed.erase(event->button())!=0) {
          mouseCoordTranslate(event->pos().x(),event->pos().y());
          if(clicked_object && event->button()==Qt::LeftButton) {
              clicked_object->mouseUp(MouseEvent{m_w,MouseButton((int)event->button()),MouseButton((int)event->buttons())});
              clicked_object = nullptr;
            } else {
              for(auto o: current_objects) {
                  o->mouseUp(MouseEvent{m_w,MouseButton((int)event->button()),MouseButton((int)event->buttons())});
                }
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
          clicked_object->hover(MouseEvent{m_w,MouseButton((int)event->button()),MouseButton((int)event->buttons())});
        } else {
            for(auto o: current_objects) {
                if(o->underMouse(m_w)) {
                    if(hovered_object) {
                        if(hovered_object!=o) {
                            hovered_object->unHover();
                            o->hover(MouseEvent{m_w,MouseButton((int)event->button()),MouseButton((int)event->buttons())});
                            hovered_object=o;
                          }
                      }
                    o->hover(MouseEvent{m_w,MouseButton((int)event->button()),MouseButton((int)event->buttons())});
                    hovered_object=o;
                  }
              }
          }
      updateGL();
    }

    virtual void enterEvent(QEvent *) override {
      updateGL();
    }

    virtual void leaveEvent(QEvent *) override {
      if(buttons_pressed.size()) {
          MouseButton current_buttons = MOUSE_NO_BUTTONS;
          for(auto i: buttons_pressed) {
              current_buttons=MouseButton(current_buttons|i);
            }
          auto it=buttons_pressed.begin();
          MouseButton btn = MouseButton(*it);
          while(it!=buttons_pressed.end()) {
              it++;
              current_buttons=MouseButton(current_buttons^btn);
              for(auto o: current_objects) {
                  o->mouseUp(MouseEvent{{std::numeric_limits<WorldPos::COORD_TYPE>::max(),
                                         std::numeric_limits<WorldPos::COORD_TYPE>::max()},
                                        btn,current_buttons});
                }
              btn = MouseButton(*it);
            }
        }
      updateGL();
    }



    virtual void keyPressEvent(QKeyEvent * e) override {
      for(auto o: current_objects) {
        if (o->isActive()) {
          bool skip_char_input = false;
          bool ignore = false;
          o->keyPress(e->key(),e->isAutoRepeat(),KeyboardModifier(int(e->modifiers())), skip_char_input, ignore);
          if(e->text().length()!=0 && !skip_char_input)
            o->charInput(e->text()[0].unicode());
        }
      }

#ifdef QT_DEBUG
      glMatrixMode( GL_PROJECTION );
      switch(e->key()) {

        case Qt::Key_T: {
            glRotatef(angle+0.1*(count/10), 1,1,0);
            count++;
            break;
          }
        case Qt::Key_Y: {
            count=0;
            break;
          }
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
#endif //QT_DEBUG
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

    void drawBackground() {
      glMatrixMode( GL_PROJECTION );
      glPushMatrix();
      glLoadIdentity();
    #if !defined(HAVE_GLES)
      glOrtho(0, width, height, 0, -1, 1 );
    #else
      glOrthof(0, width, height, 0, -1, 1 );
    #endif
      texture_background.draw(texture_background_pos.glCoords(),texture_background_pos.dimension);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
    }

    WorldPos unProject(int x, int y) {
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

        return {posX, posY};
#else
        (void)x;
        (void)y;
        return {0,0,0};
#endif
    }

    void buildMenus() {
      GLTexture2D texture_button(":/graphics/background_button.jpg");
      GLTexture2D texture_menu(":/graphics/dots.png");
      GLTexture2D texture_label(":/graphics/background_label.jpg");
      main_menu.setPos({200,150});
      main_menu.setSize({624,724});
      main_menu
          .setTexture(texture_menu)
          .addObject(GLButton({0,206},{512,100},L"New game",texture_button).setClickCallBack(
                       [&]() {
                           goToMenu(menu_new_game);
                       }))
          .addObject(GLButton({0,316},{512,100},L"Load game",texture_button).setClickCallBack(
                       [&]() {
                           emit parent->Request_get_saves_list();
                           emit parent->Request_enter_game_layout(GAME_LAYOUT::LOAD_GAME);
                       }))
          .addObject(GLButton({0,426},{512,100},L"Join game",texture_button).setClickCallBack(
                       [&]() {
                           emit parent->Request_get_hosts_list();
                           emit parent->Request_enter_game_layout(GAME_LAYOUT::JOIN_GAME);
                       }))
          .addObject(GLButton({0,536},{512,100},L"Host game",texture_button).setClickCallBack(
                       [&]() {
                           goToMenu(menu_host_game);
                       }))
          .addObject(GLButton({0,746},{512,100},L"Exit",texture_button).setClickCallBack(
                       [&]() {
                           this->close();
                         })
          )
          .setKeyCallBack(Qt::Key_Escape,MD_NONE,GLMenuItemClicker(4));

      menu_new_game.setPos({200,260});
      menu_new_game.setSize({624,504});
      menu_new_game
          .setTexture(texture_menu)
          .addObject(GLButton({0,311},{512,100},L"Pentago",texture_button).setClickCallBack(
                       [&](){
                           emit parent->Request_set_game_mode(GAME_MODE::MODE_PENTAGO);
                           emit parent->Request_enter_game_layout(GAME_LAYOUT::LOBBY);
                        }))
          .addObject(GLButton({0,421},{512,100},L"Pentago XL",texture_button).setClickCallBack(
                       [&]() {
                           emit parent->Request_set_game_mode(GAME_MODE::MODE_PENTAGO_XL);
                           goToMenu(menu_n_players);
                        }))
          .addObject(GLButton({0,631},{512,100},L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MD_NONE,GLMenuItemClicker(2));

      menu_n_players.setPos({200,220});
      menu_n_players.setSize({624,585});
      menu_n_players
          .setTexture(texture_menu)
          .addObject(GLButton({0,271},{512,100},L"2 players",texture_button).setClickCallBack(
                       [&]() {
                           emit parent->Request_show_lobby(2);
                         }))
          .addObject(GLButton({0,381},{512,100},L"3 players",texture_button).setClickCallBack(
                       [&]() {
                           emit parent->Request_show_lobby(3);
                         }))
          .addObject(GLButton({0,491},{512,100},L"4 players",texture_button).setClickCallBack(
                       [&]() {
                           parent->Request_show_lobby(4);
                         }))
          .addObject(GLButton({0,672},{512,100},L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MD_NONE,GLMenuItemClicker(3));

      menu_load_game.setPos({200,260});
      menu_load_game.setSize({624,504});
      menu_load_game
          .setTexture(texture_menu)
          .addObject(GLButton({0,311},{512,100},L"Autosave",texture_button))
          .addObject(GLButton({0,631},{512,100},L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MD_NONE,GLMenuItemClicker(1));

      menu_save_game.setPos({200,260});
      menu_save_game.setSize({624,504});
      menu_save_game
          .setTexture(texture_menu)
          .addObject(GLButton({0,311},{512,100},L"Autosave",texture_button))
          .addObject(GLButton({0,631},{512,100},L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MD_NONE,GLMenuItemClicker(1));



      menu_join_game.setPos({200,260});
      menu_join_game.setSize({624,504});
      menu_join_game
          .setTexture(texture_menu)
          .addObject(GLLabel(L"Enter Host IP:",{0,280}, QFont("Snap ITC", 32, 40, false))
                    .setBackground(texture_label)
                    )
          .addObject(GLTextEdit({0,360},{512,80},texture_button).setMaxTextLength(1024).setText(L"localhost"))
          .addObject(GLButton({0,450},{512,100},L"Connect",texture_button))
          .addObject(GLButton({0,631},{512,100},L"Back",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MD_NONE,GLMenuItemClicker(3));

      menu_host_game.setPos({110,150});
      menu_host_game.setSize({804,724});
      menu_host_game
          .setTexture(texture_menu)
          .addObject(GLLabel(L"Game name:",{0,180},QFont("Snap ITC", 32, 40, false)).setBackground(texture_label))
          .addObject(GLTextEdit({0,250},{710,80},texture_button).setMaxTextLength(512).setText(L"New game"))
          .addObject(GLLabel(L"Game password:",{0,350},QFont("Snap ITC", 32, 40, false)).setBackground(texture_label))
          .addObject(GLLabel(L"(empty if none)",{0,405},QFont("Snap ITC", 20, 40, false)).setBackground(texture_label))
          .addObject(GLTextEdit({0,455},{710,80},texture_button).setMaxTextLength(512).setText(L""))
          .addObject(GLRadioGroup({134,661},340,texture_button).setFont(QFont("Snap ITC", 32, 40, false))
                     .setItems({L"Pentago", L"Pentago XL",L"test"
                               }).setSelectedIndex(0))
          .addObject(GLButton({490,631},{370,100},L"Start",texture_button))
          .addObject(GLButton({490,741},{370,100},L"Cancel",texture_button).setClickCallBack(
                       [&]() {
                           this->goMenuBack();
                         }))
          .setKeyCallBack(Qt::Key_Escape,MD_NONE,GLMenuItemClicker(7));
    }

    void goMenuBack() {
      if(!view_history.empty()) {
        current_objects = view_history.top();
        view_history.pop();
      }
    }

    void goToMenu(GLMenu& menu) {
      view_history.push(current_objects);
      current_objects.clear();
      current_objects.push_back(&menu);
      menu.setActive(true);
    }

    void openLobby() {
      Set_game_layout(GAME_LAYOUT::GAME);
    }

//    IView: (see iview.h)
public slots:
    virtual void Show_game_ended(WINNER winner, const string& winner_name) override {

    }

    virtual void Set_saves_list(const str_array& save_names,const str_array& saves_info) override {

    }

    virtual void Enable_chat() override  {

    }

    virtual void Disable_chat() override  {

    }

    virtual void Set_game_mode(GAME_MODE mode) {
      switch(mode) {
        case GAME_MODE::MODE_PENTAGO: {
            board.setBoardSize(2);
            break;
          }
        case GAME_MODE::MODE_PENTAGO_XL: {
            board.setBoardSize(3);
            break;
          }
        }
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
            current_objects.push_back(&board);
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
      for(int i=board.getBoardWidth()-1; i>=0; --i) {
          for(int j=board.getBoardWidth()-1; j>=0; --j) {
              board.unsetStone(i,j);
            }
        }
    }

    virtual void Put_stone(int row, int col, uint32_t rgb) override {
      board.setStone(col,row);
      board.setStoneColor(col,row,
                           ((rgb&0xff0000)>>16)/255.0,
                           ((rgb&0x00ff00)>>8)/255.0,
                           (rgb&0x0000ff)/255.0,
                           1);
    }

    virtual void Rotate_quadrant(QUADRANT quadrant, DIRECTION direction) override {
      static const std::pair<int,int> quadrants[] = {
        {1,0},
        {0,0},
        {0,1},
        {1,1},
        {0,2},
        {1,2},
        {2,2},
        {2,1},
        {2,0}
      };
      int x = quadrants[int(quadrant)-1].first;
      int y = quadrants[int(quadrant)-1].second;
      board.rotate(x,y,direction==DIRECTION::RIGHT);
    }

    virtual void Disable_rotate_quadrant() override {
      board.enableRotate(false);
    }

    virtual void Enable_rotate_quadrant() override {
      board.enableRotate(true);
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

GLView::GLView(): impl(new GLviewImpl(this)) {
  setObjectName("OpenGL_view_for_Pentago_game");
  impl->show();
}


GLView::GLView(GLView&& right): impl(new GLviewImpl(&right)) {
  std::swap(impl, right.impl);
  impl->setParent(this);
}

GLView& GLView::operator=(GLView&&right) {
  std::swap(impl, right.impl);
  impl->setParent(this);
  right.impl->setParent(&right);
  return *this;
}

GLView::~GLView() {
  delete impl;
}

//
// IView:
//
void GLView::Show_game_ended(WINNER winner, const string& winner_name) {
  impl->Show_game_ended(winner,winner_name);
}

void GLView::Set_saves_list(const str_array& save_names,const str_array& saves_info) {
  impl->Set_saves_list(save_names,saves_info);
}

void GLView::Set_game_mode(GAME_MODE mode) {
  impl->Set_game_mode(mode);
}

void GLView::Set_game_layout(GAME_LAYOUT layout) {
  impl->Set_game_layout(layout);
}

void GLView::Set_lobby_params(LOBBY_STATUS status, const string& lobby_name, int player_count) {
  impl->Set_lobby_params(status,lobby_name,player_count);
}

void GLView::Set_lobby_player_name(int player_num, const string& name) {
  impl->Set_lobby_player_name(player_num,name);
}

void GLView::Set_lobby_player_color(int player_num, uint32_t rgb) {
  impl->Set_lobby_player_color(player_num,rgb);
}

void GLView::Set_lobby_player_color_charge_enable(int player_num, bool enabled) {
  impl->Set_lobby_player_color_charge_enable(player_num,enabled);
}

void GLView::Set_lobby_player_avatar(int player_num, const char* image) {
  impl->Set_lobby_player_avatar(player_num,image);
}

void GLView::Set_hosts_list(const str_array& hosts) {
  impl->Set_hosts_list(hosts);
}

void GLView::Clear_board() {
  impl->Clear_board();
}

void GLView::Put_stone(int row, int col, uint32_t rgb) {
  impl->Put_stone(row,col,rgb);
}

void GLView::Rotate_quadrant(QUADRANT quadrant, DIRECTION direction) {
  impl->Rotate_quadrant(quadrant,direction);
}

void GLView::Disable_rotate_quadrant() {
  impl->Disable_rotate_quadrant();
}

void GLView::Enable_rotate_quadrant() {
  impl->Enable_rotate_quadrant();
}

void GLView::Show_quick_message(string text, MESSAGE_TYPE type, int mili_sec) {
  impl->Show_quick_message(text,type,mili_sec);
}

void GLView::Show_message(string text, MESSAGE_BUTTONS buttons, MESSAGE_ICON icon) {
  impl->Show_message(text,buttons,icon);
}

void GLView::Hide_message() {
  impl->Hide_message();
}

void GLView::Ask_user_text_input(const string& question, const string &button_accept_text) {
  impl->Ask_user_text_input(question,button_accept_text);
}

void GLView::Enable_chat() {
  impl->Enable_chat();
}

void GLView::Disable_chat() {
  impl->Disable_chat();
}

void GLView::Clear_chat() {
  impl->Clear_chat();
}

void GLView::Add_message_to_chat(string from, string text, time_t message_time) {
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

