/*
 */

#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Menu_Bar.h>

#include "Defines.h"

const char Spell_243_fmt[]="nixiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifxiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiffffffiiiiiiiiiiiiiiiiiiiiifffiiiiiiiiiiiiiiifffixiixssssssssssssssssxssssssssssssssssxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxiiiiiiiiiixfffixxiiii";

const char* ext[] =
{
    ".sql",
    ".csv"
};
#define MAX_EXT_SUPPORT 2

int main(int argc, char* argv[])
{
    Fl_Window mainWindow(300, 200, "DBCReader by lukaasm");
    mainWindow.begin();
    {
        Fl_Menu_Bar *menuBar = new Fl_Menu_Bar(0, 0, 640, 30);
        menuBar->copy(menuitems);
    }
    mainWindow.end();
    mainWindow.show();
    return Fl::run();
}
