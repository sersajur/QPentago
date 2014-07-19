#include "GLbutton.h"

#include <QGLWidget>

#include <climits>

GLTexture2D GLButton::texture_blurr;

GLButton::GLButton(const WorldPos &pos_left_top,
               const WorldPos &vector_size,
               const string &caption,
               const GLTexture2D& texture):
    pos(pos_left_top),
    text(caption),
    active(false),
    hovered(false),
    pressed(false),
    alpha_color_bak(INT_MAX) {

  setFontColor4d(0.0,0.0,0.0,1.0);
  setFont(QFont("Snap ITC", 1/*whatever*/, 40, false));
  setSize(vector_size);
  setTexture(texture);
}

GLButton& GLButton::setTexture(const GLTexture2D& texture) {
  this->texture = texture;
  return *this;
}

GLButton& GLButton::setCaption(const string& text) {
  this->text.setText(text);
  resetTextPos();
  return *this;
}

GLButton& GLButton::setClickCallBack(const ButtonClickCallBack &call_back) {
  click_call_back = call_back;
  return *this;
}

GLButton& GLButton::setSize(const WorldPos &v_size) {
  pos.setSize(v_size);
  QFont font = text.getFont();
  int height = v_size.y;
  height = abs(height);
  height = std::min(height,150);
  height /= 2;
  font.setPointSize(height==0?1:height);
  setFont(font);
  return *this;
}


//From FontFeeperBase<GLButton>
//
GLButton& GLButton::setFont(const QFont& font) {
    text.setFont(font);
    resetTextPos();
    return *this;
}

const QFont& GLButton::getFont() const {
    return text.getFont();
}

GLButton& GLButton::setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) {
    alpha_color_bak = alpha;
    text.setFontColor4i(red,green,blue,alpha);
    return *this;
}

const GLint* GLButton::getFontColor() const {
    return text.getFontColor();
}
//
//

void GLButton::resetTextPos() {
  //allign center:
  //TODO: make possible allign left or right
  text.setPos({pos.posXcenter()-text.width()/2,
              pos.posYcenter()-text.height()/2});

}

void GLButton::setActive(bool new_active) {
  active = new_active;
  if(!new_active)
    hovered = false;
}

GLButton& GLButton::setPressed(bool pressed) {
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



void GLButton::draw() const {
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

    glColor4f(1,1,1,.33);//transparent
    texture_blurr.draw(work_pos.glCoords(),work_pos.dimension);
  }
  text.drawCroped(pos.getLeft(),pos.getRight());
}

void GLButton::click(const WorldPos &pos) {
  if(click_call_back)
    click_call_back();
  (void)pos;
}

void GLButton::mouseDown(const MouseEvent &mouse) {
  setPressed(true);
  (void)mouse;
}

void GLButton::mouseUp(const MouseEvent &mouse) {
  if (pressed && underMouse(mouse.pos)) {
    setPressed(false);//order is important
    click(mouse.pos);//
    return;
  }
  setPressed(false);
}

void GLButton::hover(const MouseEvent &mouse) {
  hovered = true;
  setActive(true);
  (void)mouse;
}

void GLButton::unHover() {
  setActive(false);
}

bool GLButton::underMouse(const WorldPos &m_pos) const {
  return pos.posInRect(m_pos);
}

void GLButton::setPos(const WorldPos &w_pos) {
  pos.setPos(w_pos);
  resetTextPos();
}

WorldPos::COORD_TYPE GLButton::posX() const {
  return pos.posX();
}

WorldPos::COORD_TYPE GLButton::posY() const {
  return pos.posY();
}

WorldPos::COORD_TYPE GLButton::height() const {
  return pos.height();
}

WorldPos::COORD_TYPE GLButton::width() const {
  return pos.width();
}

void GLButton::keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) {
  skip_char_input=false;
  lock_active = pressed;
  if(!repeat) {
      if((mod==MD_NONE) && (key==Qt::Key_Return)) {
          mouseDown(MouseEvent{{pos.posXcenter(),pos.posYcenter()},MOUSE_BUTTON_1,MOUSE_BUTTON_1});
      }
    }
}

void GLButton::keyRelease(int key, KeyboardModifier mod) {
  (void)mod;
  if(key==Qt::Key_Return) {
      mouseUp(MouseEvent{{pos.posXcenter(),pos.posYcenter()},MOUSE_BUTTON_1,MOUSE_BUTTON_1});
  }
}
