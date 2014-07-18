#ifndef BUTTON_H
#define BUTTON_H

#if !defined(HAVE_GLES)
#include <GL/gl.h>
#include "GL/glu.h"
#else
#include <GLES/gl.h>
#endif

#include "GLrenderobject.h"
#include "GLtextures.h"
#include "GLfontkeeperbase.h"
#include "GLRectangleCoord.h"
#include "GLlabel.h"

#include <utility>
#include <functional>

using ButtonClickCallBack=std::function<void()>;

class GLButton: public GLRenderObject, public GLFontKeeperBase<GLButton>
{
public:
  GLButton(GLint x_left_top = 0,
         GLint y_left_top = 0,
         GLint width = 0,
         GLint height = 0,
         const string &caption = L"Button",
         const GLTexture2D& texture = GLTexture2D());

  GLButton& setTexture(const GLTexture2D& texture);
  GLButton& setCaption(const string& caption);
  GLButton& setClickCallBack(const ButtonClickCallBack& call_back);
  GLButton& setSize(GLint width, GLint height);

  virtual GLButton& setFont(const QFont& font) override;
  virtual const QFont& getFont() const override;

  virtual GLButton& setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) override;

  virtual const GLint* getFontColor() const override;

  bool isPressed() const { return pressed; }
protected:
  GLButton& setPressed(bool pressed);
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

  static GLTexture2D texture_blurr;

private:
  //calculate and set position of text
  void resetTextPos();
  GLTexture2D texture;

  //4 two dimensional points
  //left_top, right_top, right_bottom, left_bottom
  GLRectangleCoord<GLint> pos;

  GLLabel text;

  ButtonClickCallBack click_call_back;

  bool active;
  bool hovered;
  bool pressed;
  //for some little effect
  GLint alpha_color_bak;
};

#endif // BUTTON_H
