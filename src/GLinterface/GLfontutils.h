#ifndef GL_FONT_UTILS_H
#define GL_FONT_UTILS_H

#ifdef HAVE_GLES
#include <GLES/gl.h>
#else
#include <GL/gl.h>
#endif

#include <QtGlobal>
#include <string>


class QChar;
class QFont;
class QFontMetrics;

using string = std::wstring;

namespace glutils
{

class GLfontImpl;

class GLfont final
{
public:
    GLfont(const QFont &f);
    ~GLfont();

    const QFont& font() const;
    const QFontMetrics& fontMetrics() const;

    void renderText(GLdouble x, GLdouble y, const string &text);
    void renderTextCroped(GLdouble x, GLdouble y, const string &text, GLdouble x_left, GLdouble x_right);

private:
    Q_DISABLE_COPY(GLfont)

    GLfontImpl *const d;
};

} // namespace opengl


#endif // GL_FONT_UTILS_H
