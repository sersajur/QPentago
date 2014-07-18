#include "GLmenu.h"
#include <type_traits>

GLMenu::GLMenu(int x_getLeft_getTop,
           int y_getLeft_getTop,
           int width,
           int height,
           const GLTexture2D& texture): active_index(0), active(true) {
  setPos(x_getLeft_getTop,y_getLeft_getTop);
  setSize(width, height);
  setTexture(texture);
}

GLMenu& GLMenu::setKeyCallBack(int key, const std::function<MenuKeyCallBack>& call_back) {
  key_call_backs[key] = call_back;
  return *this;
}

GLMenu& GLMenu::setSize(int width, int height) {
  pos.setSize(width,height);
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

void GLMenu::click(int x, int y) {
  for(auto o: menu_objects) {
    if (o->underMouse(x,y))
      o->click(x,y);
  }
}

void GLMenu::mouseDown(int x, int y) {
  for(auto o: menu_objects) {
    if(o->underMouse(x,y)) {
      o->mouseDown(x,y);
    }
  }
}

void GLMenu::mouseUp(int x, int y) {
  for(auto o: menu_objects) {
    o->mouseUp(x,y);
  }
}

void GLMenu::hover(int x, int y) {
  setActive(true);
  for(size_t i=0; i<menu_objects.size(); i++) {
    if (menu_objects[i]->underMouse(x,y)) {
      menu_objects[i]->hover(x,y);
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

bool GLMenu::underMouse(int x, int y) const {
  return pos.posInRect(x,y);
}

void GLMenu::setPos(int x, int y) {
  pos.setPos(x,y);
}

void GLMenu::keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input) {
  if(key_call_backs.find(key)!=key_call_backs.end()) {
      skip_char_input = key_call_backs[key](key,mod,*this);
    }
  if (mod == MD_NONE || mod == MD_SHIFT) {
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
      skip_char_input = true;
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
      break;
    default:
      for (auto& o: menu_objects) {
          if(o->isActive()) {
              o->keyPress(key,repeat,mod,skip_char_input);
              break;
            }
        }
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

