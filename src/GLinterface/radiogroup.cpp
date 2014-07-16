#include "radiogroup.h"

RadioGroup::RadioGroup(
      GLint x_left_top,
      GLint y_left_top,
      GLint width,
      const Texture2D& background):
    texture_background(background),
    texture_radio({Texture2D(":/graphics/radio.png"),
                   Texture2D(":/graphics/radio_checked.png"),
                   Texture2D(":/graphics/radio_pressed.png")}),
    active(false),
    pressed(false),
    selected_index(-1),
    hovered_index(-1) {
  setPos(x_left_top,y_left_top);
  setWidth(width);
}

RadioGroup& RadioGroup::setWidth(GLint width) {
  (void)width;//TODO
  return *this;
}

RadioGroup& RadioGroup::setList(const str_array& list) {
  (void)list;//TODO
  return *this;
}

RadioGroup& RadioGroup::setSelectedIndex(int index) {
  selected_index = index;
  return *this;
}

int RadioGroup::getItemCount() const {
  return items.size();
}

string RadioGroup::getItem(int index) const {
  return items.at(index).getText();
}

int RadioGroup::getSelectedIndex() const {
  return selected_index;
}

string RadioGroup::getSelectedItem() const {
  return selected_index>=0?items[selected_index].getText():L"";
}

void RadioGroup::fontChanged() {

}

void RadioGroup::draw() const {
  //TODO
}

void RadioGroup::setActive(bool new_active) {
  if(!active && items.size()>0) {
      hovered_index = 0;
      active = true;
    } else {
      active = new_active && (items.size()>0);
      hovered_index = (-1*!active) + active*hovered_index;
    }
}

bool RadioGroup::isActive() const {
  return active;
}

void RadioGroup::click(int x, int y) {
  (void)x;(void)y; //TODO
}

void RadioGroup::mouseDown(int x, int y) {
  (void)x;(void)y; //TODO
}

void RadioGroup::mouseUp(int x, int y) {
  (void)x;(void)y; //TODO
}

void RadioGroup::hover(int x, int y) {
  (void)x;(void)y; //TODO
}

void RadioGroup::unHover() {
  //TODO
}

bool RadioGroup::underMouse(int x, int y) const {
  return pos.posInRect(x,y);
}

void RadioGroup::setPos(int x, int y) {
  (void)x;(void)y; //TODO
}

int RadioGroup::posX() const {
  return pos.posX();
}

int RadioGroup::posY() const {
  return pos.posY();
}

int RadioGroup::height() const {
  return pos.height();
}

int RadioGroup::width() const {
  return pos.width();
}

void RadioGroup::keyPress(int key, bool repeat, KeyboardModifier mod) {
  (void)key;
  (void)repeat;
  (void)mod;
  //TODO
}

void RadioGroup::keyRelease(int key, KeyboardModifier mod) {
  (void)key;
  (void)mod;
  //TODO
}
