#include "GLmenu.h"
#include <type_traits>

GLMenu::GLMenu(const WorldPos &pos_left_top,
           const WorldPos &vector_size,
           const GLTexture2D& texture): active_index(0), active(true) {
  setPos(pos_left_top);
  setSize(vector_size);
  setTexture(texture);
}

GLMenu& GLMenu::setKeyCallBack(int key, KeyboardModifier mod, const MenuKeyCallBack &call_back) {
  key_call_backs[MenuHotkey{key,mod}] = call_back;
  return *this;
}

GLMenu& GLMenu::setSize(const WorldPos &v_size) {
  pos.setSize(v_size);
  return *this;
}

GLMenu& GLMenu::setTexture(const GLTexture2D &texture) {
  this->texture = texture;
  return *this;
}

GLMenu& GLMenu::setActiveIndex(int index) {
  if(menu_objects[index]->canBeActive()) {
    menu_objects[active_index]->setActive(false);
    menu_objects[index]->setActive(true);
    active_index = index;
  }
  return *this;
}

template<typename RenderObjectChild>
std::shared_ptr<RenderObjectChild> GLMenu::getMenuObject(unsigned index, RenderObjectChild object_type) {
    return std::dynamic_pointer_cast<RenderObjectChild>(menu_objects[index]);
}

void GLMenu::setActive(bool active) {
  this->active=active;
}

bool GLMenu::isActive() const {
  return active;
}

bool GLMenu::canBeActive() const {
  return true;
}

void GLMenu::draw() const {

  GLdouble sx = pos.width(),sy = pos.height();
  GLdouble cro1 = 0.01, cro2=4*cro1;
  GLdouble bcr = cro1*sx, bc2 = cro2*sy;
  sx/=90;
  sy/=90;
  GLdouble position[12*2] = {

  GLdouble(pos.getLeft()),pos.getTop()    +bc2,
    pos.getLeft()    +bcr,pos.getTop()    +bcr,
    pos.getLeft()    +bc2,GLdouble(pos.getTop()),

    pos.getRight()-bc2	 ,GLdouble(pos.getTop()),
    pos.getRight()-bcr	 ,pos.getTop()    +bcr,
 GLdouble(pos.getRight()),pos.getTop()    +bc2,

 GLdouble(pos.getRight()),pos.getBottom()-bc2 ,
    pos.getRight()-bcr	 ,pos.getBottom()-bcr ,
    pos.getRight()-bc2	 ,GLdouble(pos.getBottom()),

    pos.getLeft()    +bc2,GLdouble(pos.getBottom()),
    pos.getLeft()    +bcr,pos.getBottom()-bcr ,
  GLdouble(pos.getLeft()),pos.getBottom()-bc2
  };

  GLdouble crop[12*2] = {
    GLdouble(0.0     )*sx, GLdouble(1.0-cro2)*sy,
    GLdouble(0.0+cro1)*sx, GLdouble(1.0-cro1)*sy,
    GLdouble(0.0+cro2)*sx, GLdouble(1.0     )*sy,

    GLdouble(1.0-cro2)*sx, GLdouble(1.0     )*sy,
    GLdouble(1.0-cro1)*sx, GLdouble(1.0-cro1)*sy,
    GLdouble(1.0     )*sx, GLdouble(1.0-cro2)*sy,

    GLdouble(1.0     )*sx, GLdouble(0.0+cro2)*sy,
    GLdouble(1.0-cro1)*sx, GLdouble(0.0+cro1)*sy,
    GLdouble(1.0-cro2)*sx, GLdouble(0.0     )*sy,

    GLdouble(0.0+cro2)*sx, GLdouble(0.0     )*sy,
    GLdouble(0.0+cro1)*sx, GLdouble(0.0+cro1)*sy,
    GLdouble(0.0     )*sx, GLdouble(0.0+cro2)*sy
  };

  texture.draw(position,2,crop,12);

  for(auto o: menu_objects) {
    o->draw();
  }
}

void GLMenu::click(const WorldPos &pos) {
  for(auto o: menu_objects) {
    if (o->underMouse(pos))
      o->click(pos);
  }
}

void GLMenu::mouseDown(const MouseEvent &mouse) {
  for(auto o: menu_objects) {
    if(o->underMouse(mouse.pos)) {
      o->mouseDown(mouse);
    }
  }
}

void GLMenu::mouseUp(const MouseEvent &mouse) {
  for(auto o: menu_objects) {
    o->mouseUp(mouse);
  }
}

void GLMenu::hover(const MouseEvent &mouse) {
  setActive(true);
  for(size_t i=0; i<menu_objects.size(); i++) {
    if (menu_objects[i]->underMouse(mouse.pos)) {
      menu_objects[i]->hover(MouseEvent{mouse.pos});
      if(active_index!=i && menu_objects[i]->isActive()) {
        menu_objects[active_index]->setActive(false);
        menu_objects[active_index]->unHover();
        active_index = i;
      } else {
          if (!menu_objects[i]->isActive())
            menu_objects[i]->unHover();
        }
    }
  }
}

void GLMenu::unHover() {
  setActive(false);
}

bool GLMenu::underMouse(const WorldPos &m_pos) const {
  return pos.posInRect(m_pos);
}

void GLMenu::setPos(const WorldPos &w_pos) {
  pos.setPos(w_pos);
  //TODO: move all objects
}

void GLMenu::keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) {
  if(key_call_backs.find(MenuHotkey{key,mod})!=key_call_backs.end()) {
      skip_char_input = key_call_backs[MenuHotkey{key,mod}](key,mod,*this);
    }

  for (auto& o: menu_objects) {
      if(o->isActive()) {
          bool skip = false;
          o->keyPress(key,repeat,mod,skip,lock_active);
          skip_char_input |= skip;
          break;
        }
    }

  if(lock_active && !(mod&MD_CONTROL)) return;

  if (mod == MD_NONE || mod&(MD_SHIFT | MD_CONTROL)) {
    if (key==Qt::Key_Tab) key = Qt::Key_Down; else
    //Shift+Tab
    if (key==Qt::Key_Backtab) key = Qt::Key_Up;
  }

  unsigned find_count = menu_objects.size();
  if(!find_count) return;
  switch (key) {
    unsigned index;
    case Qt::Key_Down:
      index = active_index;
      do {
        if (index==menu_objects.size()-1)
          index=0;
        else
          index++;
        find_count--;
      } while(find_count && !(menu_objects[index]->canBeActive()));
      setActiveIndex(index);
      skip_char_input |= true;
      break;
    case Qt::Key_Up:
      find_count = menu_objects.size();
      index = active_index;
      do {
        if (index==0)
          index=menu_objects.size()-1;
        else
          index--;
        find_count--;
      } while(find_count && !(menu_objects[index]->canBeActive()));
      setActiveIndex(index);
      skip_char_input |= true;
      break;
  }
}


void GLMenu::keyRelease(int key, KeyboardModifier mod) {
  for (auto& o: menu_objects) {
      if(o->isActive()) {
          o->keyRelease(key,mod);
          break;
        }
    }
}

void GLMenu::charInput(int unicode_key) {
  for (auto& o: menu_objects) {
      if(o->isActive()) {
          o->charInput(unicode_key);
          break;
        }
    }
}

