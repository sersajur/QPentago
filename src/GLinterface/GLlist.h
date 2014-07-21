#ifndef GLLIST_H
#define GLLIST_H

#include "GLrenderobject.h"
#include "GLtextures.h"
#include "GLrectanglecoord.h"

#include <functional>

class GLList;

using ListSelectedChangedCallBack = std::function<void(int new_index, int old_index, GLList& parent)>;

class GLList: GLRenderObject
{
public:
  explicit GLList(const WorldPos &pos_left_top = WorldPos(0,0),
                  WorldPos::COORD_TYPE width = 0,
                  unsigned max_visible_items=1);

  GLList& setWidth(WorldPos::COORD_TYPE width);

  GLList& setListTexture(const GLTexture2D &texture);

  GLList& setItems(const str_array& items_list);

  unsigned getItemsCount() const;

  GLList& setSelectedItem(int index);

  int getSelectedItemIndex() const;
  const string& getSelectedItem() const;

  GLList& setMaxVisibleItems(unsigned count);
  unsigned getMaxVisibleItems() const;

  GLList& setMinVisibleItemIndex(unsigned index);
  unsigned getFirstVisibleItemIndex() const;

  GLList& setSelectedChangedCallBack(const ListSelectedChangedCallBack& call_back);

//GLRenderObject:
  virtual void draw() const override;

  virtual void setActive(bool active) override;
  virtual bool isActive() const override;
  virtual bool canBeActive() const override;

  virtual void click(const WorldPos &world_pos) override;
  virtual void mouseDown(const MouseEvent &mouse) override;
  virtual void mouseUp(const MouseEvent &mouse) override;
  virtual void hover(const MouseEvent &mouse) override;
  virtual void unHover() {}
  virtual bool underMouse(const WorldPos &mouse_pos) const override;

  virtual void setPos(const WorldPos &world_pos) override;
  virtual WorldPos::COORD_TYPE posX() const override;
  virtual WorldPos::COORD_TYPE posY() const override;
  virtual WorldPos::COORD_TYPE height() const override;
  virtual WorldPos::COORD_TYPE width() const override;

  virtual void keyPress(int key, bool repeat, KeyboardModifier mod, bool &skip_char_input, bool &lock_active) override;
  virtual void keyRelease(int key, KeyboardModifier mod) override;

private:
  GLRectangleCoord pos;

  GLTexture2D texture_item;
};

#endif // GLLIST_H
