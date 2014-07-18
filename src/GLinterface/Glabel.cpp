#include "GLLabel.h"
#include <limits>
#include <cstdint>

GLLabel::GLLabel(const string &m_text, int x, int y, const QFont& font):
    GLFontKeeper(font), text(m_text), x_pos(x), y_pos(y) {
    fontChanged();
}

GLLabel& GLLabel::setText(const string& text) {
  this->text = text;
  fontChanged();
  return *this;
}

GLLabel& GLLabel::setBackground(const GLTexture2D &texture) {
    x_pos-=(backgound?getFontMetrics().averageCharWidth():0);
    backgound = texture;
    x_pos+=(backgound?getFontMetrics().averageCharWidth():0);
    return *this;
}

int GLLabel::posX() const {
    return x_pos-(backgound?getFontMetrics().averageCharWidth():0);
}

int GLLabel::posY() const {
    return y_pos;
}

void GLLabel::setPos(int x, int y) {
  x_pos = x+(backgound?getFontMetrics().averageCharWidth():0);
  y_pos = y;
}

int GLLabel::width() const {
    return text_width+(backgound?2*getFontMetrics().averageCharWidth():0);
}

int GLLabel::height() const {
    return getFontMetrics().height();
}


void GLLabel::draw() const {
  if(backgound) {
      glColor4f(1,1,1,1);
      int a = getFontMetrics().averageCharWidth();
      backgound.draw({x_pos-a,y_pos},
                     {x_pos+text_width+a,y_pos},
                     {x_pos+text_width+a,y_pos+height()},
                     {x_pos-a,y_pos+height()});
  }
  glColor4f(double(font_color[0])/INT_MAX,double(font_color[1])/INT_MAX,double(font_color[2])/INT_MAX,double(font_color[3])/INT_MAX);
  text_font->renderText(x_pos,y_pos,text);
}

void GLLabel::drawCroped(int x_left, int x_right) const {
  glColor4f(double(font_color[0])/INT_MAX,double(font_color[1])/INT_MAX,double(font_color[2])/INT_MAX,double(font_color[3])/INT_MAX);
  text_font->renderTextCroped(x_pos,y_pos,text,x_left, x_right);
}

void GLLabel::fontChanged() {
  this->text_width = text_font->fontMetrics().width(QString::fromStdWString(text));
//  this->strike_out_pos = text_font->fontMetrics().strikeOutPos();
}
