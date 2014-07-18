#ifndef LABEL_H
#define LABEL_H

#include "GLrenderobject.h"
#include "GLtextures.h"
#include "GLfontutils.h"
#include "GLfontkeeper.h"

#include <memory>

using string = std::wstring;

class GLLabel: public GLRenderObject, public GLFontKeeper<GLLabel>
{
public:

  GLLabel(const string& text = L"label",
        int x_left_top = 0,
        int y_left_top = 0,
        const QFont& font = QFont());

  GLLabel& setText(const string& text);
  const string& getText() const { return text; }

  //width and height of text according to Font
  virtual int width() const override;
  virtual int height() const override;

  GLLabel& setBackground(const GLTexture2D& texture);

  virtual void draw() const override;
  void drawCroped(int x_left, int x_right) const;

  //set and get position of baseline
  virtual void setPos(int x, int y) override;
  virtual int posX() const override;
  virtual int posY() const override;

protected:
  virtual void fontChanged() override;
private:
  GLTexture2D backgound;
  string text;
  int x_pos, y_pos;
  int text_width;
};

#endif // LABEL_H
