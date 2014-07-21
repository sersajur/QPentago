#ifndef LABEL_H
#define LABEL_H

#include "GLrenderobject.h"
#include "GLtextures.h"
#include "GLfontkeeper.h"

class GLLabel: public GLRenderObject, public GLFontKeeper<GLLabel>
{
public:

  explicit GLLabel(const string& text = L"label",
        const WorldPos &pos_left_top = WorldPos(0,0),
        const QFont& font = QFont());

  GLLabel& setText(const string& text);
  const string& getText() const { return text; }

  //width and height of text according to Font
  virtual WorldPos::COORD_TYPE width() const override;
  virtual WorldPos::COORD_TYPE height() const override;

  GLLabel& setBackground(const GLTexture2D& texture);

  virtual void draw() const override;
  void drawCroped(int x_left, int x_right) const;

  //set and get position of baseline
  virtual void setPos(const WorldPos &w_pos) override;
  virtual WorldPos::COORD_TYPE posX() const override;
  virtual WorldPos::COORD_TYPE posY() const override;

protected:
  virtual void fontChanged() override;
private:
  GLTexture2D backgound;
  string text;
  WorldPos pos;
  int text_width;
};

#endif // LABEL_H
