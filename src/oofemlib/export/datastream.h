// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef datastream_h
#define datastream_h

#include "oofemcfg.h"

#include <sstream>
#include <cstdio>
#include <exception>
#include <stdexcept>

namespace oofem {
/**
 * The purpose of DataStream abstract class is to allow to store/restore context to different streams,
 * including file, communication buffers, etc., using the same routine.
 * This will facilitate many algorithms relying on saving/moving state of components
 * (such as load balancing), without writing new (and very similar) routines.
 * This  will lead to a  better consistency of code.
 * @todo Are the "long" and "unsigned long" functions really necessary?
 */
class OOFEM_EXPORT DataStream
{
public:
    /// Destructor
    virtual ~DataStream() { }
    /**
     * @name Data Stream reading methods.
     * These methods read "count" values from data stream into
     * array passed as the first argument.
     * All functions return nonzero if successful.
     */
    //@{
    /// Reads count integer values into array pointed by data.
    virtual int read(int *data, int count) = 0;
    int read(int &data) { return this->read(&data, 1); }
    /// Reads count unsigned long values into array pointed by data.
    virtual int read(unsigned long *data, int count) = 0;
    int read(unsigned long &data) { return this->read(&data, 1); }
    /// Reads count long values into array pointed by data.
    virtual int read(long *data, int count) = 0;
    int read(long &data) { return this->read(&data, 1); }
    /// Reads count double values into array pointed by data.
    virtual int read(double *data, int count) = 0;
    int read(double &data) { return this->read(&data, 1); }
    /// Reads count char values into array pointed by data.
    virtual int read(char *data, int count) = 0;
    int read(char &data) { return this->read(&data, 1); }
    /// Reads a bool value from data.
    virtual int read(bool &data) = 0;
    /// Reads a string (stored as an int for the length followed by char*).
    int read(std :: string &data);
    //@}

    /**
     * @name Data Stream writing methods.
     * These methods write "count" values of data into stream.
     * All functions return nonzero if successful.
     */
    //@{
    /// Writes count integer values from array pointed by data.
    virtual int write(const int *data, int count) = 0;
    int write(int data) { return this->write(&data, 1); }
    /// Writes count unsigned long values from array pointed by data.
    virtual int write(const unsigned long *data, int count) = 0;
    int write(unsigned long data) { return this->write(&data, 1); }
    /// Writes count long values from array pointed by data.
    virtual int write(const long *data, int count) = 0;
    int write(long data) { return this->write(&data, 1); }
    /// Writes count double values from array pointed by data.
    virtual int write(const double *data, int count) = 0;
    int write(double data) { return this->write(&data, 1); }
    /// Writes count char values from array pointed by data.
    virtual int write(const char *data, int count) = 0;
    int write(char data) { return this->write(&data, 1); }
    /// Writes a bool value.
    virtual int write(bool data) = 0;
    /// Reads a string (stored as an int for the length followed by char*).
    int write(const std :: string &data);
    /// Writes a string (wrapper needed, otherwise write(bool) is called )
    int write(const char *data) { return this->write(std :: string(data)); }
    //@}

    /**
     * @name Sizing functions.
     * These methods compute the stored size (in bytes) of an array containing "count" elements.
     */
    //@{
    virtual int givePackSizeOfInt(int count) = 0;
    virtual int givePackSizeOfDouble(int count) = 0;
    virtual int givePackSizeOfChar(int count) = 0;
    virtual int givePackSizeOfBool(int count) = 0;
    virtual int givePackSizeOfLong(int count) = 0;
    //@}
};


/**
 * Implementation of FileDataStream representing DataStream interface to file i/o.
 * This class creates a DataStream shell around c file i/o routines. This class will
 * not provide any methods for opening/closing file. This is the responsibility of user.
 * @see DataStream class.
 */
class OOFEM_EXPORT FileDataStream : public DataStream
{
public:
    class CantOpen : public std::runtime_error
    {
    public:
        std::string filename;
        CantOpen(std::string file): std::runtime_error("can't open file"), filename(std::move(file)) {}
    };

private:
    /// FILE pointer of associated stream
    FILE *stream;
    /// Filename
    std :: string filename;
public:
    /// Constructor, takes associated stream pointer as parameter
    FileDataStream(std :: string filename, bool write);

    /// Destructor (will not close stream!)
    virtual ~FileDataStream();

    int read(int *data, int count) override;
    int read(unsigned long *data, int count) override;
    int read(long *data, int count) override;
    int read(double *data, int count) override;
    int read(char *data, int count) override;
    int read(bool &data) override;

    int write(const int *data, int count) override;
    int write(const unsigned long *data, int count) override;
    int write(const long *data, int count) override;
    int write(const double *data, int count) override;
    int write(const char *data, int count) override;
    int write(bool data) override;

    int givePackSizeOfInt(int count) override;
    int givePackSizeOfDouble(int count) override;
    int givePackSizeOfChar(int count) override;
    int givePackSizeOfBool(int count) override;
    int givePackSizeOfLong(int count) override;
};

} // end namespace oofem
#endif // datastream_h
