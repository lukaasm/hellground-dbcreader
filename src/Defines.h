#ifndef H_DEFINES
#define H_DEFINES

#include <FL/Fl_menu_item.h>

#include "Callbacks.h"

Fl_Menu_Item menuitems[] = {
  { "&File",              0, 0, 0, FL_SUBMENU},
    { "&Open File...",    FL_COMMAND + 'o', (Fl_Callback*)Callback::open_dbc},
    { "Export DBC &as...", FL_COMMAND + FL_SHIFT + 's', 0, 0, FL_MENU_DIVIDER},
    { "E&xit", FL_COMMAND + 'q', (Fl_Callback*)Callback::exitp, 0},
    { 0 },
  { 0 }
};

#endif
