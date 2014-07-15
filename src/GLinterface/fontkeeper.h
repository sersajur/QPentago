#ifndef FONTKEEPER_H
#define FONTKEEPER_H

#include "fontkeeperbase.h"
#include "GLfontutils.h"

#include <memory>

template<typename Keeper>
class FontKeeper: public FontKeeperBase<Keeper>
{
public:
    FontKeeper(const QFont& font = QFont());

    virtual Keeper& setFont(const QFont& font) override final;
    virtual const QFont& getFont() const override final;

    virtual Keeper& setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) override final;

    virtual const GLint* getFontColor() const override final;

    const QFontMetrics &getFontMetrics() const;

protected:
    //font changed call back
    virtual void fontChanged() = 0;

    GLint font_color[4];
    std::shared_ptr<glutils::GLfont> text_font;
};


template<typename Keeper>
FontKeeper<Keeper>::FontKeeper(const QFont &font):
      text_font(new glutils::GLfont(QFont(font))) {
    font_color[0] = 0;
    font_color[1] = 0;
    font_color[2] = 0;
    font_color[3] = INT_MAX;
}

template<typename Keeper>
Keeper& FontKeeper<Keeper>::setFont(const QFont& font) {
    text_font.reset(new glutils::GLfont(QFont(font)));
    fontChanged();
    return dynamic_cast<Keeper&>(*this);
}

template<typename Keeper>
const QFont& FontKeeper<Keeper>::getFont() const {
    return text_font->font();
}
template<typename Keeper>
Keeper& FontKeeper<Keeper>::setFontColor4i(GLint red, GLint green, GLint blue, GLint alpha) {
    font_color[0] = red;
    font_color[1] = green;
    font_color[2] = blue;
    font_color[3] = alpha;
    return dynamic_cast<Keeper&>(*this);
}

template<typename Keeper>
const GLint* FontKeeper<Keeper>::getFontColor() const {
    return font_color;
}

template<typename Keeper>
const QFontMetrics& FontKeeper<Keeper>::getFontMetrics() const {
    return text_font->fontMetrics();
}

#endif // FONTKEEPER_H
