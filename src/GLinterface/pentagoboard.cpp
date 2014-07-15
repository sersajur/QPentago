#include "pentagoboard.h"
#include "GLRectangleCoord.h"
#include "stone.h"

#include <QDebug>

#include <array>
#include <vector>

#include <cmath>

#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

#ifndef M_2PI
#define M_2PI          6.28318530717958647692  /* 2*pi */
#endif

using std::array;
using std::vector;

class BoardQuadrant: public RenderObject {
public:
  static constexpr unsigned QUADRANT_SIZE           = 3;
  //magic constants...even don't ask
  static constexpr double STONE_SIZE_TO_QUADRANT    = 0.2031*3/QUADRANT_SIZE;
  static constexpr double STONE_SPASE_TO_QUADRANT   = 0.1415*3/QUADRANT_SIZE;
  static constexpr double STONE_SPACE_BEGIN         = 0.0537*3/QUADRANT_SIZE;
  static constexpr double QUADRANTS_SPACE           = 0.0340*3/QUADRANT_SIZE;
  static constexpr double BACKGROUND_SPACE          = 0.1040*3/QUADRANT_SIZE;
private:
  PentagoBoard* parent;

  GLRectangleCoord<GLint> pos;
  GLdouble angle;
  GLint dx,dy;
  int my_pos_x, my_pos_y;
  int active_x, active_y;
  array<array<Stone,QUADRANT_SIZE>,QUADRANT_SIZE> stones;

  struct {
    int x,y;
  } click_pos;

  bool mouse_downed;
  double rotate_angle;

  Stone* clicked_stone;
public:

  BoardQuadrant(PentagoBoard* parent,
                const Texture2D& texture = Texture2D(),
                GLint x_left_top = 0,
                GLint y_left_top = 0,
                GLint width = 0,
                GLint height = 0):
      pos(x_left_top, y_left_top, width, height),
      angle(0), dx(0), dy(0),
      my_pos_x(0), my_pos_y(0),
      active_x(-1), active_y(-1),
      mouse_downed(false),
      clicked_stone(nullptr) {

    (void)texture;
    reposStones();
    setParent(parent);
    for (auto& i: stones) {
        for (auto& o: i) {
            o.setTexture(Texture2D(":/graphics/stone.png"));
          }
      }
  }

  void setPosOnBoard(int pos_x, int pos_y) {
    my_pos_x = pos_x;
    my_pos_y = pos_y;
    setParent(parent);
  }

  void setParent(PentagoBoard* parent) {
    this->parent = parent;
    for (unsigned i=0; i< stones.size(); i++) {
        for(unsigned j=0; j< stones[i].size(); j++) {
            stones[i][j].setClickCallBack([=] () {
                parent->callStoneSetCallBack(
                      my_pos_x*BoardQuadrant::QUADRANT_SIZE+i,
                      my_pos_y*BoardQuadrant::QUADRANT_SIZE+j);
              });
          }
      }
  }

  void setSize(int width, int height) {
    pos.setSize(width, height);
    reposStones();
  }

  void setStone(int x_pos, int y_pos) {
    stones[x_pos][y_pos].setSetted(true);
  }

  void unsetStone(int x_pos, int y_pos) {
    stones[x_pos][y_pos].setSetted(false);
  }

  void  setActiveStone(int pos_x, int pos_y) {
    if(isActive()) {
        stones[active_x][active_y].unHover();
      }
    active_x = pos_x;
    active_y = pos_y;
    if((active_x>=0) & (active_y>=0)) {
        Stone& s = stones[active_x][active_y];
        s.hover(s.posX()+s.width()/2,s.posY()+s.posY()/2);
      }
  }

  void setStoneColor(int x_pos, int y_pos, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    stones[x_pos][y_pos].setColor(red,green,blue,alpha);
  }

  void rotate(bool  right_direction) {
    Stone tmp1 = stones[0][0];
    Stone tmp2 = stones[0][1];

    if(right_direction) {
        stones[0][0] = stones[0][2];
        stones[0][2] = stones[2][2];
        stones[2][2] = stones[2][0];
        stones[2][0] = tmp1;

        stones[0][1] = stones[1][2];
        stones[1][2] = stones[2][1];
        stones[2][1] = stones[1][0];
        stones[1][0] = tmp2;

      } else {
        stones[0][0] = stones[2][0];
        stones[2][0] = stones[2][2];
        stones[2][2] = stones[0][2];
        stones[0][2] = tmp1;


        stones[0][1] = stones[1][0];
        stones[1][0] = stones[2][1];
        stones[2][1] = stones[1][2];
        stones[1][2] = tmp2;
      }
    reposStones();
    setParent(parent);
    for (unsigned i=0; i< stones.size(); i++) {
        for(unsigned j=0; j< stones[i].size(); j++) {
            if (stones[i][j].isActive()) {
                active_x = i;
                active_y = j;
              }
          }
      }
  }

  void unHoverStone() {
    setActiveStone(-1,-1);
  }

  int activeX() const {
    return active_x;
  }

  int activeY() const {
    return active_y;
  }

  void draw() const {
    if(mouse_downed) {
        glPushMatrix();
        glTranslated(pos.posXcenter(),pos.posYcenter(),0);
        glRotated(rotate_angle/M_PI*180,0,0,-1);
        double scale=1-std::abs(sin(2*rotate_angle))*(1-1/sqrt(2));
        glScaled(scale,scale,1);
        glTranslated(-pos.posXcenter(),-pos.posYcenter(),0);
      }
    glColor4f(0.5,0,0,1);
    glBindTexture(GL_TEXTURE_2D,0);
    glVertexPointer(pos.dimension, GL_INT, 0, pos.glCoords());
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    for (auto& i: stones) {
        for (auto& o: i) {
            o.draw(-rotate_angle);
          }
      }
    if(mouse_downed) {
        glPopMatrix();
      }
  }

  virtual void setActive(bool active) override {
    if(!active) {
        unHoverStone();
      }
  }

  void setActive(int stone_x, int stone_y) {
    stones[stone_x][stone_y].setActive(true);
    active_x = stone_x;
    active_y = stone_y;
  }

  virtual bool isActive() const override {
    return (active_x>=0)&(active_y>=0)&((unsigned)active_x<QUADRANT_SIZE)&((unsigned)active_y<QUADRANT_SIZE);
  }

  virtual bool canBeActive() const override {
    return true;
  }

  inline double hypot3(double x, double y, double z) {
    return sqrt(x*x+y*y+z*z);
  }

  inline double pointsAngle(int px1,int py1, int px2, int py2, int px3, int py3) {
    struct pt { int x, y; };
    pt A {px1-px2,py1-py2};
    pt B {px3-px2,py3-py2};
    double x1 = A.x;
    double y1 = A.y;
    double x2 = B.x;
    double y2 = B.y;

    double z1 = 0;
    double z2 = 0;

    double angle_cos = ((x1*x2+y1*y2)/(hypot(x1,y1)*hypot(x2,y2)));
    double angle_sin = (1-2*(x1*y2-x2*y1>0))*(hypot3(x1*y2-x2*y1,x2*z1-z2*x1,y1*z2-y2*z1)/(hypot3(x1,y1,z1)*hypot3(x2,y2,z2)));

//    qDebug() << "\n";
//    qDebug() << " acos(cos(angle)) = " << acos(angle_cos);
//    qDebug() << " asin(sin(angle)) = " << asin(angle_sin);

    double res = angle_cos>0?asin(angle_sin):angle_sin>0?M_PI-asin(angle_sin):-M_PI-asin(angle_sin);
//    qDebug() << " angle = " << res;

//    qDebug() << "A = (" << A.x << "," << A.y << ");";
//    qDebug() << "B = (" << B.x << "," << B.y << ");";
    return (res==NAN||res==INFINITY)?0:res;
  }

  void mouseDown(int x, int y) override {
    for (auto& i: stones) {
        for (auto& o: i) {
            if(o.underMouse(x,y)) {
                o.mouseDown(x,y);
                clicked_stone = &o;
                return;
              }
          }
      }
    click_pos.x = x;
    click_pos.y = y;
    rotate_angle = 0;
    mouse_downed = true;
//    qDebug() << "mouse_downed = " << mouse_downed;
  }

  void mouseUp(int x, int y) override {
    if (clicked_stone) {
        clicked_stone->mouseUp(x,y);
        clicked_stone = nullptr;
      } else if (mouse_downed) {
        if (std::abs(rotate_angle)>M_PI/4) {
            parent->callRotateCallBack(my_pos_x,my_pos_y,rotate_angle<0);
          }
        mouse_downed = false;
        rotate_angle = 0;
        clicked_stone = 0;
      } else {
        for (auto& i: stones) {
            for (auto& o: i) {
                o.mouseUp(x,y);
              }
          }
      }

//   qDebug() << "mouse_downed = " << mouse_downed;
  }

  void hover(int x, int y) override {
    if(mouse_downed) {
        rotate_angle = pointsAngle(click_pos.x,click_pos.y,pos.posXcenter(),pos.posYcenter(),x,y);
        if(rotate_angle>0) {
            rotate_angle = rotate_angle/M_PI*180>88?88.0/180*M_PI:rotate_angle;
          } else {
            rotate_angle = rotate_angle/M_PI*180<-88?-88.0/180*M_PI:rotate_angle;
          }
      } else if (clicked_stone) {
        clicked_stone->hover(x,y);
      } else {
        for (unsigned i=0; i< stones.size(); i++) {
          for(unsigned j=0; j< stones[i].size(); j++) {
            if(active_x!=(int)i || active_y!=(int)j) {
              if (stones[i][j].underMouse(x,y)) {
                  stones[i][j].hover(x,y);
                  if(isActive()) {
                      stones[active_x][active_y].unHover();
                    }
                  active_x = i;
                  active_y = j;
                  return;
                }
              }
            }
          }
      }
  }

  void unHover() override {

  }

  bool underMouse(int x, int y) const override {
    return pos.posInRect(x,y);
  }

  void setPos(int x, int y) override {
    pos.setPos(x,y);
    reposStones();
  }

  int posX() const override {
    return pos.posX();
  }

  int posY() const override {
    return pos.posY();
  }

  int height() const override {
    return pos.height();
  }

  int width() const override {
    return pos.width();
  }

  void keyPress(int key, bool repeat, KeyboardModifier mod) override {
    (void)repeat;
    if (mod == MD_NONE) {
        (void)key;
      }
  }

  void keyRelease(int key, KeyboardModifier mod) override {
    if (mod == MD_NONE) {
        (void)key;
      }
  }

private:
  void reposStones() {
    static const float H = STONE_SIZE_TO_QUADRANT;
    static const float h = STONE_SPASE_TO_QUADRANT;
    static const float hh = STONE_SPACE_BEGIN;

    for (unsigned i=0; i< stones.size(); i++) {
        for(unsigned j=0; j< stones[i].size(); j++) {
            stones[i][j].setPos(pos.posX()+pos.width()*(hh+i*(H+h)),pos.posY()+pos.height()*(hh+j*(H+h)));
            stones[i][j].setSize(pos.width()*H,pos.height()*H);
          }
      }
  }
}; //board quadrant

class PentagoBoardImpl: public RenderObject {
  PentagoBoard *parent;
  GLRectangleCoord<GLint> pos;
  GLRectangleCoord<GLint> background_pos;
  bool active;
  std::function<StoneSetCallBack> set_call_back;
  std::function<RotateCallBack> rotate_call_back;
  vector<vector<BoardQuadrant>> quadrants;

  int active_x, active_y;

  BoardQuadrant* clicked_quadrant;
public:
  PentagoBoardImpl(PentagoBoard *parent,
                   GLint x_left_top = 0,
                   GLint y_left_top = 0,
                   GLint width = 0,
                   GLint height = 0,
                   int board_size = 2):
      parent(parent),
      pos(x_left_top, y_left_top, width, height),
      active(false),
      quadrants(board_size,vector<BoardQuadrant>(board_size,BoardQuadrant(parent))),
      active_x(0), active_y(0),
      clicked_quadrant(nullptr) {
        for (int i=0; i<board_size; i++) {
            for (int j=0; j<board_size; j++) {
                quadrants[i][j].setPosOnBoard(i,j);
              }
          }
        reposQuadrants();
        quadrants[0][0].setActiveStone(0,0);
  }

  void setParent(PentagoBoard *parent) {
    this->parent = parent;
    for(auto& i: quadrants) {
        for(auto& o: i) {
            o.setParent(parent);
          }
      }
  }

  void setSize(int width, int height) {
    pos.setSize(width,height);
    reposQuadrants();
  }

  void setStoneColor(int x_pos, int y_pos, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    quadrants[x_pos/3][y_pos/3].setStoneColor(x_pos%3,y_pos%3,red,green,blue,alpha);
  }

  void setStone(int x_pos, int y_pos) {
    quadrants[x_pos/3][y_pos/3].setStone(x_pos%3,y_pos%3);
  }

  void unsetStone(int x_pos, int y_pos) {
    quadrants[x_pos/3][y_pos/3].unsetStone(x_pos%3,y_pos%3);
  }

  void rotate(int board_x, int board_y, bool  right_direction) {
    quadrants[board_x][board_y].rotate(right_direction);
  }

  void setRotateCallBack(std::function<RotateCallBack> rotate_call_back) {
    this->rotate_call_back = rotate_call_back;
  }

  void callRotateCallBack(int quadrant_x, int quadrant_y, bool rotate_right) const {
    if(rotate_call_back)
      rotate_call_back(quadrant_x,quadrant_y,rotate_right);
  }

  void setStoneSetCallBack(std::function<StoneSetCallBack> stone_set_call_back) {
    this->set_call_back = stone_set_call_back;
  }

  void callStoneSetCallBack(int pos_x, int pos_y) const {
    if(set_call_back) {
        set_call_back(pos_x,pos_y);
      }
  }

  virtual void draw() const override {
    glColor4f(0.7,0.7,0.7,1);
    glBindTexture(GL_TEXTURE_2D,0);
    glVertexPointer(background_pos.dimension, GL_INT, 0, background_pos.glCoords());
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    for(auto& i: quadrants) {
        for(auto& o: i) {
            o.draw();
          }
      }
  }

  virtual void setActive(bool active) override {
    this->active = active;
  }

  virtual bool isActive() const override {
    return active;
  }

  virtual bool canBeActive() const override {
    return true;
  }

  virtual void click(int x, int y) override {
    for(auto& i: quadrants) {
        for(auto& o: i) {
            if(o.underMouse(x,y)) {
                o.click(x,y);
              }
          }
      }
  }

  virtual void mouseDown(int x, int y) override {
    if(underMouse(x,y)) {
        setActive(true);
      }
    for(auto& i: quadrants) {
        for(auto& o: i) {
            if(o.underMouse(x,y)) {
                o.mouseDown(x,y);
                clicked_quadrant = &o;
                return;
              }
          }
      }
  }

  virtual void mouseUp(int x, int y) override {
    if (clicked_quadrant) {
        clicked_quadrant->mouseUp(x,y);
        clicked_quadrant = nullptr;
      } else {
        for(auto& i: quadrants) {
          for(auto& o: i) {
              o.mouseUp(x,y);
            }
          }
      }
  }

  virtual void hover(int x, int y) override {
    if (clicked_quadrant) {
        clicked_quadrant->hover(x,y);
      } else {
        for(unsigned i = 0; i<quadrants.size(); i++) {
            for(unsigned j = 0; j<quadrants[i].size(); j++) {
                if(quadrants[i][j].underMouse(x,y)) {
                    quadrants[i][j].hover(x,y);
                    if(quadrants[i][j].isActive()) {
                        if((active_x!=(int)i)|(active_y!=(int)j)) {
                            quadrants[active_x][active_y].unHoverStone();
                            quadrants[active_x][active_y].unHover();
                            active_x = i;
                            active_y = j;
                            return;
                          }
                      }
                  }
              }
        }
      }
  }

  virtual void unHover() override {
    if (clicked_quadrant) {
        clicked_quadrant->unHover();
        clicked_quadrant = nullptr;
      } else {
          for(auto& i: quadrants) {
              for(auto& o: i) {
                  o.unHover();
                }
            }
        }
  }

  virtual bool underMouse(int x, int y) const override {
    return pos.posInRect(x,y);
  }

  virtual void setPos(int x, int y) override {
    pos.setPos(x,y);
    reposQuadrants();
  }

  virtual int posX() const override {
    return pos.posY();
  }

  virtual int posY() const override {
    return pos.posY();
  }

  virtual int height() const override {
    return pos.height();
  }

  virtual int width() const override {
    return pos.width();
  }

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod) override {
    (void)repeat;
    if(mod == MD_NONE) {
        BoardQuadrant& a_q = quadrants[active_x][active_y];
        switch(key) {
          case Qt::Key_Left: {
              if(a_q.activeX()==0) {
                  if(active_x) {
                      --active_x;
                    } else {
                      active_x = quadrants.size()-1;
                    }
                  quadrants[active_x][active_y].setActiveStone(2,a_q.activeY());
                  a_q.unHoverStone();
                } else {
                  a_q.setActiveStone(a_q.activeX()-1,a_q.activeY());
                }
              break;
            }
          case Qt::Key_Right: {
              if(a_q.activeX()==BoardQuadrant::QUADRANT_SIZE-1) {
                  if(active_x<int(quadrants.size()-1)) {
                      ++active_x;
                    } else {
                      active_x = 0;
                    }
                  quadrants[active_x][active_y].setActiveStone(0,a_q.activeY());
                  a_q.unHoverStone();
                } else {
                  a_q.setActiveStone(a_q.activeX()+1,a_q.activeY());
                }
              break;
            }
          case Qt::Key_Up: {
              if(a_q.activeY()==0) {
                  if(active_y) {
                      --active_y;
                    } else {
                      active_y = quadrants[active_x].size()-1;
                    }
                  quadrants[active_x][active_y].setActiveStone(a_q.activeX(),2);
                  a_q.unHoverStone();
                } else {
                  a_q.setActiveStone(a_q.activeX(),a_q.activeY()-1);
                }
              break;
            }
          case Qt::Key_Down: {
              if(a_q.activeY()==BoardQuadrant::QUADRANT_SIZE-1) {
                  if(active_y<int(quadrants[active_x].size()-1)) {
                      ++active_y;
                    } else {
                      active_y = 0;
                    }
                  quadrants[active_x][active_y].setActiveStone(a_q.activeX(),0);
                  a_q.unHoverStone();
                } else {
                  a_q.setActiveStone(a_q.activeX(),a_q.activeY()+1);
                }
              break;
            }
          }
      }
  }

  virtual void keyRelease(int key, KeyboardModifier mod) override {
    (void)key;
    (void)mod;
  }
private:
  void reposQuadrants() {
    double dx = pos.width()/(1/BoardQuadrant::QUADRANTS_SPACE+(quadrants.size()-1)*(1+1/BoardQuadrant::QUADRANTS_SPACE));
    double w = (pos.width()-dx*(quadrants.size()-1))/quadrants.size();
    double x = pos.posX();

    double dy = pos.height()/(1/BoardQuadrant::QUADRANTS_SPACE+(quadrants[0].size()-1)*(1+1/BoardQuadrant::QUADRANTS_SPACE));
    double h = (pos.height()-dy*(quadrants[0].size()-1))/quadrants[0].size();

    double dw = w*BoardQuadrant::BACKGROUND_SPACE;
    double dh = h*BoardQuadrant::BACKGROUND_SPACE;
    background_pos = decltype(background_pos)
        (pos.posX()+dw,pos.posY()+dh,pos.width()-2*dw,pos.height()-2*dh);

    for(unsigned int i = 0; i<quadrants.size(); i++) {
        double dy = pos.height()/(1/BoardQuadrant::QUADRANTS_SPACE+(quadrants[i].size()-1)*(1+1/BoardQuadrant::QUADRANTS_SPACE));
        double h = (pos.height()-dy*(quadrants[i].size()-1))/quadrants[i].size();
        double y = pos.posY();
        for(unsigned int j = 0; j<quadrants[i].size(); j++) {
            quadrants[i][j].setPos(x,y);
            quadrants[i][j].setSize(std::round(w),std::round(h));
            y+=h+dy;
          }
        x+=w+dx;
      }
  }

};//class PentagoBoardImpl

PentagoBoard::PentagoBoard(GLint x_left_top, GLint y_left_top, GLint width, GLint height, int board_size):
    impl(new PentagoBoardImpl(this,x_left_top,y_left_top,width, height,board_size)) { }

PentagoBoard::~PentagoBoard() {
  delete impl;
}

PentagoBoard::PentagoBoard(const PentagoBoard& rigth) {
  delete impl;
  impl = new PentagoBoardImpl(*rigth.impl);
  impl->setParent(this);
}

PentagoBoard& PentagoBoard::operator=(const PentagoBoard& rigth) {
  delete impl;
  impl = new PentagoBoardImpl(*rigth.impl);
  impl->setParent(this);
  return *this;
}


PentagoBoard& PentagoBoard::setSize(int width, int height) {
  impl->setSize(width, height);
  return *this;
}

PentagoBoard& PentagoBoard::setStone(int x_pos, int y_pos) {
  impl->setStone(x_pos, y_pos);
  return *this;
}

PentagoBoard& PentagoBoard::setStoneColor(int x_pos, int y_pos, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  impl->setStoneColor(x_pos, y_pos,red, green, blue, alpha);
  return *this;
}

PentagoBoard& PentagoBoard::unsetStone(int x_pos, int y_pos) {
  impl->unsetStone(x_pos, y_pos);
  return *this;
}

PentagoBoard& PentagoBoard::rotate(int board_x, int board_y, bool  right_direction) {
  impl->rotate(board_x, board_y, right_direction);
  return *this;
}

PentagoBoard& PentagoBoard::setRotateCallBack(std::function<RotateCallBack> rotate_call_back) {
  impl->setRotateCallBack(rotate_call_back);
  return *this;
}

void PentagoBoard::callRotateCallBack(int quadrant_x, int quadrant_y, bool rotate_right) const {
  impl->callRotateCallBack(quadrant_x,quadrant_y,rotate_right);
}

PentagoBoard& PentagoBoard::setStoneSetCallBack(std::function<StoneSetCallBack> stone_set_call_back) {
  impl->setStoneSetCallBack(stone_set_call_back);
  return *this;
}

void PentagoBoard::callStoneSetCallBack(int pos_x, int pos_y) const {
  impl->callStoneSetCallBack(pos_x,pos_y);
}

void PentagoBoard::draw() const {
  impl->draw();
}

void PentagoBoard::setActive(bool active) {
  impl->setActive(active);
}

bool PentagoBoard::isActive() const {
  return impl->isActive();
}

bool PentagoBoard::canBeActive() const {
  return impl->canBeActive();
}

void PentagoBoard::click(int x, int y) {
  impl->click(x,y);
}

void PentagoBoard::mouseDown(int x, int y) {
  impl->mouseDown(x,y);
}

void PentagoBoard::mouseUp(int x, int y) {
  impl->mouseUp(x,y);
}

void PentagoBoard::hover(int x, int y) {
  impl->hover(x,y);
}

void PentagoBoard::unHover() {
  impl->unHover();
}

bool PentagoBoard::underMouse(int x, int y) const {
  return impl->underMouse(x,y);
}

void PentagoBoard::setPos(int x, int y) {
  impl->setPos(x,y);
}

int  PentagoBoard::posX() const {
  return impl->posX();
}

int  PentagoBoard::posY() const {
  return impl->posY();
}

int  PentagoBoard::height() const {
  return impl->height();
}

int  PentagoBoard::width() const {
  return impl->width();
}

void PentagoBoard::keyPress(int key, bool repeat, KeyboardModifier mod) {
  impl->keyPress(key,repeat,mod);
}

void PentagoBoard::keyRelease(int key, KeyboardModifier mod) {
  impl->keyRelease(key,mod);
}
