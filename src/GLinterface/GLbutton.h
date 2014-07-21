#ifndef BUTTON_H
#define BUTTON_H

#include "GLrenderobject.h"
#include "GLtextures.h"
#include "GLfontkeeperbase.h"
#include "GLrectanglecoord.h"
#include "GLlabel.h"

#include <utility>
#include <functional>

using ButtonClickCallBack=std::function<void()>;

class GLButton: public GLRenderObject, public GLFontKeeperBase<GLButton>
{
public:
  explicit GLButton(const WorldPos &pos_left_top = WorldPos(0,0),
         const WorldPos &vector_size = WorldPos(0,0),
         const string &caption = L"Button",
         const GLTexture2D& texture = GLTexture2D());

  GLButton& setTexture(const GLTexture2D& texture);

  GLButton& setCaption(const string& caption);
  const string& getCaption() const;

  GLButton& setClickCallBack(const ButtonClickCallBack& call_back);

  GLButton& setSize(const WorldPos &v_size);

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

  virtual void click(const WorldPos &pos) override;
  virtual void mouseDown(const MouseEvent &mouse) override;
  virtual void mouseUp(const MouseEvent &mouse) override;
  virtual void hover(const MouseEvent &mouse) override;
  virtual void unHover() override;
  virtual bool underMouse(const WorldPos &m_pos) const override;

  //position of left top corner
  virtual void setPos(const WorldPos &w_pos) override;
  virtual WorldPos::COORD_TYPE posX() const override;
  virtual WorldPos::COORD_TYPE posY() const override;

  virtual WorldPos::COORD_TYPE height() const override;
  virtual WorldPos::COORD_TYPE width() const override;

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) override;
  virtual void keyRelease(int key, KeyboardModifier mod) override;

  static GLTexture2D texture_blurr;

private:
  //calculate and set position of text
  void resetTextPos();
  GLTexture2D texture;

  //4 two dimensional points
  //left_top, right_top, right_bottom, left_bottom
  GLRectangleCoord pos;

  GLLabel text;

  ButtonClickCallBack click_call_back;

  bool active;
  bool hovered;
  bool pressed;
  //for some little effect
  GLint alpha_color_bak;
};

#endif // BUTTON_H
