#include "GLradiogroup.h"

#include <stdexcept>

GLTexture2D GLRadioGroup::texture_blurr;

GLRadioGroup::GLRadioGroup(
      GLint x_left_top,
      GLint y_left_top,
      GLint width,
      const GLTexture2D& background):
    texture_background(background),
    texture_radio({GLTexture2D(":/graphics/radio.png"),
                   GLTexture2D(":/graphics/radio_checked.png"),
                   GLTexture2D(":/graphics/radio_pressed.png")}),
    active(false),
    pressed_index(-1),
    selected_index(-1),
    hovered_index(-1),
    item_height(10) {
  setPos(x_left_top,y_left_top);
  setWidth(width);
}

GLRadioGroup& GLRadioGroup::setWidth(GLint width) {
  pos.setSize(width,pos.height());
  return *this;
}

GLRadioGroup& GLRadioGroup::setItems(const str_array& items_list) {
  setSelectedIndex(-1);
  items.clear();
  for(auto& item: items_list) {
      items.push_back(GLLabel(item,0,0,getFont()));
    }
  pos.setSize(pos.width(),item_height*items.size());
  setPos(posX(),posY());//repos Labels
  return *this;
}

GLRadioGroup& GLRadioGroup::setSelectedChangedCallBack(const RadioGroupSelectedChangedCallBack& call_back) {
  selected_changed_call_back = call_back;
  return *this;
}

GLRadioGroup& GLRadioGroup::setSelectedIndex(int index) {
  if(index<int(items.size())) {
      int old_index = selected_index;
      selected_index = index;
      if(selected_changed_call_back) {
          selected_changed_call_back(old_index,selected_index);
        }
    }
  else
    throw std::out_of_range("GLRadioGroup::setSelectedIndex: new selected index out or range");
  return *this;
}

int GLRadioGroup::getItemCount() const {
  return items.size();
}

const string& GLRadioGroup::getItem(int index) const {
  return items.at(index).getText();
}

int GLRadioGroup::getSelectedIndex() const {
  return selected_index;
}

const string& GLRadioGroup::getSelectedItem() const {
  static const string empty = L"";
  return selected_index>=0?items[selected_index].getText():empty;
}

void GLRadioGroup::fontChanged() {
  item_height = getFontMetrics().height();
  for(auto&i:items) {
      i.setFont(getFont());
    }
}

void GLRadioGroup::draw() const {
  glColor4f(1,1,1,1);
  texture_background.draw(pos.glCoords(),pos.dimension);
  if(hovered_index>=0) {
      decltype(pos) bluer_pos(pos.getLeft(),pos.getTop()+hovered_index*item_height,
                              pos.width(),item_height);
      glColor4f(1,1,1,.33);
      texture_blurr.draw(bluer_pos.glCoords(),bluer_pos.dimension);
    }
  glColor4f(1,1,1,1);
  decltype(pos) chord(pos.getLeft(),pos.getBottom()-item_height,
                      item_height,item_height);
  for(int i=items.size()-1; i>=0; --i) {
      texture_radio[((selected_index==i)&(pressed_index!=i))+(pressed_index==i)*2].draw(chord.glCoords(),chord.dimension);
      items[i].drawCroped(pos.getLeft(),pos.getRight());
      chord.setPos(chord.posX(),chord.posY()-item_height);
    }
}

void GLRadioGroup::setActive(bool new_active) {
  if(new_active && items.size()>0) {
      hovered_index = 0;
      active = true;
    } else {
      active = new_active && (items.size()>0);
      hovered_index = (!active*-1) | (active*hovered_index);
    }
}

bool GLRadioGroup::isActive() const {
  return active;
}

void GLRadioGroup::click(int x, int y) {
  if(underMouse(x,y)) {
      int new_index = std::trunc((y-pos.getTop())/item_height);
      if(selected_index!=new_index){
          setSelectedIndex(new_index);
        }
    }
}

void GLRadioGroup::mouseDown(int x, int y) {
  if(underMouse(x,y)) {
      pressed_index=std::trunc((y-pos.getTop())/item_height);
    }
}

void GLRadioGroup::mouseUp(int x, int y) {
  if(underMouse(x,y)) {
      if(pressed_index==std::trunc((y-pos.getTop())/item_height)) {
          click(x,y);
        }
    }
  pressed_index=-1;
}

void GLRadioGroup::hover(int x, int y) {
  if(underMouse(x,y)) {
      setActive(true);
      hovered_index=std::trunc((y-pos.getTop())/item_height);
    }
}

void GLRadioGroup::unHover() {
  setActive(false);
}

bool GLRadioGroup::underMouse(int x, int y) const {
  return pos.posInRect(x,y);
}

void GLRadioGroup::setPos(int x, int y) {
  pos.setPos(x,y);

  int pos_x = x+item_height;
  int pos_y = y;
  for(auto&i:items) {
      i.setPos(pos_x,pos_y);
      pos_y+=item_height;
    }
}

int GLRadioGroup::posX() const {
  return pos.posX();
}

int GLRadioGroup::posY() const {
  return pos.posY();
}

int GLRadioGroup::height() const {
  return pos.height();
}

int GLRadioGroup::width() const {
  return pos.width();
}

void GLRadioGroup::keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) {
  (void)key;
  (void)repeat;
  (void)mod;
  (void)skip_char_input;
  (void)lock_active;


  //TODO
}

void GLRadioGroup::keyRelease(int key, KeyboardModifier mod) {
  (void)key;
  (void)mod;
  //TODO
}
