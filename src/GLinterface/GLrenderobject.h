#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <Qt>

enum KeyboardModifier {MD_NONE = Qt::NoModifier,/*No modifier key is pressed*/
                        MD_SHIFT = Qt::ShiftModifier,/*Key "Shift" is pressed*/
                        MD_CONTROL = Qt::ControlModifier,/*Key "Control" is pressed*/
                        MD_ALT = Qt::AltModifier,/*Key "Alt" is pressed*/
                        MD_SUPER = Qt::MetaModifier};

class RenderObject
{
public:
  virtual void draw() const = 0; //at least must be implemented

  virtual void setActive(bool active) { (void)active; }
  virtual bool isActive() const { return false; }
  virtual bool canBeActive() const { return false; }

  virtual void click(int x, int y) { (void)x;(void)y; }
  virtual void mouseDown(int x, int y) { (void)x;(void)y; }
  virtual void mouseUp(int x, int y) { (void)x;(void)y; }
  virtual void hover(int x, int y) { (void)x;(void)y; }
  virtual void unHover() {}
  virtual bool underMouse(int x, int y) const { (void)x;(void)y; return false; }

  virtual void setPos(int x, int y) = 0;
  virtual int posX() const = 0;
  virtual int posY() const = 0;
  virtual int height() const = 0;
  virtual int width() const = 0;

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod) { (void)repeat;(void)key;(void)mod; }
  virtual void keyRelease(int key, KeyboardModifier mod) { (void)key;(void)mod; }
  virtual void charInput(int unicode_key) { (void)unicode_key; }

  virtual ~RenderObject() { }
};

#endif // RENDEROBJECT_H
