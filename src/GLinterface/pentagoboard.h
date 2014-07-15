#ifndef BOARD_H
#define BOARD_H

#include "renderobject.h"

#include <memory>
#include <functional>

#ifndef HAVE_GLES
#include <GL/gl.h>
#else
#include <GLES/gl.h>
#endif

using RotateCallBack = void(int quadrant_x, int quadrant_y, bool rotate_right);
using StoneSetCallBack = void(int pos_x, int pos_y);

class PentagoBoardImpl;

class PentagoBoard: public RenderObject
{
public:
  PentagoBoard(GLint x_left_top = 0,
        GLint y_left_top = 0,
        GLint width = 0,
        GLint height = 0,
        int board_size = 2);

  ~PentagoBoard();

  PentagoBoard(const PentagoBoard& rigth);
  PentagoBoard& operator=(const PentagoBoard& rigth);

  PentagoBoard& setSize(int width, int height);

  PentagoBoard& setStone(int x_pos, int y_pos);
  PentagoBoard& unsetStone(int x_pos, int y_pos);
  PentagoBoard& setStoneColor(int x_pos, int y_pos, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
  PentagoBoard& rotate(int board_x, int board_y, bool  right_direction);

  PentagoBoard& setRotateCallBack(std::function<RotateCallBack> rotate_call_back);
  void callRotateCallBack(int quadrant_x, int quadrant_y, bool rotate_right) const;

  PentagoBoard& setStoneSetCallBack(std::function<StoneSetCallBack> stone_set_call_back);
  void callStoneSetCallBack(int pos_x, int pos_y) const;

  virtual void draw() const override;

  virtual void setActive(bool active) override;
  virtual bool isActive() const override;
  virtual bool canBeActive() const override;

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

private:
  PentagoBoardImpl* impl;
};

#endif // BOARD_H
