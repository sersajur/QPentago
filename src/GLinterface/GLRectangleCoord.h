#ifndef GLRECTANGLECOORD_H
#define GLRECTANGLECOORD_H

#include "GLrenderobject.h"

//now designed only for 2 dimensional
//TODO: remake later
class GLRectangleCoord {
public:

  using CoordType = WorldPos::COORD_TYPE;
  static constexpr int dimension = sizeof(WorldPos)/sizeof(WorldPos::x);
  static constexpr GLenum glCoordType = WorldPos::glCoordType;

  explicit inline GLRectangleCoord(
      const WorldPos &base_pos = WorldPos(),
      const WorldPos &vector_size = WorldPos());

  //x,y = left top korner
  inline void setPos(const WorldPos &b_pos);
  inline void setSize(const WorldPos &v_pos);

  inline void setLeft(CoordType left);
  inline void setRight(CoordType right);
  inline void setTop(CoordType top);
  inline void setBottom(CoordType bottom);

  inline CoordType getLeft() const;
  inline CoordType getRight() const;
  inline CoordType getTop() const;
  inline CoordType getBottom() const;

  //left top
  inline CoordType posX() const;
  //left top
  inline CoordType posY() const;

  inline WorldPos pos() const;

  inline CoordType height() const;
  inline CoordType width() const;

  inline CoordType posXcenter() const;
  inline CoordType posYcenter() const;

  inline const CoordType* glCoords() const;

  inline bool posInRect(const WorldPos &w_pos) const;
private:
  WorldPos::COORD_TYPE _pos[4][dimension];
};


GLRectangleCoord::GLRectangleCoord(
    const WorldPos &base_pos,
    const WorldPos &vector_size) {
  setPos(base_pos);
  setSize(vector_size);
}

void GLRectangleCoord::setPos(const WorldPos &b_pos) {
  WorldPos::COORD_TYPE w = width();
  WorldPos::COORD_TYPE h = height();
  _pos[0][0] = b_pos.x;
  _pos[0][1] = b_pos.y;
  setSize({w,h});
}


void GLRectangleCoord::setSize(const WorldPos &v_pos) {
  _pos[1][0] = _pos[0][0] + v_pos.x;
  _pos[1][1] = _pos[0][1];

  _pos[2][0] = _pos[0][0] + v_pos.x;
  _pos[2][1] = _pos[0][1] + v_pos.y;

  _pos[3][0] = _pos[0][0];
  _pos[3][1] = _pos[0][1] + v_pos.y;
}


void GLRectangleCoord::setLeft(CoordType left) {
  _pos[0][0] = left;
  _pos[3][0] = left;
}


void GLRectangleCoord::setRight(CoordType right) {
  _pos[1][0] = right;
  _pos[2][0] = right;
}


void GLRectangleCoord::setTop(CoordType top) {
  _pos[0][1] = top;
  _pos[1][1] = top;
}


void GLRectangleCoord::setBottom(CoordType bottom) {
  _pos[2][1] = bottom;
  _pos[3][1] = bottom;
}



GLRectangleCoord::CoordType GLRectangleCoord::getLeft() const {
  return _pos[0][0];
}


GLRectangleCoord::CoordType GLRectangleCoord::getRight() const {
  return _pos[1][0];
}


GLRectangleCoord::CoordType GLRectangleCoord::getTop() const {
  return _pos[0][1];
}


GLRectangleCoord::CoordType GLRectangleCoord::getBottom() const {
  return _pos[3][1];
}


GLRectangleCoord::CoordType GLRectangleCoord::posX() const {
  return _pos[0][0];
}

GLRectangleCoord::CoordType GLRectangleCoord::posY() const {
  return _pos[0][1];
}

WorldPos GLRectangleCoord::pos() const {
  return {posX(),posY()};
}

GLRectangleCoord::CoordType GLRectangleCoord::height() const {
  return _pos[3][1]-_pos[0][1];
}


GLRectangleCoord::CoordType GLRectangleCoord::width() const {
  return _pos[1][0]-_pos[0][0];
}


GLRectangleCoord::CoordType GLRectangleCoord::posXcenter() const {
  return (_pos[0][0]+_pos[1][0])/2;
}


GLRectangleCoord::CoordType GLRectangleCoord::posYcenter() const {
  return (_pos[0][1]+_pos[3][1])/2;
}


const GLRectangleCoord::CoordType* GLRectangleCoord::glCoords() const {
  return (CoordType*)_pos;
}


bool GLRectangleCoord::posInRect(const WorldPos &w_pos) const {
  return (_pos[0][0] <= w_pos.x) && (_pos[0][1] <= w_pos.y) &&
      (_pos[2][0] >= w_pos.x) && (_pos[2][1] >= w_pos.y);
}

#endif // GLRECTANGLECOORD_H
