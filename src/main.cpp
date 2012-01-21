/*
 */

#include <iostream>
#include <fstream>
#include <string>

#include "DBCFileLoader.h"

const char* ext[] =
{
    ".sql", // 0
    ".csv"  // 1
};
#define MAX_EXT_SUPPORT 2

using namespace std;

void GetFirstLineFor(fstream &file, const char* ext2, uint32_t cols)
{
    for (uint8_t i = 0; i < MAX_EXT_SUPPORT; i++)
    {
        if (strstr(ext2, ext[i]))
        {
            switch (i)
            {
                case 0: // sql
                    file << "DROP TABLE IF EXISTS `table_name`;\n";
                    file << "CREATE TABLE `table_name` (\n";
                    for (uint16_t i = 0; i < cols;)
                    {
                        file << "`" << i << "` bigint(20) unsigned";
                        if (++i < cols)
                            file << ",\n";
                        else
                            file << "\n) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;\n";
                    }
                    file << "INSERT INTO `table_name` VALUES \n(";
                    return;
                case 1: // csv
                    for (uint16_t i = 0; i < cols; i++)
                        file << i << ", ";

                    file << "\n";
                    return;
            }
        }
    }
}

void GetLastLineFor(fstream &file, const char* ext2)
{
    for (uint8_t i = 0; i < MAX_EXT_SUPPORT; i++)
    {
        if (strstr(ext2, ext[i]))
        {
            switch (i)
            {
                case 0: // sql
                    file << ");";
                    return;
                case 1: // csv
                    file << "";
                    return;
            }
        }
    }
}

void GetRowSeparatorFor(fstream &file, const char* ext2)
{
    for (uint8_t i = 0; i < MAX_EXT_SUPPORT; i++)
    {
        if (strstr(ext2, ext[i]))
        {
            switch (i)
            {
                case 0: // sql
                    file << "), \n(";
                    return;
                case 1: // csv
                    file << "\n";
                    return;
            }
        }
    }
}

void GetColumnSeparatorFor(fstream &file, const char* ext2)
{
    for (uint8_t i = 0; i < MAX_EXT_SUPPORT; i++)
    {
        if (strstr(ext2, ext[i]))
        {
            switch (i)
            {
                case 0: // sql
                case 1: // csv
                    file << ", ";
            }
        }
    }
}


int main(int argc, char* argv[])
{
    cout << "*** DBCReader v 0.01 by lukaasm" << endl;
    cout << endl;
    cout << "*** DBCReader isn't exacly reader, it is more like exporter." << endl;
    cout << "*** It was developed, because author needed small app that" << endl;
    cout << "*** will convert DBC files to different DB format." << endl;
    cout << "*** I hope it will be useful also for you :]" << endl;
    cout << "*** With regards, lukaasm" << endl << endl << endl;

    string choosenExt = "";

    string fileToExport = "";
    if (argc >= 2)
        fileToExport = argv[1];

    bool supportedExt = false;

    do
    {
        if (fileToExport == "")
        {
           cout << "DBC file: ";
           cin >> fileToExport;
        }
        else if (choosenExt == "")
        {
            cout << "Output format:" << endl;
            for (int8_t i = 0; i < MAX_EXT_SUPPORT; i++)
                cout << i+1 << ". " << ext[i] << endl;

            int selectExt = 0;
            cin >> selectExt;

            --selectExt;

            if (selectExt >= 0 && selectExt < MAX_EXT_SUPPORT)
            {
                supportedExt = true;
                choosenExt = ext[selectExt];
                cout << "Choosen extension: " << choosenExt << endl;
            }
            else
                cout << "Choose valid extension." << endl;
        }
    }
    while (!supportedExt);


    DBCFileLoader dbc;
    if (!dbc.Load(fileToExport.c_str(), ""))
    {
        cout << "Specified file: " << fileToExport << " cannot be loaded." << endl;
        return -1;
    }

    fileToExport = fileToExport + choosenExt;
    cout << "DBC will be saved as: " << fileToExport << endl;

    fstream outputFile;
    outputFile.open(fileToExport, ios_base::out);
    if (outputFile.fail())
    {
        cout << "Problem occured while creating file: " << fileToExport << endl;
        return -1;
    }

    GetFirstLineFor(outputFile, choosenExt.c_str(), dbc.GetCols());
    for (uint16_t i = 0; i < dbc.GetNumRows();)
    {
        for (uint16_t j = 0; j < dbc.GetCols();)
        {
            outputFile << dbc.getRecord(i).getUInt(j);

            if (++j <  dbc.GetCols())
                GetColumnSeparatorFor(outputFile, choosenExt.c_str());
        }

        if (++i <  dbc.GetNumRows())
            GetRowSeparatorFor(outputFile, choosenExt.c_str());
    }

    GetLastLineFor(outputFile, choosenExt.c_str());
    outputFile.close();

   cout << endl << " ---------------- " << endl;
   cout << "DBC conversion ended: " << endl;
   cout << "Exported: " << dbc.GetNumRows() << " rows and " << dbc.GetCols() << " columns as " << choosenExt << " file." << endl;

   return 0;
}
