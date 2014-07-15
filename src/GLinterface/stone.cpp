#include "stone.h"

Stone::Stone(GLint x_left_top,
             GLint y_left_top,
             GLint radius,
             const Texture2D& texture):
    active(false),
    setted(false),
    pressed(false),
    color{1,1,1,1},
    pos(x_left_top,y_left_top,radius*2,radius*2),
    texture(texture) {

}

Stone& Stone::setSize(int width, int height) {
  if (setted)
    pos = decltype(pos) (pos.posX()+pos.width(),pos.posY()+pos.height(),-pos.width(),-pos.height());
  pos.setSize(width,height);
  if (setted)
    pos = decltype(pos) (pos.posX()+pos.width(),pos.posY()+pos.height(),-pos.width(),-pos.height());
  return *this;
}

Stone& Stone::setSetted(bool set) {
  if(setted!=set) {
      pos = decltype(pos) (pos.posX()+pos.width(),pos.posY()+pos.height(),-pos.width(),-pos.height());
    }
  setted=set;
  return *this;
}

Stone& Stone::setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  color[0] = red;
  color[1] = green;
  color[2] = blue;
  color[3] = alpha;
  return *this;
}

const GLfloat* Stone::getColor() const {
  return color;
}

Stone& Stone::setTexture(const Texture2D& txtr) {
  texture = txtr;
  return *this;
}

Stone& Stone::setClickCallBack(const std::function<void()>& call_back) {
  click_call_back = call_back;
  return *this;
}

void Stone::draw() const {
  float alpha_index = active&!setted?0.75:active&setted?1:setted?1:0.9;
  float color_index = active&setted?0.9:1;
  if(active&!setted) {
      glColor4f(0.2,0.2,0.2,1);
    } else {
    glColor4f(
      color[0]*color_index,
      color[1]*color_index,
      color[2]*color_index,
      color[3]*alpha_index*(pressed?0.9:1));
  }
  texture.draw(pos.glCoords(),pos.dimension);
}

#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void Stone::draw(double angle) const {
  if(angle) {
      glPushMatrix();
      glTranslated(pos.posXcenter(),pos.posYcenter(),0);
      glRotated(angle/M_PI*180,0,0,-1);
      glTranslated(-pos.posXcenter(),-pos.posYcenter(),0);
    }
  draw();
  if(angle) {
      glPopMatrix();
    }
}

void Stone::click(int x, int y) {
  (void)x;
  (void)y;
  if(click_call_back) {
      click_call_back();
    }
}

void Stone::mouseDown(int x, int y) {
  (void)x;
  (void)y;
  pressed = true;
}

void Stone::mouseUp(int x, int y) {
  (void)x;
  (void)y;
  if(underMouse(x,y)&&pressed) {
      click(x,y);
    }
  pressed = false;
}

void Stone::hover(int x, int y) {
  (void)x;
  (void)y;
  active = true;
}

void Stone::unHover() {
  active = false;
}

#define SQR(x) (x)*(x)

bool Stone::underMouse(int x, int y) const {
  return SQR(float(pos.posXcenter()-x)/(pos.width()/2.0))+SQR(float(pos.posYcenter()-y)/(pos.height()/2.0))<1;
}

void Stone::setPos(int x, int y) {
  if (setted)
    pos = decltype(pos) (pos.posX()+pos.width(),pos.posY()+pos.height(),-pos.width(),-pos.height());
  pos.setPos(x,y);
  if (setted)
    pos = decltype(pos) (pos.posX()+pos.width(),pos.posY()+pos.height(),-pos.width(),-pos.height());

}

int Stone::posX() const {
  return pos.posX();
}

int Stone::posY() const {
  return pos.posY();
}

int Stone::height() const {
  return pos.height();
}

int Stone::width() const {
  return pos.width();
}
