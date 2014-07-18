#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "GLrenderobject.h"
#include "GLtextures.h"
#include "GLlabel.h"
#include "GLRectangleCoord.h"
#include "GLfontkeeperbase.h"


#include <map>
#include <functional>
#include <tuple>

#ifndef HAVE_GLES
#include "GL/gl.h"
#else
#include "GLES/gl.h"
#endif

class GLTextEdit;

struct TextEditHotKey {
  int key;
  KeyboardModifier mod;
  bool operator<(const TextEditHotKey& right) const {
    return std::tie(key,mod)<std::tie(right.key,right.mod);
  }
};

//function return true if this key must not be used as char iput at the same event
//if hotkey is registered, regular keyPress for this GLTextEdit will NOT be called
using  TextEditKeyCallBack = std::function<bool(int key, KeyboardModifier mod, GLTextEdit& menu)>;

class GLTextEdit : public GLRenderObject, public GLFontKeeperBase<GLTextEdit>
{
public:
    GLTextEdit(GLint x_left_top = 0,
             GLint y_left_top = 0,
             GLint width = 0,
             GLint height = 0,
             const GLTexture2D& background = GLTexture2D());

    GLTextEdit& setSize(GLint width,GLint  height);

    unsigned getCurPos() const { return cur_pos; }
    GLTextEdit& setCurPos(int cursor_pos);

    int getMaxTextLength() const { return max_width; }
    GLTextEdit& setMaxTextLength(int length);

    const string& getText() const { return text.getText(); }
    GLTextEdit& setText(const string& text);
    GLTextEdit& setTexture(const GLTexture2D& background);

    GLTextEdit& setKeyCallBack(int key, KeyboardModifier mod, const TextEditKeyCallBack& call_back);

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

    virtual void keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) override;
    virtual void keyRelease(int key, KeyboardModifier mod) override;
    virtual void charInput(int unicode_key) override;

    virtual GLTextEdit& setFont(const QFont& font) override;
    virtual const QFont& getFont() const override;

    virtual GLTextEdit& setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) override;

    virtual const GLint* getFontColor() const override;

private:
    GLTexture2D background;

    GLdouble cur_world_pos;
    GLLabel text;
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

    std::map<TextEditHotKey,TextEditKeyCallBack> key_call_backs;
};

#endif // TEXTEDIT_H
