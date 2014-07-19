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
         const WorldPos &pos_left_top = WorldPos(0,0),
         WorldPos::COORD_TYPE width = 0,
         const GLTexture2D& background = GLTexture2D());

  GLRadioGroup& setWidth(WorldPos::COORD_TYPE width);
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

  virtual void click(const WorldPos &w_pos) override;
  virtual void mouseDown(const MouseEvent &mouse) override;
  virtual void mouseUp(const MouseEvent &mouse) override;
  virtual void hover(const MouseEvent &mouse) override;
  virtual void unHover() override;
  virtual bool underMouse(const WorldPos &m_pos) const override;

  virtual void setPos(const WorldPos &w_pos) override;
  virtual WorldPos::COORD_TYPE posX() const override;
  virtual WorldPos::COORD_TYPE posY() const override;
  virtual WorldPos::COORD_TYPE height() const override;
  virtual WorldPos::COORD_TYPE width() const override;

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) override;
  virtual void keyRelease(int key, KeyboardModifier mod) override;

  virtual ~GLRadioGroup() { }

protected:
  virtual void fontChanged() override;
private:
  GLRectangleCoord pos;
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

  WorldPos::COORD_TYPE item_height;

  RadioGroupSelectedChangedCallBack selected_changed_call_back;
public:
  static GLTexture2D texture_blurr;
};

#endif // RADIOGROUP_H
