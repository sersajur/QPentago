#ifndef TEXTURES_H
#define TEXTURES_H

//QT staff
#include <QString>
#include <QGLContext>

//

#include "GLrenderobject.h"

class GLTexture2D
{
  class Textures2DHolder;

  struct Texture2DInfo {
    GLint texture;
    int width, height;
    Texture2DInfo(): texture(0), width(0), height(0) {}
    Texture2DInfo(GLuint t, int w, int h): texture(t), width(w), height(h) {}
  };

  friend class Textures2DHolder;

  Texture2DInfo info;
  QString filename;
  GLdouble crop[4][2];
  static QGLContext* context;
public:
  static void setContext(QGLContext* ctxt) {
    context = ctxt;
  }

  //construct and load texture from file
  // "" - means empty texture
  explicit GLTexture2D(const QString& filename = "");

  GLTexture2D(const GLTexture2D& right);
  GLTexture2D& operator=(const GLTexture2D& right);

  GLTexture2D(GLTexture2D&&) = default;
  GLTexture2D& operator=(GLTexture2D&&) = default;

  ~GLTexture2D();

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
  void draw(const WorldPos& left_top = WorldPos(0,0)) const;

  //draw texture on current OpenGL context
  //starting with left top coordinates to right bottom
  void draw(const WorldPos& left_top,const WorldPos& right_bottom) const;

  //draw texture on current OpenGL context
  //in quadrangle setted by input parameters
  void draw(const WorldPos& left_top,
             const WorldPos& right_top,
             const WorldPos& right_bottom,
             const WorldPos& left_bottom) const;

  //draw texture on current OpenGL context
  //in quadrangle setted by input array
  //using glDrawElements
  void draw(const GLint* pos, int dim) const;

  void draw(const GLdouble* pos, int dim) const;

  void draw(const GLint* pos, int dim, const GLdouble* crop_in, int vertex_count = 4) const;

  void draw(const GLdouble* pos, int dim, const GLdouble* crop_in, int vertex_count = 4) const;

  //if texture must be drawn cropped
  GLTexture2D& setCropRegion(
          const WorldPos& left_top,const WorldPos& right_top,
          const WorldPos& right_bottom, const WorldPos& left_bottom);
};

#endif // TEXTURES_H
