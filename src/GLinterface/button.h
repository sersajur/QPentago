#ifndef BUTTON_H
#define BUTTON_H

#include "renderobject.h"
#include "textures.h"
#include "fontkeeperbase.h"
#include "GLRectangleCoord.h"
#include "label.h"

#include <utility>
#include <functional>

class QGLWidget;

class Button: public RenderObject, public FontKeeperBase<Button>
{
public:
  Button(GLint x_left_top = 0,
         GLint y_left_top = 0,
         GLint width = 0,
         GLint height = 0,
         const string &caption = L"Button",
         const Texture2D& texture = Texture2D());

  Button& setTexture(const Texture2D& texture);
  Button& setCaption(const string& caption);
  Button& setClickCallBack(const std::function<void()>& call_back);
  Button& setSize(GLint width, GLint height);

  virtual Button& setFont(const QFont& font) override;
  virtual const QFont& getFont() const override;

  virtual Button& setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) override;

  virtual const GLint* getFontColor() const override;

  bool isPressed() const { return pressed; }
protected:
  Button& setPressed(bool pressed);
public:

  virtual void draw() const override;

  virtual void setActive(bool active) override;
  virtual bool isActive() const override { return active; }
  virtual bool canBeActive() const override { return true; }

  virtual void click(GLint x, GLint y) override;
  virtual void mouseDown(GLint x, GLint y) override;
  virtual void mouseUp(GLint x, GLint y) override;
  virtual void hover(GLint x, GLint y) override;
  virtual void unHover() override;
  virtual bool underMouse(GLint x, GLint y) const override;

  //position of left top corner
  virtual void setPos(GLint x, GLint y) override;
  virtual GLint posX() const override;
  virtual GLint posY() const override;

  virtual GLint height() const override;
  virtual GLint width() const override;

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod) override;
  virtual void keyRelease(int key, KeyboardModifier mod) override;

  static Texture2D texture_blurr;

private:
  //calculate and set position of text
  void resetTextPos();
  Texture2D texture;

  //4 two dimensional points
  //left_top, right_top, right_bottom, left_bottom
  GLRectangleCoord<GLint,2> pos;

  Label text;

  std::function<void()> click_call_back;

  bool active;
  bool hovered;
  bool pressed;
  //for some little effect
  GLint alpha_color_bak;
};

#endif // BUTTON_H
