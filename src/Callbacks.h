#ifndef H_CALLBACKS
#define H_CALLBACKS

#include <FL/Fl_File_Chooser.h>

#include "DBCFileLoader.h"

namespace Callback
{
    void open_dbc(Fl_Widget*, void*)
    {
        char *newfile = fl_file_chooser("Open File?", "*.dbc", "", 1);
        if (newfile != NULL)
        {
            DBCFileLoader dbc;
            if (dbc.Load(newfile, ""))
                printf("DBC loaded.");
            else
                printf("DBC NOT loaded.");
        }
    }

    void exitp(Fl_Widget*, void*)
    {
        exit(0);
    }
}

#endif
