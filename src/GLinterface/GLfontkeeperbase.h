#ifndef FONTKEEPERBASE_H
#define FONTKEEPERBASE_H

#include "GLrenderobject.h"
#include <QFont>

#include <climits>

template<typename Keeper>
class GLFontKeeperBase
{
public:  
  virtual Keeper& setFont(const QFont& font) = 0;
  virtual const QFont& getFont() const = 0;

  virtual Keeper& setFontColor4iv(const GLint* rgba) final {
    return setFontColor4i(rgba[0],rgba[1],rgba[2],rgba[3]);
  }

  virtual Keeper& setFontColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha) final {
    return setFontColor4i(INT_MAX*red,INT_MAX*green,INT_MAX*blue,INT_MAX*alpha);
  }

  virtual Keeper& setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) = 0;

  virtual const GLint* getFontColor() const = 0;

  virtual ~GLFontKeeperBase() {}
};

#endif // FONTKEEPERBASE_H
