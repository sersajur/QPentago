#ifndef TEXTURES_H
#define TEXTURES_H

#if !defined(HAVE_GLES)
#include <GL/gl.h>
#else
#include <GLES/gl.h>
#endif

//QT staff
#include <QString>
#include <QGLContext>

using string = std::wstring;
using Tcontext = QGLContext;
//

template<typename T>
struct point_base {
    T x,y;
    point_base(T x = 0, T y = 0): x(x), y(y) {}
};

typedef point_base<GLint> point;

struct Texture2DInfo {
  GLint texture;
  int width, height;
  Texture2DInfo(): texture(0), width(0), height(0) {}
  Texture2DInfo(GLuint t, int w, int h): texture(t), width(w), height(h) {}
};

class Texture2D
{
  Texture2DInfo info;
  QString filename;
  GLdouble crop[4][2];
  static Tcontext* context;
public:
  static void setContext(Tcontext* ctxt) {
    context = ctxt;
  }

  //construct and load texture from file
  // "" - means empty texture
  Texture2D(const QString& filename = "");
  ~Texture2D();

  Texture2D(const Texture2D& right);
  Texture2D& operator=(const Texture2D& right);

  //release current and load new texture from filename
  bool load(const QString &filename);
  //release this texture resources
  void release();

  operator GLuint() const { return info.texture; }

  int width() const { return info.width; }

  int height() const { return info.height; }

  //draw texture on current OpenGL context
  //starting with left top coordinates
  //and keep original texture size
  void draw(const point& left_top = point()) const;

  //draw texture on current OpenGL context
  //starting with left top coordinates to right bottom
  void draw(const point& left_top,const point& right_bottom) const;

  //draw texture on current OpenGL context
  //in quadrangle setted by input parameters
  void draw(const point& left_top,
             const point& right_top,
             const point& right_bottom,
             const point& left_bottom) const;

  //draw texture on current OpenGL context
  //in quadrangle setted by input array
  //using glDrawElements
  void draw(const GLint* pos, int dim) const;

  void draw(const GLdouble* pos, int dim) const;

  void draw(const GLint* pos, int dim, const GLdouble* crop_in, int vertex_count = 4) const;

  void draw(const GLdouble* pos, int dim, const GLdouble* crop_in, int vertex_count = 4) const;

  //if texture is repeatable pattern,
  //scale must be setted, otherwise - zero (by default)
  //1,1 - means keep original texture size
//  Texture2D& setRepeatScale(float x_sc, float y_sc);

  //if texture must be drawn cropped
  Texture2D& setCropRegion(
          const point& left_top,const point& right_top,
          const point& right_bottom, const point& left_bottom);
};

#endif // TEXTURES_H
