#ifndef GLRECTANGLECOORD_H
#define GLRECTANGLECOORD_H


//dim - dimension can be 2 or 3
//CoordType - like GLint or GLfloat
//TODO: add 3 dimension support
template <typename CoordType, int dim = 2>
class GLRectangleCoord {
  CoordType pos[4][dim];
public:

  static constexpr int dimension = dim;

  inline GLRectangleCoord(
    CoordType x_left_top = 0,
    CoordType y_left_top = 0,
    CoordType width = 0,
    CoordType height = 0);

  //x,y = left top korner
  void setPos(CoordType x, CoordType y);
  void setSize(CoordType width, CoordType height);

  void setLeft(CoordType left);
  void setRight(CoordType right);
  void setTop(CoordType top);
  void setBottom(CoordType bottom);

  CoordType getLeft() const;
  CoordType getRight() const;
  CoordType getTop() const;
  CoordType getBottom() const;

  //left top
  inline CoordType posX() const;
  //left top
  inline CoordType posY() const;

  inline CoordType height() const;
  inline CoordType width() const;

  inline CoordType posXcenter() const;
  inline CoordType posYcenter() const;

  inline const CoordType* glCoords() const;

  inline bool posInRect(CoordType x, CoordType y) const;
};

template <typename CoordType, int dim>
GLRectangleCoord<CoordType,dim>::GLRectangleCoord(
  CoordType x_left_top,
  CoordType y_left_top,
  CoordType width,
  CoordType height) {
  static_assert(dim==2||dim==3,"Dimension of GLRectangleCoord can be only 2 or 3.");
  setPos(x_left_top,y_left_top);
  setSize(width,height);
}

template <typename CoordType, int dim>
void GLRectangleCoord<CoordType,dim>::setPos(CoordType x, CoordType y) {
  int w = width();
  int h = height();
  pos[0][0] = x;
  pos[0][1] = y;
  setSize(w,h);
}

template <typename CoordType, int dim>
void GLRectangleCoord<CoordType,dim>::setSize(CoordType width, CoordType height) {
  pos[1][0] = pos[0][0] + width;
  pos[1][1] = pos[0][1];

  pos[2][0] = pos[0][0] + width;
  pos[2][1] = pos[0][1] + height;

  pos[3][0] = pos[0][0];
  pos[3][1] = pos[0][1] + height;
}

template <typename CoordType, int dim>
void GLRectangleCoord<CoordType,dim>::setLeft(CoordType left) {
  pos[0][0] = left;
  pos[3][0] = left;
}

template <typename CoordType, int dim>
void GLRectangleCoord<CoordType,dim>::setRight(CoordType right) {
  pos[1][0] = right;
  pos[2][0] = right;
}

template <typename CoordType, int dim>
void GLRectangleCoord<CoordType,dim>::setTop(CoordType top) {
  pos[0][1] = top;
  pos[1][1] = top;
}

template <typename CoordType, int dim>
void GLRectangleCoord<CoordType,dim>::setBottom(CoordType bottom) {
  pos[2][1] = bottom;
  pos[3][1] = bottom;
}


template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::getLeft() const {
  return pos[0][0];
}

template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::getRight() const {
  return pos[1][0];
}

template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::getTop() const {
  return pos[0][1];
}

template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::getBottom() const {
  return pos[3][1];
}

template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::posX() const {
  return pos[0][0];
}
template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::posY() const {
  return pos[0][1];
}

template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::height() const {
  return pos[3][1]-pos[0][1];
}

template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::width() const {
  return pos[1][0]-pos[0][0];
}

template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::posXcenter() const {
  return (pos[0][0]+pos[1][0])/2;
}

template <typename CoordType, int dim>
CoordType GLRectangleCoord<CoordType,dim>::posYcenter() const {
  return (pos[0][1]+pos[3][1])/2;
}

template <typename CoordType, int dim>
const CoordType* GLRectangleCoord<CoordType,dim>::glCoords() const {
  return (CoordType*)pos;
}

template <typename CoordType, int dim>
bool GLRectangleCoord<CoordType,dim>::posInRect(CoordType x, CoordType y) const {
  return (pos[0][0] <= x) && (pos[0][1] <= y) &&
      (pos[2][0] >= x) && (pos[2][1] >= y);
}

#endif // GLRECTANGLECOORD_H
