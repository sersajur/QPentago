#ifndef BOARD_H
#define BOARD_H

#include "GLrenderobject.h"

#include <memory>
#include <functional>

using RotateCallBack = std::function<void(int quadrant_x, int quadrant_y, bool rotate_right)>;
using StoneSetCallBack = std::function<void(int pos_x, int pos_y)>;

class PentagoBoard: public GLRenderObject
{
  class PentagoBoardImpl;
public:
  PentagoBoard(const WorldPos &pos_left_top = WorldPos(0,0),
               const WorldPos &vector_size = WorldPos(0,0),
               unsigned board_size = 2);

  PentagoBoard(const PentagoBoard& rigth);
  PentagoBoard& operator=(const PentagoBoard& rigth);

  PentagoBoard(PentagoBoard&&) = default;
  PentagoBoard& operator=(PentagoBoard&&) = default;

  ~PentagoBoard();

  PentagoBoard& initTextures();

  PentagoBoard& setSize(const WorldPos &v_size);

  PentagoBoard& setStone(int x_pos, int y_pos);
  PentagoBoard& unsetStone(int x_pos, int y_pos);
  PentagoBoard& setStoneColor(int x_pos, int y_pos, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
  PentagoBoard& rotate(int board_x, int board_y, bool  right_direction);

  PentagoBoard& enableRotate(bool enable);

  PentagoBoard& setBoardSize(unsigned board_size);
  unsigned getBoardWidth() const;
  unsigned getBoardHeight() const;

  PentagoBoard& setRotateCallBack(const RotateCallBack &rotate_call_back);
  void callRotateCallBack(int quadrant_x, int quadrant_y, bool rotate_right) const;

  PentagoBoard& setStoneSetCallBack(const StoneSetCallBack &stone_set_call_back);
  void callStoneSetCallBack(int pos_x, int pos_y) const;

  virtual void draw() const override;

  virtual void setActive(bool active) override;
  virtual bool isActive() const override;
  virtual bool canBeActive() const override;

  virtual void click(const WorldPos &m_pos) override;
  virtual void mouseDown(const MouseEvent &mouse) override;
  virtual void mouseUp(const MouseEvent &mouse) override;
  virtual void hover(const MouseEvent &mouse) override;
  virtual void unHover() override;
  virtual bool underMouse(const WorldPos &m_pos) const override;

  virtual void setPos(const WorldPos &m_pos) override;
  virtual WorldPos::COORD_TYPE posX() const override;
  virtual WorldPos::COORD_TYPE posY() const override;
  virtual WorldPos::COORD_TYPE height() const override;
  virtual WorldPos::COORD_TYPE width() const override;

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) override;
  virtual void keyRelease(int key, KeyboardModifier mod) override;

private:
  PentagoBoardImpl* impl;
};

#endif // BOARD_H
