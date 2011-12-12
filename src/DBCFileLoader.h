/*
 */

#ifndef DBC_FILE_LOADER_H
#define DBC_FILE_LOADER_H

#include <cassert>

#include "stdint.h"

enum
{
    FT_NA='x',                                              //not used or unknown, 4 byte size
    FT_NA_BYTE='X',                                         //not used or unknown, byte
    FT_STRING='s',                                          //char*
    FT_FLOAT='f',                                           //float
    FT_INT='i',                                             //uint32_t
    FT_BYTE='b',                                            //uint8_t
    FT_SORT='d',                                            //sorted by this field, field is not included
    FT_IND='n',                                             //the same,but parsed to data
    FT_LOGIC='l'                                            //Logical (boolean)
};

class DBCFileLoader
{
    public:
        DBCFileLoader();
        ~DBCFileLoader();

        bool Load(const char *filename, const char *fmt);

        class Record
        {
            public:
                float getFloat(size_t field) const
                {
                    assert(field < file.fieldCount);
                    float val = *reinterpret_cast<float*>(offset+file.GetOffset(field));
                    return val;
                }

                uint32_t getUInt(size_t field) const
                {
                    assert(field < file.fieldCount);
                    uint32_t val = *reinterpret_cast<uint32_t*>(offset+file.GetOffset(field));
                    return val;
                }

                uint8_t getuint8_t(size_t field) const
                {
                    assert(field < file.fieldCount);
                    return *reinterpret_cast<uint8_t*>(offset+file.GetOffset(field));
                }

                const char *getString(size_t field) const
                {
                    assert(field < file.fieldCount);
                    size_t stringOffset = getUInt(field);
                    assert(stringOffset < file.stringSize);
                    return reinterpret_cast<char*>(file.stringTable + stringOffset);
                }

            private:
                Record(DBCFileLoader &file_, unsigned char *offset_): offset(offset_), file(file_) {}
                unsigned char *offset;
                DBCFileLoader &file;

                friend class DBCFileLoader;
        };

        // Get record by id
        Record getRecord(size_t id);
        /// Get begin iterator over records

        uint32_t GetNumRows() const { return recordCount;}
        uint32_t GetCols() const { return fieldCount; }
        uint32_t GetOffset(size_t id) const { return (fieldsOffset != NULL && id < fieldCount) ? fieldsOffset[id] : 0; }
        bool IsLoaded() {return (data!=NULL);}
        char* AutoProduceData(const char* fmt, uint32_t& count, char**& indexTable);
        char* AutoProduceStrings(const char* fmt, char* dataTable);
        static uint32_t GetFormatRecordSize(const char * format, int32_t * index_pos = NULL);

    private:
        uint32_t recordSize;
        uint32_t recordCount;
        uint32_t fieldCount;
        uint32_t stringSize;
        uint32_t *fieldsOffset;
        unsigned char *data;
        unsigned char *stringTable;
};

#endif
