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
    GLTextEdit(const WorldPos &pos_left_top = WorldPos(0,0),
             const WorldPos &vector_size = WorldPos(0,0),
             const GLTexture2D& background = GLTexture2D());

    GLTextEdit& setSize(const WorldPos &v_size);

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

    virtual void click(const WorldPos &w_pos) override;
    virtual void mouseDown(const MouseEvent &mouse) override;
    virtual void mouseUp(const MouseEvent &mouse) override;
    virtual void hover(const MouseEvent &mouse) override;
    virtual void unHover() override;
    virtual bool underMouse(const WorldPos &m_pos) const override;

    virtual void setPos(const WorldPos &w_pos) override;
    virtual WorldPos::COORD_TYPE posX() const override;
    virtual WorldPos::COORD_TYPE posY() const override;
    virtual WorldPos::COORD_TYPE height() const override;
    virtual WorldPos::COORD_TYPE width() const override;

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
    GLRectangleCoord pos;
    GLRectangleCoord text_crop;
    GLRectangleCoord back_pos;
    void calcCrop();

    //cursor position in the text
    int cur_pos;
    int max_width;
    //position in the render world

    std::map<TextEditHotKey,TextEditKeyCallBack> key_call_backs;
};

#endif // TEXTEDIT_H
