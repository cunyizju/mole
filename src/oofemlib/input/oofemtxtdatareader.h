// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef oofemtxtdatareader_h
#define oofemtxtdatareader_h

#include "input/datareader.h"
#include "input/oofemtxtinputrecord.h"

#include <fstream>

namespace oofem {
/**
 * Class representing the implementation of plain text date reader.
 * It reads a sequence of input records from data file
 * and creates the corresponding input records.
 * There is no check for record type requested, it is assumed that records are
 * written in correct order, which determined by the coded sequence of
 * component initialization and described in input manual.
 */
class OOFEM_EXPORT OOFEMTXTDataReader : public DataReader
{
protected:
    std :: string dataSourceName;
    std :: list< OOFEMTXTInputRecord > recordList;

    /// Keeps track of the current position in the list
    std :: list< OOFEMTXTInputRecord > :: iterator it;

public:
    /// Constructor.
    OOFEMTXTDataReader(std :: string inputfilename);
    OOFEMTXTDataReader(const OOFEMTXTDataReader & x);
    virtual ~OOFEMTXTDataReader();

    InputRecord &giveInputRecord(InputRecordType, int recordId) override;
    bool peakNext(const std :: string &keyword) override;
    void finish() override;
    std :: string giveReferenceName() const override { return dataSourceName; }

protected:
    /**
     * Reads one line from inputStream
     * Parts within quotations have case preserved.
     */
    bool giveLineFromInput(std :: ifstream &stream, int &lineNum, std :: string &line);
    /// Reads one line from stream.
    bool giveRawLineFromInput(std :: ifstream &stream, int &lineNum, std :: string &line);
};
} // end namespace oofem
#endif // oofemtxtdatareader_h
