#include "textedit.h"
#include <cwctype>

//distance between white background and the text (left and right)
#define DELTA_PIX 6
#define CROP_X 6
#define CROP_Y 4

TextEdit::TextEdit(
    GLint x_left_top, GLint y_left_top, GLint width, GLint height, const Texture2D &background):
    background(background),
    text(L"text edit"),
    active(false),
    hovered(false),
    pos(x_left_top,y_left_top),
    cur_pos(text.width()),
    max_width(-1)  {

  calcCrop();
  setCurPos(0);
  text.setFont(QFont("Snap ITC", height/2, 40, false));
  setSize(width,height);
}

void TextEdit::calcCrop() {
  text_crop = decltype(text_crop)
      (pos.posX()+CROP_X+DELTA_PIX,pos.posY()+CROP_Y,pos.width()-2*(CROP_X+DELTA_PIX), pos.height()-2*CROP_Y);
  back_pos = decltype(back_pos)
      (text_crop.posX()-DELTA_PIX,text_crop.posY(),text_crop.width()+DELTA_PIX*2, text_crop.height());
}

TextEdit& TextEdit::setSize(GLint width,GLint  height) {
  pos.setSize(width,height);
  calcCrop();
  QFont font = text.getFont();
  height = abs(height);
  height = std::min(height,150);
  height /= 2;
  font.setPointSize(height==0?1:height);
  text.setFont(font);
  text.setPos(text_crop.posX(),text_crop.posY());
  return *this;
}

TextEdit& TextEdit::setCurPos(int pos) {
  if(pos>(int)text.getText().length())
    pos = text.getText().length();
  else if (pos<0)
    pos = 0;
  cur_pos = pos;
  int tmp_len = 0;
  for(int i=0; i<cur_pos; i++) {
      tmp_len+=text.getFontMetrics().width(text.getText()[i]);
    }
  cur_world_pos = text.posX() + tmp_len;
  if (cur_world_pos>text_crop.getRight()) {
      int dx = cur_world_pos-(text_crop.getRight());
      cur_world_pos-=dx;
      text.setPos(text.posX()-dx,text.posY());
    }
  if (cur_world_pos<text_crop.getLeft()) {
      int dx = text_crop.getLeft()-cur_world_pos;
      cur_world_pos+=dx;
      text.setPos(text.posX()+dx,text.posY());
    }
  return *this;
}

TextEdit& TextEdit::setMaxTextLength(int length) {
  max_width = length;
  return *this;
}

TextEdit& TextEdit::setText(const string& text) {
  this->text.setText(text);
  setCurPos(text.length());
  return *this;
}

TextEdit& TextEdit::setTexture(const Texture2D& bckgrnd) {
  background = bckgrnd;
  return *this;
}

void TextEdit::draw() const {
  glColor4f(1,1,1,1);
  background.draw(pos.glCoords(),pos.dimension);
  glBindTexture(GL_TEXTURE_2D, 0);

  if(active) {
      glColor4f(1,1,1,0.5);
      glVertexPointer(back_pos.dimension, GL_INT, 0, back_pos.glCoords());
      glDrawArrays(GL_TRIANGLE_FAN,0,4);

      GLint cursor[2][2] = {
        {(GLint)cur_world_pos,(GLint)(text_crop.getTop()+text_crop.height()/10.0)},
        {(GLint)cur_world_pos,(GLint)(text_crop.getBottom()-text_crop.height()/10.0)}
      };
      glColor4f(0,0,0,1);
      glVertexPointer(2, GL_INT, 0, (GLint*)cursor);
      glLineWidth(2);
      glDrawArrays(GL_LINE_STRIP,0,2);
    }
  if(hovered) {
      glColor4f(.35,.54,1,1);
    } else {
      glColor4f(0,0,0,1);
    }
  glLineWidth(1);
  glVertexPointer(back_pos.dimension, GL_INT, 0, back_pos.glCoords());
  glDrawArrays(GL_LINE_LOOP,0,4);
  text.drawCroped(back_pos.getLeft(),back_pos.getRight());
}

void TextEdit::setActive(bool act) {
  active = act;
  hovered = false;
}

bool TextEdit::isActive() const {
  return active;
}

bool TextEdit::canBeActive() const {
  return true;
}

void TextEdit::click(int x, int y) {
  (void)x;
  (void)y;
}

void TextEdit::mouseDown(int x, int y) {
  if(!active&&underMouse(x,y)) {
      setActive(true);
    } else {
      if(text_crop.posInRect(x,y)) {
          int tmp_pos = text.posX();
          for(unsigned i = 0; i<text.getText().length(); i++) {
              int w = text.getFontMetrics().width(text.getText()[i]);
              if(tmp_pos+w/2>x) {
                  setCurPos(i);
                  tmp_pos+=w/2;
                  break;
                } else {
                  tmp_pos+=w;
                }
            }
          if (tmp_pos<x) {
              setCurPos(text.getText().length());
            }
        }
    }
}

void TextEdit::mouseUp(int x, int y) {
  (void)x;
  (void)y;
}

void TextEdit::hover(int x, int y) {
  hovered = true;
  if(underMouse(x,y)) {
      setActive(true);
    }
}

void TextEdit::unHover() {
  hovered = false;
}

bool TextEdit::underMouse(int x, int y) const {
  return pos.posInRect(x,y);
}
void TextEdit::setPos(int x, int y) {
  pos.setPos(x,y);
  calcCrop();
  setCurPos(0);
  text.setPos(text_crop.posX(),text_crop.posY());
}

int  TextEdit::posX() const {
  return pos.posX();
}

int  TextEdit::posY() const {
  return pos.posY();
}

int  TextEdit::height() const {
  return pos.height();
}

int  TextEdit::width() const {
  return pos.width();
}

void TextEdit::keyPress(int key, bool repeat, KeyboardModifier mod) {
  if (mod == MD_NONE) {
      switch(key) {
        case Qt::Key_Backspace: {
            if(cur_pos>0) {
                string temp_text = text.getText();
                temp_text.erase(--cur_pos,1);
                text.setText(temp_text);
                setCurPos(cur_pos);
              } else {
                //Beep?
              }
            break;
          }
        case Qt::Key_Delete: {
            if(cur_pos<int(text.getText().length())) {
                string temp_text = text.getText();
                temp_text.erase(cur_pos,1);
                text.setText(temp_text);
              } else {
                //Beep?
              }
            break;
          }
        case Qt::Key_Left: {
            setCurPos(cur_pos-1);
            break;
          }
        case Qt::Key_Right: {
            setCurPos(cur_pos+1);
            break;
          }
        }
    }
  (void)repeat;
}

void TextEdit::keyRelease(int key, KeyboardModifier mod) {
  (void)key;
  (void)mod;
}

void TextEdit::charInput(int unicode_key) {
  if (iswprint(unicode_key)) {
    if (max_width>0 && int(text.getText().size())>=max_width) {
        // TODO: Beep here
        return;
      }
    string temp_text = text.getText();
    temp_text.insert(temp_text.begin()+cur_pos++,wchar_t(unicode_key));
    text.setText(temp_text);
    setCurPos(cur_pos);
  }
}

TextEdit& TextEdit::setFont(const QFont& font) {
  text.setFont(font);
  return *this;
}

const QFont& TextEdit::getFont() const {
  return text.getFont();
}

TextEdit& TextEdit::setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) {
  text.setFontColor4i(red,green,blue,alpha);
  return *this;
}

const GLint* TextEdit::getFontColor() const {
  return text.getFontColor();
}
