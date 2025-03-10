// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "input/dynamicdatareader.h"
#include "input/inputrecord.h"
#include "error/error.h"

#include <memory>
#include <fstream>

namespace oofem {
DynamicDataReader :: DynamicDataReader(std :: string name) : DataReader(), name(std :: move(name))
{
    this->it = recordList.end();
}

DynamicDataReader :: ~DynamicDataReader()
{
}

void
DynamicDataReader :: insertInputRecord(InputRecordType type, std::unique_ptr<InputRecord> record)
{
    // Should care about the record type, but its a hassle.
    this->recordList.push_back(std::move(record));
    this->it = recordList.end();
}

InputRecord &
DynamicDataReader :: giveInputRecord(InputRecordType typeId, int recordId)
{
    // Ignores recordId in favor of having a dynamic list (just incremental access). typeId could be supported, but its a hassle.
    // The txt data reader makes the same assumptions.
    if ( this->it == this->recordList.end() ) {
        this->it = this->recordList.begin();
    } else {
        ++this->it;
    }
    return **(this->it);
}

bool
DynamicDataReader :: peakNext(const std :: string &keyword)
{
    std :: string nextKey;
    auto temp = this->it;
    temp++;
    (*temp)->giveRecordKeywordField(nextKey);
    return keyword.compare( nextKey ) == 0;
}

void
DynamicDataReader :: finish()
{
    this->recordList.clear();
}

void
DynamicDataReader :: writeToFile(const char *fileName)
{
    std :: ofstream fout(fileName);

    fout << this->outputFileName << '\n';
    fout << this->description << '\n';
    for ( auto &rec: this->recordList ) {
        fout << rec->giveRecordAsString() << "\n";
    }
    fout.close();
}
} // end namespace oofem
