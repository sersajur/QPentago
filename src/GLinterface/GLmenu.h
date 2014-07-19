#ifndef MENU_H
#define MENU_H

#include "GLrenderobject.h"
#include "GLtextures.h"
#include "GLRectangleCoord.h"

#include <utility>
#include <list>
#include <vector>
#include <memory>
#include <functional>
#include <map>

class GLMenu;
class GLMenuItemClicker;

struct MenuHotkey {
  int key;
  KeyboardModifier mod;
  bool operator<(const MenuHotkey& right) const {
    return std::tie(key,mod)<std::tie(right.key,right.mod);
  }
};

//function return true if this key must not be used as char iput at the same event
using  MenuKeyCallBack = std::function<bool(int key, KeyboardModifier mod,GLMenu& menu)>;

class GLMenu: public GLRenderObject
{
public:
  GLMenu(int x_left_top = 0,
       int y_left_top = 0,
       int width = 0,
       int height = 0,
       const GLTexture2D& texture = GLTexture2D());

  GLMenu& setKeyCallBack(int key, KeyboardModifier mod, const MenuKeyCallBack& call_back);

  GLMenu& setSize(int width, int height);
  GLMenu& setTexture(const GLTexture2D& texture);

  int getActiveIndex() const { return active_index; }
  GLMenu& setActiveIndex(int index);

  template<typename ObjectType>
  GLMenu& addObject(const ObjectType& object);

  template<typename RenderObjectChild>
  std::shared_ptr<RenderObjectChild> getMenuObject(unsigned index, RenderObjectChild object_type);

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

  //position of left top corner of menu
  virtual void setPos(int x, int y) override;
  virtual int posX() const override { return pos.posX(); }
  virtual int posY() const override { return pos.posY(); }

  virtual int height() const override { return pos.height(); }
  virtual int width() const override { return pos.width(); }


  virtual void keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) override;
  virtual void keyRelease(int key, KeyboardModifier mod) override;
  virtual void charInput(int unicode_key) override;

private:
  std::map<MenuHotkey,MenuKeyCallBack> key_call_backs;

  friend class GLMenuItemClicker;
  GLTexture2D texture;
  GLRectangleCoord<GLint> pos;

  size_t active_index;
  std::vector<std::shared_ptr<GLRenderObject>> menu_objects;

  bool active;
};


template<typename RenderObjectType>
GLMenu& GLMenu::addObject(const RenderObjectType& object) {
  static_assert(std::is_base_of<GLRenderObject,RenderObjectType>::value,"GLRenderObject must be a base of \"object\"");
  menu_objects.push_back(std::shared_ptr<GLRenderObject>(new RenderObjectType(object)));
  if(menu_objects.size()) {
    if(!menu_objects[active_index]->isActive()) {
        setActiveIndex(menu_objects.size()-1);
    }
  }
  if (object.posX()==0)
    menu_objects.back()->setPos(pos.posXcenter()-object.width()/2,object.posY());
  return *this;
}

//just little adaptor
//it can press any item in menu (basically designed for buttons)
class GLMenuItemClicker {
  unsigned index;
public:
  GLMenuItemClicker(unsigned item_index): index(item_index) {

  }
  //key,modifier,menu
  bool operator() (int, KeyboardModifier, GLMenu& menu) {
    if (index<menu.menu_objects.size()) {
        auto& o = menu.menu_objects[index];
        o->click(o->posX()+o->width()/2,o->posY()+o->height()/2);
      }
    return true;
  }
};

#endif // MENU_H