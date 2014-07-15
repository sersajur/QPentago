#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "renderobject.h"
#include "textures.h"
#include "label.h"
#include "GLRectangleCoord.h"
#include "fontkeeperbase.h"

#ifndef HAVE_GLES
#include "GL/gl.h"
#else
#include "GLES/gl.h"
#endif

class TextEdit : public RenderObject, public FontKeeperBase<TextEdit>
{
public:
    TextEdit(GLint x_left_top = 0,
             GLint y_left_top = 0,
             GLint width = 0,
             GLint height = 0,
             const Texture2D& background = Texture2D());

    TextEdit& setSize(GLint width,GLint  height);

    unsigned getCurPos() const { return cur_pos; }
    TextEdit& setCurPos(int cursor_pos);

    int getMaxTextLength() const { return max_width; }
    TextEdit& setMaxTextLength(int length);

    const string& getText() const { return text.getText(); }
    TextEdit& setText(const string& text);
    TextEdit& setTexture(const Texture2D& background);

    virtual void draw() const override;

    virtual void setActive(bool active) override;
    virtual bool isActive() const override;
    virtual bool canBeActive() const override;

    virtual void click(int x, int y) override;
    virtual void mouseDown(int x, int y) override;
    virtual void mouseUp(int x, int y) override;
    virtual void hover(int x, int y) override;
    virtual void unHover() override;
    virtual bool underMouse(int x, int y) const override;

    virtual void setPos(int x, int y) override;
    virtual int posX() const override;
    virtual int posY() const override;
    virtual int height() const override;
    virtual int width() const override;

    virtual void keyPress(int key, bool repeat, KeyboardModifier mod) override;
    virtual void keyRelease(int key, KeyboardModifier mod) override;
    virtual void charInput(int unicode_key) override;

    virtual TextEdit& setFont(const QFont& font) override;
    virtual const QFont& getFont() const override;

    virtual TextEdit& setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) override;

    virtual const GLint* getFontColor() const override;

private:
    Texture2D background;

    GLdouble cur_world_pos;
    Label text;
    bool active;
    bool hovered;
    GLRectangleCoord<GLint,2> pos;
    GLRectangleCoord<GLint,2> text_crop;
    GLRectangleCoord<GLint,2> back_pos;
    void calcCrop();

    //cursor position in the text
    int cur_pos;
    int max_width;
    //position in the render world
};

#endif // TEXTEDIT_H
