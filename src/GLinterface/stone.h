#ifndef STONE_H
#define STONE_H

#include "GLrenderobject.h"
#include "GLrectanglecoord.h"
#include "GLtextures.h"

#include <functional>

class Stone: public GLRenderObject
{
public:
  explicit Stone(const WorldPos &pos_left_top = WorldPos(0,0),
        WorldPos::COORD_TYPE radius = 0,
        const GLTexture2D& texture = GLTexture2D());

  Stone& setSize(const WorldPos &v_size);

  bool isSetted() const { return setted; }
  Stone& setSetted(bool set);

  Stone& setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
  const GLfloat* getColor() const;

  Stone& setTexture(const GLTexture2D& texture = GLTexture2D());

  Stone& setClickCallBack(const std::function<void()>& call_back);

  virtual void draw() const override;
  void draw(double angle) const;

  virtual void setActive(bool active) { this->active = active; }
  virtual bool isActive() const { return active; }
  virtual bool canBeActive() const { return true; }

  virtual void click(const WorldPos &w_pos) override;
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

private:
  bool active;
  bool setted;
  bool pressed;
  GLfloat color[4];

  GLRectangleCoord pos;
  GLTexture2D texture;

  std::function<void()> click_call_back;
};

#endif // STONE_H
