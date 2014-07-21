#include "GLlist.h"

#include <stdexcept>

GLList::GLList(const WorldPos &pos_left_top, WorldPos::COORD_TYPE width, unsigned max_visible_items)
{
}


GLList& GLList::setWidth(WorldPos::COORD_TYPE width) {

  return *this;
}

GLList& GLList::setListTexture(const GLTexture2D &texture) {

  return *this;
}

GLList& GLList::setItems(const str_array& items_list) {

  return *this;
}

unsigned GLList::getItemsCount() const {
  return items.size();
}

GLList& GLList::setSelectedItem(int index) {
  if(index>=items.size()) {
      throw std::out_of_range("GLList: no such index in list:"+std::to_string(index));
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

GLList& GLList::setMaxVisibleItems(unsigned count) {

}

unsigned GLList::getMaxVisibleItems() const {
  return max_visible_cout;
}

GLList& GLList::setMinVisibleItemIndex(unsigned index) {

}

unsigned GLList::getFirstVisibleItemIndex() const {

}

GLList& GLList::setSelectedChangedCallBack(const ListSelectedChangedCallBack& call_back) {

}

//GLFontKeeper
void GLList::fontChanged() {

}

void GLList::fontColorChanged() {

}

//GLRenderObject:
void GLList::draw() const {

}

void GLList::setActive(bool active) {

}

bool GLList::isActive() const {

}

bool GLList::canBeActive() const {

}
void GLList::click(const WorldPos &world_pos) {

}

void GLList::mouseDown(const MouseEvent &mouse) {

}

void GLList::mouseUp(const MouseEvent &mouse) {

}

void GLList::hover(const MouseEvent &mouse) {

}

void GLList::unHover() {

}

bool GLList::underMouse(const WorldPos &mouse_pos) const {

}

void GLList::setPos(const WorldPos &world_pos) {

}

WorldPos::COORD_TYPE GLList::posX() const {

}

WorldPos::COORD_TYPE GLList::posY() const {

}

WorldPos::COORD_TYPE GLList::height() const {

}

WorldPos::COORD_TYPE GLList::width() const {

}

void GLList::keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) {

}

void GLList::keyRelease(int key, KeyboardModifier mod) {

}
