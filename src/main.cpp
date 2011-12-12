/*
 */

#include <iostream>
#include "DBCFileLoader.h"

static const char Spell_243_fmt[]="nixiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifxiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiffffffiiiiiiiiiiiiiiiiiiiiifffiiiiiiiiiiiiiiifffixiixssssssssssssssssxssssssssssssssssxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxiiiiiiiiiixfffixxiiii";

int main()
{
    DBCFileLoader dbc;
    if (!dbc.Load("D:\\WoW enGB TBC\\dbc\\Spell.dbc", Spell_243_fmt))
        return -1;

    std::cout << "Loaded: rows(" << dbc.GetNumRows() << "), cols(" << dbc.GetCols() << ") " << std::endl;
    return 1;
}
