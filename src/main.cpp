/*
 */

#include <iostream>
#include <fstream>
#include <string>

#include <vector>

#include "DBCFileLoader.h"

const char* Ext[] =
{
    "sql", // 0
    "csv"  // 1
};
#define MAX_EXT_SUPPORT 2

using namespace std;

struct FieldInfo
{
    string fieldName;
    string fieldType;

    bool operator=(string& input)
    {
        // invalid line format :p
        size_t pos = input.find(',');
        
        if (pos == string::npos)
            return false;

        fieldName = input.substr(0, pos);
        fieldType = input.substr(pos+1);

        if (fieldName == "" || fieldType == "")
            return false;

        return true;
    }
};

class DBCTemplate
{
    public:
        ~DBCTemplate() { CleanupFieldInfo(); }

        bool CreateTemplate(string fileName)
        {
            ifstream in(fileName);
            if (in.fail())
                return false;

            FieldInfo *fi;
            string line;
            while (getline(in, line))
            {
                fi = new FieldInfo;
                if (*fi = line)
                    fInfo.push_back(fi);
                else
                {
                    delete fi;
                    CleanupFieldInfo();
                    return false;
                }
            }

            return true;
        }

        void GetDBCFormat(string &fmt)
        {
            fmt.clear();

            for (vector<FieldInfo *>::iterator i = fInfo.begin(); i != fInfo.end(); ++i)
                fmt += (*i)->fieldType;
        }

        void CleanupFieldInfo()
        {
            for (vector<FieldInfo *>::iterator i = FieldsInfo().begin(); i != FieldsInfo().end(); ++i)
                delete *i;

            fInfo.clear();
        }

        vector<FieldInfo *> &FieldsInfo() { return fInfo; }

    private:
        vector<FieldInfo *> fInfo;
};

class Extension
{
    public:
        ~Extension()
        {
            if (_firstLine)
                delete _firstLine;

             if (_lastLine)
                 delete _lastLine;

             if (_rowSeparator)
                 delete _rowSeparator;

             if (_colSeparator)
                 delete _colSeparator;
        }

        virtual string &FirstLine(int) =0;
        virtual string &LastLine() =0;

        virtual string &RowSeparator() =0;
        virtual string &ColumnSeparator() =0;

        void SetName(const char* name) { _name = name; }
        string &GetName() { return _name; }

        void SetTemplate(DBCTemplate *templt) { _template = templt; }
        DBCTemplate *GetTemplate() const { return _template; }

    protected:
         Extension() : _firstLine(NULL), _lastLine(NULL), _rowSeparator(NULL), _colSeparator(NULL), _template(NULL) {}
         Extension(Extension&);

         string *_firstLine;
         string *_lastLine;
         string *_rowSeparator;
         string *_colSeparator;

    private:
         DBCTemplate *_template;
         string _name;
};

class SQLExtension : public Extension
{
    public:
        SQLExtension() : Extension() { SetName(".sql"); }

        string &FirstLine(int cols)
        {
            if (_firstLine)
                return *_firstLine;

            string *temp = new string;
            *temp += "DROP TABLE IF EXISTS `table_name`;\n";
            *temp += "CREATE TABLE `table_name` (\n";
            for (int i = 0; i < cols;)
            {
                if (DBCTemplate *t = GetTemplate())
                {
                    if (t->FieldsInfo()[i]->fieldType.find('x') == string::npos)
                    {
                        *temp += "`";
                        *temp += t->FieldsInfo()[i]->fieldName;
                    }
                    else
                    {
                        ++i;
                        continue;
                    }
                }
                else
                {
                    *temp += "`";
                    char str[10];
                    *temp += itoa(i, str, 10);
                }

                // still need to create table with proper column format
                *temp += "` bigint(20) unsigned";
                if (++i < cols)
                     *temp += ",\n";
                else
                    *temp += "\n) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;\n";
            }
            *temp += "INSERT INTO `table_name` VALUES \n(";

            _firstLine = temp;
            return *_firstLine;
        };

        string &LastLine()
        {
            if (_lastLine)
                return *_lastLine;

            string *temp = new string;
            *temp += ");";

            _lastLine = temp;
            return *_lastLine;
        };

        string &RowSeparator()
        {
            if (_rowSeparator)
                return *_rowSeparator;

            string *temp = new string;
            *temp += "), \n(";

            _rowSeparator = temp;
            return *_rowSeparator;
        };

        string &ColumnSeparator()
        {
            if (_colSeparator)
                return *_colSeparator;

            string *temp = new string;
            *temp += ", ";

            _colSeparator = temp;
            return *_colSeparator;
        };
};

class CSVExtension : public Extension
{
    public:
        CSVExtension() : Extension() { SetName(".csv"); }

        string &FirstLine(int cols)
        {
            if (_firstLine)
                return *_firstLine;

            string *temp = new string;
            for (int i = 0; i < cols;)
            {
                if (DBCTemplate *t = GetTemplate())
                {
                    if (t->FieldsInfo()[i]->fieldType.find('x') == string::npos)
                        *temp += t->FieldsInfo()[i]->fieldName;
                    else
                    {
                        ++i;
                        continue;
                    }
                }
                else
                {
                    char str[10];
                    *temp += itoa(i, str, 10);
                }

                if (++i < cols)
                    *temp += ", ";
            }
            *temp += "\n";

            _firstLine = temp;
            return *_firstLine;
        };

        string &LastLine()
        {
            if (_lastLine)
                return *_lastLine;

            string *temp = new string;
            *temp += "";

            _lastLine = temp;
            return *_lastLine;
        };

        string &RowSeparator()
        {
            if (_rowSeparator)
                return *_rowSeparator;

            string *temp = new string;
            *temp += "\n";

            _rowSeparator = temp;
            return *_rowSeparator;
        };

        string &ColumnSeparator()
        {
            if (_colSeparator)
                return *_colSeparator;

            string *temp = new string;
            *temp += ", ";

            _colSeparator = temp;
            return *_colSeparator;
        };
};

namespace ExtensionSelector
{
    static Extension *getExtension(string type)
    {
        if (type.find("csv") != string::npos)
            return new CSVExtension;

        if (type.find("sql") != string::npos)
            return new SQLExtension;

        return NULL;
    }
};

int main(int argc, char* argv[])
{
    cout << "*** DBCReader v 0.01 by lukaasm" << endl;
    cout << endl;
    cout << "*** DBCReader isn't exacly a reader, it's more like exporter." << endl;
    cout << "*** It was developed, because I just needed small app that" << endl;
    cout << "*** will convert DBC files to different DB format." << endl;
    cout << "*** I hope it will be useful also for you :]" << endl;
    cout << "*** With regards, lukaasm" << endl << endl << endl;

    string dbcFile = "";
    if (argc >= 2)
        dbcFile = argv[1];

    Extension *ext = NULL;

    do
    {
        if (dbcFile == "")
        {
            cout << "DBC file: ";
            getline(cin, dbcFile);
        }
        else
        {
            cout << "Output format:" << endl;
            for (int i = 0; i < MAX_EXT_SUPPORT; i++)
                cout << i+1 << ". " << Ext[i] << endl;

            int selectExt = 0;
            cin >> selectExt;

            if (selectExt-1 < MAX_EXT_SUPPORT)
                ext = ExtensionSelector::getExtension(Ext[selectExt-1]);
        }
    }
    while (ext == NULL);

    string fileToExport = dbcFile + ext->GetName();
    cout << "DBC will be saved as: " << fileToExport << endl;

    DBCTemplate templt;
    bool useTemplate = templt.CreateTemplate(dbcFile + ".ini");

    string dbcFormat = "";
    if (useTemplate)
    {
        templt.GetDBCFormat(dbcFormat);
        ext->SetTemplate(&templt);
    }

    DBCFileLoader dbc;
    if (!dbc.Load(dbcFile.c_str(), dbcFormat.c_str()))
    {
        cout << "Problem with loading file: " << dbcFile << endl;
        return -1;
    }

    if (useTemplate && templt.FieldsInfo().size() != dbc.GetCols())
    {
        cout << "Something is wrong with your template definition, fields count isn't same as in DBC." << endl;
        cout << "Template cols: " << templt.FieldsInfo().size() << ", DBC cols: " << dbc.GetCols() << endl << endl;
        cout << "Fix or remove your template definition and re-run DBCReader." << endl;
        return -1;
    }

    fstream outputFile;
    outputFile.open(fileToExport, ios_base::out);
    if (outputFile.fail())
    {
        cout << "Problem occured while creating file: " << fileToExport << endl;
        return -1;
    }

    outputFile << ext->FirstLine(dbc.GetCols());
    for (uint16_t i = 0; i < dbc.GetNumRows();)
    {
        for (uint16_t j = 0; j < dbc.GetCols();)
        {
            // use template to obtain proper data
            if (!useTemplate || templt.FieldsInfo()[j]->fieldType.find('x') == string::npos)
            {
                outputFile << dbc.getRecord(i).getUInt(j);

                if (++j <  dbc.GetCols())
                    outputFile << ext->ColumnSeparator();
            }
            else
                ++j;
        }

        if (++i <  dbc.GetNumRows())
            outputFile << ext->RowSeparator();
    }
    outputFile << ext->LastLine();

    outputFile.close();

   cout << endl << " ---------------- " << endl;
   cout << "DBC conversion ended: " << endl;
   cout << "Exported: " << dbc.GetNumRows() << " rows and " << dbc.GetCols() << " columns as " << ext->GetName() << " file." << endl;

   delete ext;
   return 0;
}
