// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "export/datastream.h"
#include "error/error.h"
#include <vector>

namespace oofem
{
int DataStream :: read(std :: string &data)
{
    int n;
    std :: vector< char >str;
    if ( !this->read(& n, 1) ) {
        data = "";
        return 0;
    }
    str.resize(n);
    if ( !this->read(str.data(), n) ) {
        data = "";
        return 0;
    }
    data = std::string(str.data(), str.size());
    return 1;
}

int DataStream :: write(const std :: string &data)
{
    int n = ( int ) data.size();
    this->write(& n, 1);
    return this->write(data.data(), n);
}

FileDataStream :: FileDataStream(std::string filename, bool write): 
    stream(nullptr),
    filename(std::move(filename))
{
    //stream.open(filename, (write ? std::ios::out : std::ios:in) | std::ios::binary );
    this->stream = fopen(this->filename.c_str(), write ? "wb" : "rb" );
    if ( !this->stream ) {
        throw CantOpen(this->filename);
    }
}

FileDataStream :: ~FileDataStream()
{
    fclose(this->stream);
}

int FileDataStream :: read(int *data, int count)
{
    return ( (int)fread(data, sizeof( int ), count, stream) == count );
    //this->stream.read(reinterpret_cast< char* >(data), sizeof(int)*count);
    //return this->stream.good();
}

int FileDataStream :: read(unsigned long *data, int count)
{
    return ( (int)fread(data, sizeof( unsigned long ), count, stream) == count );
}

int FileDataStream :: read(long *data, int count)
{
    return ( (int)fread(data, sizeof( long ), count, stream) == count );
}

int FileDataStream :: read(double *data, int count)
{
    return ( (int)fread(data, sizeof( double ), count, stream) == count );
}

int FileDataStream :: read(char *data, int count)
{
    return ( (int)fread(data, sizeof( char ), count, stream) == count );
}

int FileDataStream :: read(bool &data)
{
    return ( (int)fread(& data, sizeof( bool ), 1, stream) == 1 );
}

int FileDataStream :: write(const int *data, int count)
{
    return ( (int)fwrite(data, sizeof( int ), count, stream) == count );
}

int FileDataStream :: write(const unsigned long *data, int count)
{
    return ( (int)fwrite(data, sizeof( unsigned long ), count, stream) == count );
}

int FileDataStream :: write(const long *data, int count)
{
    return ( (int)fwrite(data, sizeof( long ), count, stream) == count );
}

int FileDataStream :: write(const double *data, int count)
{
    return ( (int)fwrite(data, sizeof( double ), count, stream) == count );
}

int FileDataStream :: write(const char *data, int count)
{
    return ( (int)fwrite(data, sizeof( char ), count, stream) == count );
}

int FileDataStream :: write(bool data)
{
    return ( (int)fwrite(& data, sizeof( bool ), 1, stream) == 1 );
}

int FileDataStream :: givePackSizeOfInt(int count)
{
    return sizeof(int)*count;
}

int FileDataStream :: givePackSizeOfDouble(int count)
{
    return sizeof(double)*count;
}

int FileDataStream :: givePackSizeOfChar(int count)
{
    return sizeof(char)*count;
}

int FileDataStream :: givePackSizeOfBool(int count)
{
    return sizeof(bool)*count;
}

int FileDataStream :: givePackSizeOfLong(int count)
{
    return sizeof(int)*count;
}

}
