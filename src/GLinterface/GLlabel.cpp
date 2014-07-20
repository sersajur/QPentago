#include "GLlabel.h"
#include <limits>
#include <cstdint>
#include <memory>

GLLabel::GLLabel(const string &m_text, const WorldPos &pos_left_top, const QFont& font):
    GLFontKeeper(font), text(m_text), pos(pos_left_top) {
    fontChanged();
}

GLLabel& GLLabel::setText(const string& text) {
  this->text = text;
  fontChanged();
  return *this;
}

GLLabel& GLLabel::setBackground(const GLTexture2D &texture) {
    pos.x-=(backgound?getFontMetrics().averageCharWidth():0);
    backgound = texture;
    pos.x+=(backgound?getFontMetrics().averageCharWidth():0);
    return *this;
}

WorldPos::COORD_TYPE GLLabel::posX() const {
    return pos.x-(backgound?getFontMetrics().averageCharWidth():0);
}

WorldPos::COORD_TYPE GLLabel::posY() const {
    return pos.y;
}

void GLLabel::setPos(const WorldPos &w_pos) {
  pos = {w_pos.x+(backgound?getFontMetrics().averageCharWidth():0),w_pos.y};
}

WorldPos::COORD_TYPE GLLabel::width() const {
    return text_width+(backgound?2*getFontMetrics().averageCharWidth():0);
}

WorldPos::COORD_TYPE GLLabel::height() const {
    return getFontMetrics().height();
}


void GLLabel::draw() const {
  if(backgound) {
      glColor4f(1,1,1,1);
      int a = getFontMetrics().averageCharWidth();
      backgound.draw({pos.x-a,pos.y},
                     {pos.x+text_width+a,pos.y},
                     {pos.x+text_width+a,pos.y+height()},
                     {pos.x-a,pos.y+height()});
  }
  glColor4f(double(font_color[0])/INT_MAX,double(font_color[1])/INT_MAX,double(font_color[2])/INT_MAX,double(font_color[3])/INT_MAX);
  text_font->renderText(pos.x,pos.y,text);
}

void GLLabel::drawCroped(int x_left, int x_right) const {
  glColor4f(double(font_color[0])/INT_MAX,double(font_color[1])/INT_MAX,double(font_color[2])/INT_MAX,double(font_color[3])/INT_MAX);
  text_font->renderTextCroped(pos.x,pos.y,text,x_left, x_right);
}

void GLLabel::fontChanged() {
  this->text_width = text_font->fontMetrics().width(QString::fromStdWString(text));
//  this->strike_out_pos = text_font->fontMetrics().strikeOutPos();
}
