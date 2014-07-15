#include "GLfontutils.h"

#include <vector>
#include <unordered_map>

#include <cmath>
#include <iostream>

#include <QSysInfo>
#include <QPainter>
#include <QPixmap>
#include <QGLFormat>

#include "GLRectangleCoord.h"

namespace
{
    const int TEXTURE_SIZE = 256;

    struct CharData
    {
        GLuint textureId;
        GLsizei width;
        GLsizei height;
        GLRectangleCoord<GLfloat> pos;
    };

} // anonymous namespace


namespace glutils
{

struct GLfontImpl
{
    GLfontImpl(const QFont &f);
    ~GLfontImpl();

    void allocateTexture();
    CharData &createCharacter(QChar c);

    QFont font;
    QFontMetrics fontMetrics;

    std::unordered_map<ushort, CharData> characters;
    std::vector<GLuint> textures;

    GLint xOffset;
    GLint yOffset;
};

GLfontImpl::GLfontImpl(const QFont &f)
    : font(f), fontMetrics(f), xOffset(0), yOffset(0)
{
}

GLfontImpl::~GLfontImpl()
{
    if(!textures.empty())
        glDeleteTextures(textures.size(), textures.data());
}

void GLfontImpl::allocateTexture()
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    QImage image(TEXTURE_SIZE, TEXTURE_SIZE, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    image = QGLWidget::convertToGLFormat(image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, TEXTURE_SIZE, TEXTURE_SIZE,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    textures.push_back(texture);
}

CharData &GLfontImpl::createCharacter(QChar c)
{
    ushort unicodeC = c.unicode();
    if (characters.find(unicodeC)!=characters.end())
        return characters[unicodeC];

    if (textures.empty())
        allocateTexture();

    GLuint texture = textures.back();

    GLsizei width = fontMetrics.width(c);
    GLsizei height = fontMetrics.height();

    QPixmap pixmap(width, height);
    pixmap.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pixmap);
    painter.setRenderHints(QPainter::HighQualityAntialiasing
                           | QPainter::TextAntialiasing);
    painter.setFont(font);
    painter.setPen(Qt::white);

    painter.drawText(0, fontMetrics.ascent(), c);
    painter.end();


    QImage image = QGLWidget::convertToGLFormat(pixmap.toImage());
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RGBA,
                    GL_UNSIGNED_BYTE, image.bits());

    CharData& character = characters[unicodeC];
    character.textureId = texture;
    character.width = width;
    character.height = height;
//    character.s[0] = static_cast<GLfloat>(xOffset) / TEXTURE_SIZE;
//    character.t[0] = static_cast<GLfloat>(yOffset) / TEXTURE_SIZE;
//    character.s[1] = static_cast<GLfloat>(xOffset + width) / TEXTURE_SIZE;
//    character.t[1] = static_cast<GLfloat>(yOffset + height) / TEXTURE_SIZE;

    character.pos.setLeft(  static_cast<GLfloat>(xOffset) / TEXTURE_SIZE);
    character.pos.setBottom(static_cast<GLfloat>(yOffset) / TEXTURE_SIZE);
    character.pos.setRight( static_cast<GLfloat>(xOffset + width) / TEXTURE_SIZE);
    character.pos.setTop(   static_cast<GLfloat>(yOffset + height) / TEXTURE_SIZE);

    xOffset += width;
    if (xOffset + fontMetrics.maxWidth() >= TEXTURE_SIZE)
    {
        xOffset = 1;
        yOffset += height;
    }
    if (yOffset + fontMetrics.height() >= TEXTURE_SIZE)
    {
        allocateTexture();
        yOffset = 1;
    }
    return character;
}

GLfont::GLfont(const QFont &f) : d(new GLfontImpl(f))
{
}

GLfont::~GLfont()
{
    delete d;
}

const QFont& GLfont::font() const
{
    return d->font;
}

const QFontMetrics& GLfont::fontMetrics() const
{
    return d->fontMetrics;
}

//! Renders text at given x, y.
void GLfont::renderText(GLfloat x, GLfloat y, const string &text)
{
    if(text.empty()) return;
    // If the current context's device is not active for painting, the
    // texture generation does not work. This may be specific to the way
    // MIFit is setup.

    //tested on QT 5.2, and this check is not working
//    if (!QGLContext::currentContext()->device()->paintingActive())
//        return;

//    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);

//    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    glPushMatrix();
    GLuint texture = 0;
//    glTranslatef(x, y, 0);
    GLRectangleCoord<GLfloat> char_pos(x,y);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        CharData &c = d->createCharacter(text[i]);
        if (texture != c.textureId)
        {
            texture = c.textureId;
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        char_pos.setSize(c.width,c.height);
        glVertexPointer(char_pos.dimension, GL_FLOAT, 0, char_pos.glCoords());
        glTexCoordPointer(2, GL_FLOAT, 0, c.pos.glCoords());
        glDrawArrays(GL_TRIANGLE_FAN,0,4);

        char_pos.setPos(char_pos.getLeft()+c.width,char_pos.getTop());

//        glBegin(GL_QUADS);
//        glTexCoord2f(c.s[0], c.t[0]);
//        glVertex2f(0, c.height);


//        glTexCoord2f(c.s[1], c.t[0]);
//        glVertex2f(c.width, c.height);


//        glTexCoord2f(c.s[1], c.t[1]);
//        glVertex2f(c.width, 0);

//        glTexCoord2f(c.s[0], c.t[1]);
//        glVertex2f(0, 0);
//        glEnd();

//        glTranslatef(c.width, 0, 0);
    }

    glPopMatrix();
//    glPopAttrib();
}

void GLfont::renderTextCroped(GLfloat x, GLfloat y, const string &text, GLfloat x_left, GLfloat x_right) {
    if(text.empty()) return;
    GLuint texture = 0;
    GLRectangleCoord<GLfloat> char_pos(x,y);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        CharData &c = d->createCharacter(text[i]);
        if (texture != c.textureId)
        {
            texture = c.textureId;
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        char_pos.setSize(c.width,c.height);
        if((char_pos.getLeft()>=x_left) && (char_pos.getRight()<=x_right)) {
            glVertexPointer(char_pos.dimension, GL_FLOAT, 0, char_pos.glCoords());
            glTexCoordPointer(2, GL_FLOAT, 0, c.pos.glCoords());
            glDrawArrays(GL_TRIANGLE_FAN,0,4);
          }

        char_pos.setPos(char_pos.getLeft()+c.width,char_pos.getTop());
    }

    glPopMatrix();
}

} // namespace glutils
