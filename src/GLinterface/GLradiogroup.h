#ifndef RADIOGROUP_H
#define RADIOGROUP_H

#if !defined(HAVE_GLES)
#include <GL/gl.h>
#else
#include <GLES/gl.h>
#endif

#include "GLrenderobject.h"
#include "GLtextures.h"
#include "GLfontkeeper.h"
#include "GLRectangleCoord.h"
#include "GLlabel.h"

#include <string>
#include <vector>
#include <functional>

using string = std::wstring;
using str_array = std::vector<string>;
using RadioGroupSelectedChangedCallBack = std::function<void(int old_selected_index, int new_selected_index)>;

class GLRadioGroup : public GLRenderObject, public GLFontKeeper<GLRadioGroup>
{
public:
  GLRadioGroup(
         GLint x_left_top = 0,
         GLint y_left_top = 0,
         GLint width = 0,
         const GLTexture2D& background = GLTexture2D());

  GLRadioGroup& setWidth(GLint width);
  GLRadioGroup& setItems(const str_array& items_list);

  GLRadioGroup& setSelectedChangedCallBack(const RadioGroupSelectedChangedCallBack& call_back);

  GLRadioGroup& setSelectedIndex(int index);
  int getItemCount() const;
  const string& getItem(int index) const;
  int getSelectedIndex() const;
  const string& getSelectedItem() const;  //same as getItem(getSelectedIndex())

  virtual void draw() const override;

  virtual void setActive(bool active) override;
  virtual bool isActive() const override;
  virtual bool canBeActive() const { return true; }

  virtual void click(int x, int y) override;
  virtual void mouseDown(int x, int y) override;
  virtual void mouseUp(int x, int y) override;
  virtual void hover(int x, int y) override;
  virtual void unHover() override;
  virtual bool underMouse(int x, int y) const override;

  virtual void setPos(int x, int y) override;
  virtual int posX() const override;
  virtual int posY() const override;
  virtual int height() const override;
  virtual int width() const override;

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) override;
  virtual void keyRelease(int key, KeyboardModifier mod) override;

  virtual ~GLRadioGroup() { }

protected:
  virtual void fontChanged() override;
private:
  GLRectangleCoord<GLint> pos;
  GLTexture2D texture_background;

  GLTexture2D texture_radio[3];
  //texture_radio[int(false)] -- "not selected" texture
  //texture_radio[int(true)]  -- "selected" texture
  //texture_radio[2]          -- "pressed" (mouse down) texture

  bool active;

  int pressed_index;
  int selected_index;
  int hovered_index;

  std::vector<GLLabel> items;

  int item_height;

  RadioGroupSelectedChangedCallBack selected_changed_call_back;
public:
  static GLTexture2D texture_blurr;
};

#endif // RADIOGROUP_H
