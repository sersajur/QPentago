#include "stone.h"

Stone::Stone(const WorldPos &pos_left_top,
             WorldPos::COORD_TYPE radius,
             const GLTexture2D& texture):
    active(false),
    setted(false),
    pressed(false),
    color{1,1,1,1},
    pos(pos_left_top,{radius*2,radius*2}),
    texture(texture) {

}

Stone& Stone::setSize(const WorldPos &v_size) {
  if (setted)
    pos = decltype(pos) ({pos.posX()+pos.width(),pos.posY()+pos.height()},{-pos.width(),-pos.height()});
  pos.setSize(v_size);
  if (setted)
    pos = decltype(pos) ({pos.posX()+pos.width(),pos.posY()+pos.height()},{-pos.width(),-pos.height()});
  return *this;
}

Stone& Stone::setSetted(bool set) {
  if(setted!=set) {
      pos = decltype(pos) ({pos.posX()+pos.width(),pos.posY()+pos.height()},{-pos.width(),-pos.height()});
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

Stone& Stone::setTexture(const GLTexture2D& txtr) {
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

void Stone::click(const WorldPos &w_pos) {
  (void)w_pos;
  if(click_call_back) {
      click_call_back();
    }
}

void Stone::mouseDown(const MouseEvent &mouse) {
  (void)mouse;
  if(mouse.button==MOUSE_BUTTON_1) {
      pressed = true;
    }
}

void Stone::mouseUp(const MouseEvent &mouse) {
  if(mouse.button==MOUSE_BUTTON_1) {
      if(underMouse(mouse.pos)&&pressed) {
          click(mouse.pos);
        }
      pressed = false;
    }
}

void Stone::hover(const MouseEvent &mouse) {
  (void)mouse;
  active = true;
}

void Stone::unHover() {
  active = false;
}

static inline double SQR(double x) { return x*x; }

bool Stone::underMouse(const WorldPos &m_pos) const {
  return SQR(float(pos.posXcenter()-m_pos.x)/(pos.width()/2.0))+SQR(float(pos.posYcenter()-m_pos.y)/(pos.height()/2.0))<1;
}

void Stone::setPos(const WorldPos &m_pos) {
  if (setted)
    pos = decltype(pos) ({pos.posX()+pos.width(),pos.posY()+pos.height()},{-pos.width(),-pos.height()});
  pos.setPos(m_pos);
  if (setted)
    pos = decltype(pos) ({pos.posX()+pos.width(),pos.posY()+pos.height()},{-pos.width(),-pos.height()});

}

WorldPos::COORD_TYPE Stone::posX() const {
  return pos.posX();
}

WorldPos::COORD_TYPE Stone::posY() const {
  return pos.posY();
}

WorldPos::COORD_TYPE Stone::height() const {
  return pos.height();
}

WorldPos::COORD_TYPE Stone::width() const {
  return pos.width();
}
