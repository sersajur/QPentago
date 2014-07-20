#ifndef GL_FONT_UTILS_H
#define GL_FONT_UTILS_H

#include "GLrenderobject.h"

#include <QFont>

#include <string>


class QChar;
class QFont;
class QFontMetrics;

using string = std::wstring;

namespace glutils
{

class GLFont final
{
  class GLfontImpl;

public:
    GLFont(const QFont &f = QFont());

    GLFont(const GLFont&) = delete;
    GLFont& operator=(const GLFont&) = delete;

    GLFont(GLFont&&) = default;
    GLFont& operator=(GLFont&&) = default;

    ~GLFont();

    const QFont& font() const;
    const QFontMetrics& fontMetrics() const;

    void renderText(GLdouble x, GLdouble y, const string &text);
    void renderTextCroped(GLdouble x, GLdouble y, const string &text, GLdouble x_left, GLdouble x_right);

private:

    GLfontImpl *const d;
};

} // namespace opengl


#endif // GL_FONT_UTILS_H
