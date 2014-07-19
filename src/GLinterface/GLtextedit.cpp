#include "GLtextedit.h"
#include <cwctype>

//distance between white background and the text (left and right)
#define DELTA_PIX 6
#define CROP_X 6
#define CROP_Y 4

GLTextEdit::GLTextEdit(const WorldPos &pos_left_top, const WorldPos &vector_size, const GLTexture2D &background):
    background(background),
    text(L"text edit"),
    active(false),
    hovered(false),
    pos(pos_left_top),
    cur_pos(text.width()),
    max_width(-1)  {

  calcCrop();
  setCurPos(0);
  text.setFont(QFont("Snap ITC", 1/*whatever*/, 40, false));
  setSize(vector_size);
}

void GLTextEdit::calcCrop() {
  text_crop = decltype(text_crop)
      ({pos.posX()+CROP_X+DELTA_PIX,pos.posY()+CROP_Y},
       {pos.width()-2*(CROP_X+DELTA_PIX), pos.height()-2*CROP_Y});
  back_pos = decltype(back_pos)
      ({text_crop.posX()-DELTA_PIX,text_crop.posY()},
       {text_crop.width()+DELTA_PIX*2, text_crop.height()});
}

GLTextEdit& GLTextEdit::setSize(const WorldPos &v_size) {
  pos.setSize(v_size);
  calcCrop();
  QFont font = text.getFont();
  WorldPos::COORD_TYPE height = v_size.y;
  height = std::abs(height);
  height = std::min(height,WorldPos::COORD_TYPE(150));
  height /= 2;
  font.setPointSize(height==0?1:height);
  text.setFont(font);
  text.setPos(text_crop.pos());
  return *this;
}

GLTextEdit& GLTextEdit::setCurPos(int pos) {
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
      text.setPos({text.posX()-dx,text.posY()});
    }
  if (cur_world_pos<text_crop.getLeft()) {
      int dx = text_crop.getLeft()-cur_world_pos;
      cur_world_pos+=dx;
      text.setPos({text.posX()+dx,text.posY()});
    }
  return *this;
}

GLTextEdit& GLTextEdit::setMaxTextLength(int length) {
  max_width = length;
  return *this;
}

GLTextEdit& GLTextEdit::setText(const string& text) {
  this->text.setText(text);
  setCurPos(text.length());
  return *this;
}

GLTextEdit& GLTextEdit::setTexture(const GLTexture2D& bckgrnd) {
  background = bckgrnd;
  return *this;
}

GLTextEdit& GLTextEdit::setKeyCallBack(int key, KeyboardModifier mod, const TextEditKeyCallBack& call_back) {
  key_call_backs[TextEditHotKey{key,mod}]=call_back;
  return *this;
}

void GLTextEdit::draw() const {
  glColor4f(1,1,1,1);
  background.draw(pos.glCoords(),pos.dimension);
  glBindTexture(GL_TEXTURE_2D, 0);

  if(active) {
      glColor4f(1,1,1,0.5);
      glVertexPointer(back_pos.dimension, back_pos.glCoordType, 0, back_pos.glCoords());
      glDrawArrays(GL_TRIANGLE_FAN,0,4);

      WorldPos::COORD_TYPE cursor[2][2] = {
        {(WorldPos::COORD_TYPE)cur_world_pos,(WorldPos::COORD_TYPE)(text_crop.getTop()+text_crop.height()/10.0)},
        {(WorldPos::COORD_TYPE)cur_world_pos,(WorldPos::COORD_TYPE)(text_crop.getBottom()-text_crop.height()/10.0)}
      };
      glColor4f(0,0,0,1);
      glVertexPointer(2, WorldPos::glCoordType, 0, (WorldPos::COORD_TYPE*)cursor);
      glLineWidth(2);
      glDrawArrays(GL_LINE_STRIP,0,2);
    }
  if(hovered) {
      glColor4f(.35,.54,1,1);
    } else {
      glColor4f(0,0,0,1);
    }
  glLineWidth(1);
  glVertexPointer(back_pos.dimension, back_pos.glCoordType, 0, back_pos.glCoords());
  glDrawArrays(GL_LINE_LOOP,0,4);
  text.drawCroped(back_pos.getLeft(),back_pos.getRight());
}

void GLTextEdit::setActive(bool act) {
  active = act;
  hovered = false;
}

bool GLTextEdit::isActive() const {
  return active;
}

bool GLTextEdit::canBeActive() const {
  return true;
}

void GLTextEdit::click(const WorldPos &w_pos) {
  (void)w_pos;
}

void GLTextEdit::mouseDown(const MouseEvent &mouse) {
  if(!active&&underMouse(mouse.pos)) {
      setActive(true);
    } else {
      if(text_crop.posInRect(mouse.pos)) {
          int tmp_pos = text.posX();
          for(unsigned i = 0; i<text.getText().length(); i++) {
              int w = text.getFontMetrics().width(text.getText()[i]);
              if(tmp_pos+w/2>mouse.pos.x) {
                  setCurPos(i);
                  tmp_pos+=w/2;
                  break;
                } else {
                  tmp_pos+=w;
                }
            }
          if (tmp_pos<mouse.pos.x) {
              setCurPos(text.getText().length());
            }
        }
    }
}

void GLTextEdit::mouseUp(const MouseEvent &mouse) {
  (void)mouse;
}

void GLTextEdit::hover(const MouseEvent &mouse) {
  hovered = true;
  if(underMouse(mouse.pos)) {
      setActive(true);
    }
}

void GLTextEdit::unHover() {
  hovered = false;
}

bool GLTextEdit::underMouse(const WorldPos &m_pos) const {
  return pos.posInRect(m_pos);
}
void GLTextEdit::setPos(const WorldPos &w_pos) {
  pos.setPos(w_pos);
  calcCrop();
  setCurPos(0);
  text.setPos(text_crop.pos());
}

WorldPos::COORD_TYPE  GLTextEdit::posX() const {
  return pos.posX();
}

WorldPos::COORD_TYPE  GLTextEdit::posY() const {
  return pos.posY();
}

WorldPos::COORD_TYPE  GLTextEdit::height() const {
  return pos.height();
}

WorldPos::COORD_TYPE GLTextEdit::width() const {
  return pos.width();
}

void GLTextEdit::keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) {
  (void)repeat;
  lock_active = false;
  if(key_call_backs.find(TextEditHotKey{key,mod})!=key_call_backs.end()) {
      skip_char_input=key_call_backs[TextEditHotKey{key,mod}](key,mod,*this);
      return;
    }
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
}

void GLTextEdit::keyRelease(int key, KeyboardModifier mod) {
  (void)key;
  (void)mod;
}

void GLTextEdit::charInput(int unicode_key) {
  if (iswprint(unicode_key)&&unicode_key!='\t') {
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

GLTextEdit& GLTextEdit::setFont(const QFont& font) {
  text.setFont(font);
  return *this;
}

const QFont& GLTextEdit::getFont() const {
  return text.getFont();
}

GLTextEdit& GLTextEdit::setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) {
  text.setFontColor4i(red,green,blue,alpha);
  return *this;
}

const GLint* GLTextEdit::getFontColor() const {
  return text.getFontColor();
}
