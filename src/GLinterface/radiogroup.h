#ifndef RADIOGROUP_H
#define RADIOGROUP_H

#if !defined(HAVE_GLES)
#include <GL/gl.h>
#else
#include <GLES/gl.h>
#endif

#include "renderobject.h"
#include "textures.h"
#include "fontkeeper.h"
#include "GLRectangleCoord.h"

#include <string>
#include <vector>

using string = std::wstring;
using str_array = std::vector<string>;

class RadioGroup : public RenderObject, public FontKeeper<RadioGroup>
{
public:
  RadioGroup(
         GLint x_left_top = 0,
         GLint y_left_top = 0,
         GLint width = 0,
         GLint height = 0,
         const Texture2D& texture = Texture2D());

  RadioGroup& setWidth(GLint width);
  RadioGroup& setList(const str_array& list);

  RadioGroup& setSelectedIndex(int index);
  int getItemCount() const;
  string getItem(int index) const;
  int getSelectedIndex() const;
  string getSelectedItem() const;  //same as getItem(getSelectedIndex())

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

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod) override;
  virtual void keyRelease(int key, KeyboardModifier mod) override;

  virtual ~RadioGroup() { }
private:
  GLRectangleCoord pos;
  Texture2D texture;
};

#endif // RADIOGROUP_H
