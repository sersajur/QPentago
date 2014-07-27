#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <Qt>
#include <limits>

#ifndef HAVE_GLES
#include <GL/gl.h>
#else
#include <GLES/gl.h>
#endif

static const char DEFAULT_FONT_FAMILY[] = "Lobster";

using string = std::wstring;
template<typename T>
inline string to_string(T val) {
  return std::to_wstring(val);
}
using str_array = std::vector<string>;

enum KeyboardModifier { MD_NONE = Qt::NoModifier,/*No modifier key is pressed*/
                        MD_SHIFT = Qt::ShiftModifier,/*Key "Shift" is pressed*/
                        MD_CONTROL = Qt::ControlModifier,/*Key "Control" is pressed*/
                        MD_ALT = Qt::AltModifier,/*Key "Alt" is pressed*/
                        MD_SUPER = Qt::MetaModifier
};

enum MouseButton {  MOUSE_NO_BUTTONS = Qt::NoButton,
                    MOUSE_BUTTON_1 = Qt::LeftButton,
                    MOUSE_BUTTON_2 = Qt::RightButton,
                    MOUSE_BUTTON_3 = Qt::MiddleButton, /*wheel*/
                    MOUSE_BUTTON_4 = Qt::ExtraButton1, /*back button*/
                    MOUSE_BUTTON_5 = Qt::ExtraButton2, /*forward button*/
                    MOUSE_ALL_BUTTONS = Qt::AllButtons /*can be used as a mask*/
};

struct WorldPos {
  typedef GLdouble COORD_TYPE;
  static constexpr GLenum glCoordType = GL_DOUBLE;

  COORD_TYPE x;
  COORD_TYPE y;

  WorldPos(COORD_TYPE x = std::numeric_limits<COORD_TYPE>::max(), COORD_TYPE y = std::numeric_limits<COORD_TYPE>::max()): x(x), y(y) {}
};

struct MouseEvent {
  WorldPos pos;//position of the mouse
  MouseButton button;//button caused the event (MOUSE_NO_BUTTONS for move buttons)
  MouseButton buttons;//buttons pressen at the event moment (combination of MouseButton using the OR operator)
  explicit MouseEvent(WorldPos mouse_pos = WorldPos(), MouseButton button = MOUSE_NO_BUTTONS, MouseButton buttons = MOUSE_NO_BUTTONS):
    pos(mouse_pos), button(button), buttons(buttons) { (void)mouse_pos; }
};

class GLRenderObject
{
public:
  virtual void draw() const = 0; //at least must be implemented

  virtual void setActive(bool active) { (void)active; }
  virtual bool isActive() const { return false; }
  virtual bool canBeActive() const { return false; }

  virtual void click(const WorldPos &world_pos) { (void)world_pos; }
  virtual void mouseDown(const MouseEvent &mouse) { (void)mouse; }
  virtual void mouseUp(const MouseEvent &mouse) { (void)mouse; }
  virtual void hover(const MouseEvent &mouse) { (void)mouse; }
  virtual void unHover() {}
  virtual bool underMouse(const WorldPos &mouse_pos) const { (void)mouse_pos; return false; }

  virtual void setPos(const WorldPos &world_pos) = 0;
  virtual WorldPos::COORD_TYPE posX() const = 0;
  virtual WorldPos::COORD_TYPE posY() const = 0;
  virtual WorldPos::COORD_TYPE height() const = 0;
  virtual WorldPos::COORD_TYPE width() const = 0;

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) { (void)repeat;(void)key;(void)mod; (void)skip_char_input; (void)lock_active; }
  virtual void keyRelease(int key, KeyboardModifier mod) { (void)key;(void)mod; }
  virtual void charInput(int unicode_key) { (void)unicode_key; }

  virtual ~GLRenderObject() { }
};

#endif // RENDEROBJECT_H
