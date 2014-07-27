#include "GLlist.h"

#include <stdexcept>

GLTexture2D GLList::texture_blurr;

GLList::GLList(const WorldPos &pos_left_top, WorldPos::COORD_TYPE width, unsigned max_visible_items):
    max_visible_cout(max_visible_items) {
  numbers.resize(2);
  numbers.front().setCaption(L"");
  numbers.back().setCaption(L"");
  fontChanged();//to resize elements
  setPos(pos_left_top);
  setWidth(width);
  go_up.setCaption(L"up");
  go_down.setCaption(L"down");
}


GLList& GLList::setWidth(WorldPos::COORD_TYPE width) {
  pos.setSize({width,pos.height()});
  for(auto &o:items) {
      o.setSize({width-numbers[0].width(),o.height()});
    }
  go_down.setSize({width-numbers[0].width(),go_down.height()});
  go_up.setSize({width-numbers[0].width(),go_up.height()});
  return *this;
}

GLList& GLList::setItemsTexture(const GLTexture2D &texture) {
  texture_item = texture;
  for(auto &i: items) {
      i.setTexture(texture);
    }
  go_down.setTexture(texture);
  go_up.setTexture(texture);
  return *this;
}

GLList& GLList::setNumbersTexture(const GLTexture2D &texture) {
  texture_number = texture;
  for(auto &i: numbers) {
      i.setTexture(texture);
    }
  return *this;
}

GLList& GLList::setItems(const str_array& items_list) {
  first_visible = 0;
  //numbers:
  numbers.resize(items_list.size()+2);
  numbers.back().setCaption(L"");
  setNumbersTexture(texture_number);
  resetNumbersWidth();
  for(unsigned i=1; i<=numbers.size()-1; i++) {
      numbers[i].setCaption(to_string(i));
    }
  //items:
  items.resize(items_list.size());
  setItemsTexture(texture_item);
  for(unsigned i=0; i<items_list.size(); i++) {
      items[i].setCaption(items_list[i]);
    }
  //all:
  setWidth(pos.width());//set the width of all items
  setPos(pos.pos());//set pos for all elements
  return *this;
}

unsigned GLList::getItemsCount() const {
  return items.size();
}

GLList& GLList::setSelectedItem(int index) {
  if(index>=(int)items.size()) {
      throw std::out_of_range("GLList: no such item in this list: index = "+std::to_string(index));
    }
  selected_index = index;
  return *this;
}

int GLList::getSelectedItemIndex() const {
  return selected_index;
}

const string& GLList::getSelectedItem() const {
  static const string empty;
  return selected_index>=0?items[selected_index].getCaption():empty;
}


const string& GLList::getItem(unsigned index) const {
  if(index>=items.size()) {
      throw std::out_of_range("GLList: no such item in this list: index = "+std::to_string(index));
    }
  return items[index].getCaption();
}

GLList& GLList::setMaxVisibleItems(unsigned count) {
  max_visible_cout = count;
  pos.setSize({pos.width(),item_height*(items.size()+1)});
  return *this;
}

unsigned GLList::getMaxVisibleItems() const {
  return max_visible_cout;
}

GLList& GLList::setMinVisibleItemIndex(unsigned index) {
  if(index>=items.size()) {
      throw std::out_of_range("GLList: no such item in this list: index = "+std::to_string(index));
    }
  first_visible = std::max((unsigned)0,std::min(index,(items.size()-max_visible_cout)));
  return *this;
}

unsigned GLList::getFirstVisibleItemIndex() const {
  return first_visible;
}

GLList& GLList::setSelectedChangedCallBack(const ListSelectedChangedCallBack& call_back) {
  selected_changed_call_back = call_back;
  return *this;
}

void GLList::resetObjectsHeight() {
  go_up.setSize({go_up.width(),item_height/2});
  go_down.setSize({go_down.width(),item_height/2});
  numbers.front().setSize({numbers.front().width(),item_height/2});
  numbers.back().setSize({numbers.back().width(),item_height/2});
  for(unsigned i=1; i<numbers.size()-1; i++) {
      numbers[i].setSize({numbers[i].width(),item_height});
    }
  for(auto &i: items) {
      i.setSize({i.width(),item_height});
    }
}

void GLList::resetNumbersWidth() {
  WorldPos::COORD_TYPE num_wid=getFontMetrics().width(QString::number(items.size()))+
      getFontMetrics().averageCharWidth();
  for(auto &number:numbers) {
      number.setSize({num_wid,number.height()});
    }
}

void GLList::resetObjectsPos() {
  WorldPos x_y(pos.pos());
  numbers.front().setPos(x_y);
  x_y.y += numbers.front().height();
  unsigned i;
  for(i=first_visible+1; (i<numbers.size()-1) && (i<first_visible+max_visible_cout+1); i++) {
      numbers[i].setPos(x_y);
      x_y.y += numbers[i].height();
    }
  while(i<first_visible+max_visible_cout+1) {
      x_y.y += item_height; i++;
    }
  numbers.back().setPos(x_y);

  x_y = pos.pos();
  x_y.x += numbers.front().width();
  go_up.setPos(x_y);
  x_y.y += go_up.height();
  for(i=first_visible; i<items.size() && i<first_visible+max_visible_cout; i++) {
      items[i].setPos(x_y);
      x_y.y += numbers[i+1].height();
    }
  while(i<first_visible+max_visible_cout) {
      x_y.y += item_height; i++;
    }
  go_down.setPos(x_y);
}

//GLFontKeeper:
void GLList::fontChanged() {
  item_height = 2 * getFontMetrics().height();
  setMaxVisibleItems(max_visible_cout);//reset height
  resetNumbersWidth();
  resetObjectsHeight();
  resetObjectsPos();
  for(unsigned i=1; i<numbers.size()-1; i++) {
      numbers[i].setFont(getFont());
    }
  for(auto &i: items) {
      i.setFont(getFont());
    }
}

void GLList::fontColorChanged() {
  go_up.setFontColor4iv(getFontColor());
  go_down.setFontColor4iv(getFontColor());
  for(auto &i: items) {
      i.setFontColor4iv(getFontColor());
    }
  for(auto &number: numbers) {
      number.setFontColor4iv(getFontColor());
    }
}

//GLRenderObject:
void GLList::draw() const {
  go_up.draw();
  go_down.draw();
  numbers.front().draw();
  numbers.back().draw();
  for(unsigned i=first_visible+1; i<numbers.size()-1 && i<first_visible+max_visible_cout+1; i++) {
      numbers[i].draw();
    }
  for(unsigned i=first_visible; i<items.size() && i<first_visible+max_visible_cout; i++) {
      items[i].draw();
    }
}

void GLList::setActive(bool active) {
  (void)active;
}

bool GLList::isActive() const {
  return hovered_index>=0;
}

bool GLList::canBeActive() const {
  return true;
}

void GLList::click(const WorldPos &world_pos) {
  (void)world_pos;
}

void GLList::mouseDown(const MouseEvent &mouse) {
  (void)mouse;
}

void GLList::mouseUp(const MouseEvent &mouse) {
  (void)mouse;
}

void GLList::hover(const MouseEvent &mouse) {
  (void)mouse;
}

void GLList::unHover() {

}

bool GLList::underMouse(const WorldPos &mouse_pos) const {
  return pos.posInRect(mouse_pos);
}

void GLList::setPos(const WorldPos &world_pos) {
  pos.setPos(world_pos);
  resetObjectsPos();
}

WorldPos::COORD_TYPE GLList::posX() const {
  return pos.posX();
}

WorldPos::COORD_TYPE GLList::posY() const {
  return pos.posX();
}

WorldPos::COORD_TYPE GLList::height() const {
  return pos.height();
}

WorldPos::COORD_TYPE GLList::width() const {
  return pos.width();
}

void GLList::keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) {
  (void)key;
  (void)mod;
  (void)repeat;
  (void)skip_char_input;
  (void)lock_active;
}

void GLList::keyRelease(int key, KeyboardModifier mod) {
  (void)key;
  (void)mod;
}
