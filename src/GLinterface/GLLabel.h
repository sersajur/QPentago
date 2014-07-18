#ifndef LABEL_H
#define LABEL_H

#include "renderobject.h"
#include "textures.h"
#include "GLfontutils.h"

#include "memory"
#include "fontkeeper.h"

class Label: public RenderObject, public FontKeeper<Label>
{
public:

  Label(const string& text = L"label",
        int x_left_top = 0,
        int y_left_top = 0,
        const QFont& font = QFont());

  Label& setText(const string& text);
  const string& getText() const { return text; }

  //width and height of text according to Font
  virtual int width() const override;
  virtual int height() const override;

  Label& setBackground(const Texture2D& texture);

  virtual void draw() const override;
  void drawCroped(int x_left, int x_right) const;

  //set and get position of baseline
  virtual void setPos(int x, int y) override;
  virtual int posX() const override;
  virtual int posY() const override;

protected:
  virtual void fontChanged() override;
private:
  Texture2D backgound;
  string text;
  int x_pos, y_pos;
  int text_width;
};

#endif // LABEL_H
