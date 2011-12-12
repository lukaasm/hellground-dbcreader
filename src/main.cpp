/*
 */

#include <iostream>
#include <sstream>

#include "DBCFileLoader.h"

const char Spell_243_fmt[]="nixiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifxiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiffffffiiiiiiiiiiiiiiiiiiiiifffiiiiiiiiiiiiiiifffixiixssssssssssssssssxssssssssssssssssxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxiiiiiiiiiixfffixxiiii";

enum eExt
{
    EXT_SQL  = 0,
    EXT_CSV  = 1,

    EXT_NONE = -1,
};

#define MAX_EXT_SUPPORT 2

const char* ext[MAX_EXT_SUPPORT] =
{
    ".sql",
    ".csv"
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "There is NOT enough parameters specified."
                  << std::endl
                  << "Usage: name.exe inputFile.dbc outputFile.EXT where EXT defines to witch format DBC will be converted."
                  << std::endl;
        return -1;
    }

    eExt extension = EXT_NONE;
    for (uint8_t i = 0; i < MAX_EXT_SUPPORT; ++i)
    {
        if (strstr(argv[2], ext[i]))
        {
            extension = eExt(i);
            break;
        }
    }

    if (extension == EXT_NONE)
    {
        std::cout << "Unsuported extension. Please change to supported one: " << std::endl
                  << "Supported ones are: ";

        for (uint8_t i = 0; i < MAX_EXT_SUPPORT; ++i)
            std::cout << ext[i] << " ";

        return -1;
    }

    DBCFileLoader dbc;
    if (!dbc.Load(argv[1], Spell_243_fmt))
    {
        std::cout << "Cannot load file: " << argv[1] << std::endl;
        return -1;
    }

    FILE* pFile = fopen (argv[2],"w+");
    if (!pFile)
    {
        std::cout << "Can NOT create file(" << argv[2] << ") !!!" << std::endl;
        return -1;
    }


    fclose(pFile);

    std::cout << "Loaded: rows(" << dbc.GetNumRows() << "), cols(" << dbc.GetCols() << ") " << std::endl;
    return 1;
}
