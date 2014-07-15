#include "button.h"

#include <QGLWidget>

#include <climits>

Texture2D Button::texture_blurr;

Button::Button(GLint x_left_top,
               GLint y_left_top,
               GLint width,
               GLint height,
               const string &caption,
               const Texture2D& texture):
    pos(x_left_top, y_left_top),
    text(caption),
    active(false),
    hovered(false),
    pressed(false),
    alpha_color_bak(INT_MAX) {

  setFontColor4d(0.0,0.0,0.0,1.0);
  setSize(width,height);
  setTexture(texture);
  setFont(QFont("Snap ITC", height/2, 40, false));
}

Button& Button::setTexture(const Texture2D& texture) {
  this->texture = texture;
  return *this;
}

Button& Button::setCaption(const string& text) {
  this->text.setText(text);
  resetTextPos();
  return *this;
}

Button& Button::setClickCallBack(const std::function<void()>& call_back) {
  click_call_back = call_back;
  return *this;
}

Button& Button::setSize(GLint width,GLint  height) {
  pos.setSize(width,height);
  QFont font = text.getFont();
  height = abs(height);
  height = std::min(height,150);
  height /= 2;
  font.setPointSize(height==0?1:height);
  text.setFont(font);
  resetTextPos();
  return *this;
}


//From FontFeeperBase<Button>
//
Button& Button::setFont(const QFont& font) {
    text.setFont(font);
    resetTextPos();
    return *this;
}

const QFont& Button::getFont() const {
    return text.getFont();
}

Button& Button::setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) {
    alpha_color_bak = alpha;
    text.setFontColor4i(red,green,blue,alpha);
    return *this;
}

const GLint* Button::getFontColor() const {
    return text.getFontColor();
}
//
//

void Button::resetTextPos() {
  //allign center:
  //TODO: make possible allign left or right
  text.setPos(pos.posXcenter()-text.width()/2,
              pos.posYcenter()-text.height()/2);

}

void Button::setActive(bool active) {
  this->active = active;
}

Button& Button::setPressed(bool pressed) {
  if(this->pressed!=pressed) {
    this->pressed = pressed;
    const GLint* color = text.getFontColor();
    if (pressed) {
      alpha_color_bak = color[3];
      text.setFontColor4i(color[0],color[1],color[2],0.7*INT_MAX);
    }
    else
      text.setFontColor4i(color[0],color[1],color[2],alpha_color_bak);
  }
  return *this;
}



void Button::draw() const {
  decltype(pos) work_pos = pos;
  GLint dx = 0;
  GLint dy = 0;
  if (active && pressed) {
    glColor4f(.39,.39,.39,.55);
    texture.draw(pos.glCoords(),pos.dimension);
    dx = 2;
    dy = 1;
  }
  glColor4f(1,1,1,1);//black
  work_pos.setLeft(work_pos.getLeft()+dx);
  work_pos.setRight(work_pos.getRight()-dx);
  work_pos.setTop(work_pos.getTop()+dy);
  work_pos.setBottom(work_pos.getBottom()-dy);

  texture.draw(work_pos.glCoords(),work_pos.dimension);
  if(active | pressed | hovered) {
    dx += work_pos.width()*0.03;
    work_pos.setLeft(work_pos.getLeft()+dx);
    work_pos.setRight(work_pos.getRight()-dx);

    glColor4f(1,1,1,.23);//transparent
    texture_blurr.draw(work_pos.glCoords(),work_pos.dimension);
  }
  text.draw();
}

void Button::click(GLint x, GLint y) {
  if(click_call_back)
    click_call_back();
  (void)x;
  (void)y;
}

void Button::mouseDown(GLint x, GLint y) {
  setPressed(true);
  (void)x;
  (void)y;
}

void Button::mouseUp(GLint x, GLint y) {
  if (pressed && underMouse(x,y)) {
    setPressed(false);//order is important
    click(x,y);//
  }
  setPressed(false);
}

void Button::hover(GLint x, GLint y) {
  hovered = true;
  setActive(true);
  (void)x;
  (void)y;
}

void Button::unHover() {
  hovered = false;
  setActive(false);
}

bool Button::underMouse(GLint x, GLint y) const {
  return pos.posInRect(x,y);
}

void Button::setPos(GLint x, GLint y) {
  pos.setPos(x,y);
  resetTextPos();
}

GLint Button::posX() const {
  return pos.posX();
}

GLint Button::posY() const {
  return pos.posY();
}

GLint Button::height() const {
  return pos.height();
}

GLint Button::width() const {
  return pos.width();
}

void Button::keyPress(int key, bool repeat, KeyboardModifier mod) {
  (void)repeat;
  if((mod==MD_NONE) && (key==Qt::Key_Return)) {
    mouseDown(pos.posXcenter(),pos.posYcenter());
  }
}

void Button::keyRelease(int key, KeyboardModifier mod) {
  (void)mod;
  if(key==Qt::Key_Return) {
    mouseUp(pos.posXcenter(),pos.posYcenter());
  }
}
