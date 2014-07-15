#include "textures.h"
#include <map>
#include <mutex>
#include <QtOpenGL>
#include <QImage>

namespace Textures2DHolder {
  struct AllInfo {
    Texture2DInfo info;
    int count;
  };

  static std::mutex m;
  static std::map<QString,AllInfo> textures_list;

  static Texture2DInfo loadTexture(const QString& filename, QGLContext* context) {
    AllInfo texture;
    if (filename!="") {
      std::lock_guard<std::mutex> lg(m);
      try {
        texture = textures_list.at(filename);
        textures_list[filename].count++;
      }catch(std::out_of_range&) {
        QImage im(filename);
        texture.info = Texture2DInfo(context->bindTexture(im), im.width(), im.height());
        texture.count = 1;
        textures_list[filename] = texture;
      }
    }
    return texture.info;
  }

  static void freeTexture(const QString& filename, QGLContext* context) {
    std::lock_guard<std::mutex> lg(m);
    try {
      textures_list.at(filename).count--;
      if(textures_list[filename].count==0) {
        context->deleteTexture(textures_list[filename].info.texture);
        textures_list.erase(filename);
      }
    }catch(...) {

    }
  }
}

Tcontext* Texture2D::context;

Texture2D::Texture2D(const QString &filename):
    crop{{0,1},{1,1},{1,0},{0,0}} {
  load(filename);
}

Texture2D::~Texture2D() {
  release();
}

Texture2D::Texture2D(const Texture2D& right):
    crop{{0,1},{1,1},{1,0},{0,0}} {
  load(right.filename);
}

Texture2D& Texture2D::operator=(const Texture2D& right) {
  release();
  filename = right.filename;
  info = Textures2DHolder::loadTexture(filename,context);
  return *this;
}

bool Texture2D::load(const QString& filename) {
  info = Textures2DHolder::loadTexture(filename,context);
  if (info.texture != 0) {
    this->filename = filename;
  } else {
    return false;
  }
  return true;
}

void Texture2D::release() {
  if (info.texture) {
    Textures2DHolder::freeTexture(filename, context);
    filename.clear();
    info = Texture2DInfo();
  }
}

void Texture2D::draw(const point& left_top) const {
  draw(left_top,{left_top.x+width(),left_top.y+height()});
}

void Texture2D::draw(const point& left_top,const point& right_bottom) const {
  draw(left_top,
        {right_bottom.x, left_top.y},
        right_bottom,
        {left_top.x, right_bottom.y});
}

void Texture2D::draw(const point& left_top,
           const point& right_top,
           const point& right_bottom,
           const point& left_bottom) const {
//  glBindTexture(GL_TEXTURE_2D, info.texture);
//  glBegin(GL_QUADS);
//    glTexCoord2i(crop_lt.x, crop_lt.y); glVertex2d(left_top.x, left_top.y);
//    glTexCoord2i(crop_rt.x, crop_rt.y); glVertex2d(right_top.x, right_top.y);
//    glTexCoord2i(crop_rb.x, crop_rb.y); glVertex2d(right_bottom.x, right_bottom.y);
//    glTexCoord2i(crop_lb.x, crop_lb.y); glVertex2d(left_bottom.x, left_bottom.y);
//  glEnd();
  GLint region[4][2] = {
    {left_top.x,left_top.y},
    {right_top.x,right_top.y},
    {right_bottom.x,right_bottom.y},
    {left_bottom.x,left_bottom.y}
  };
  draw((GLint*)region,2);
}

void Texture2D::draw(const GLint* pos, int dim) const {
  glBindTexture(GL_TEXTURE_2D, info.texture);
  glVertexPointer(dim, GL_INT, 0, pos);
  glTexCoordPointer(2, GL_DOUBLE, 0, crop);
  glDrawArrays(GL_TRIANGLE_FAN,0,4);
}

void Texture2D::draw(const GLdouble* pos, int dim) const {
  glBindTexture(GL_TEXTURE_2D, info.texture);
  glVertexPointer(dim, GL_DOUBLE, 0, pos);
  glTexCoordPointer(2, GL_DOUBLE, 0, crop);
  glDrawArrays(GL_TRIANGLE_FAN,0,4);
}

void Texture2D::draw(const GLint* pos, int dim, const GLdouble* crop_in, int vertex_count) const {
  glBindTexture(GL_TEXTURE_2D, info.texture);
  glVertexPointer(dim, GL_INT, 0, pos);
  glTexCoordPointer(2, GL_DOUBLE, 0, crop_in);
  glDrawArrays(GL_TRIANGLE_FAN,0,vertex_count);
}

void Texture2D::draw(const GLdouble* pos, int dim, const GLdouble* crop_in, int vertex_count) const {
  glBindTexture(GL_TEXTURE_2D, info.texture);
  glVertexPointer(dim, GL_DOUBLE, 0, pos);
  glTexCoordPointer(2, GL_DOUBLE, 0, crop_in);
  glDrawArrays(GL_TRIANGLE_FAN,0,vertex_count);
}

//Texture2D& Texture2D::setRepeatScale(float x_sc, float y_sc) {
//  return *this;
//}

Texture2D& Texture2D::setCropRegion(
        const point& left_top,const point& right_top,
        const point& right_bottom, const point& left_bottom) {
    crop[0][0] = double(info.width)/left_top.x;
    crop[1][0] = double(info.width)/right_top.x;
    crop[2][0] = double(info.width)/right_bottom.x;
    crop[3][0] = double(info.width)/left_bottom.x;

    crop[0][1] = double(info.height)/left_top.y;
    crop[1][1] = double(info.height)/right_top.y;
    crop[2][1] = double(info.height)/right_bottom.y;
    crop[3][1] = double(info.height)/left_bottom.y;
    return *this;
}
