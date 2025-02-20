// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "utility/dictionary.h"
#include "input/logger.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"
#include "utility/contextmode.h"

#include <cstdlib>
#include <ostream>

namespace oofem {
Dictionary :: ~Dictionary()
// Destructor.
{
    this->clear();
}

void
Dictionary :: clear()
{
    Pair *Next;

    while ( first ) {
        Next = first->giveNext();
        delete first;
        first = Next;
    }

    first = NULL;
    last = NULL;
}

int
Dictionary :: giveSize()
{
    int size = 0;
    Pair *next = first;
    while ( next ) {
        size++;
        next = next->giveNext();
    }

    return size;
}

Pair *Dictionary :: add(int k, double v)
// Adds the pair (k,v) to the receiver. Returns this new pair.
{

#  ifdef DEBUG
    if ( this->includes(k) ) {
        OOFEM_ERROR("key (%d) already exists", k);
    }

#  endif

    Pair *newPair = new Pair(k, v);
    if ( last ) {
        last->append(newPair);
    } else {                              // empty dictionary
        first = newPair;
    }

    last = newPair;

    return newPair;
}


double &Dictionary :: at(int aKey)
// Returns the value of the pair which key is aKey. If such pair does
// not exist, creates it and assign value 0.
{
    Pair *next = first;
    while ( next ) {
        if ( next->giveKey() == aKey ) {
            return next->giveValue();
        }

        next = next->giveNext();
    }

    Pair *newPair = this->add(aKey, 0);         // pair does not exist yet
    return newPair->giveValue();
}


double Dictionary :: at(int aKey) const
{
    Pair *next = first;
    while ( next ) {
        if ( next->giveKey() == aKey ) {
            return next->giveValue();
        }

        next = next->giveNext();
    }
    OOFEM_ERROR("Requested key missing from dictionary");
    return 0.;
}


bool Dictionary :: includes(int aKey) const
// Returns True if the receiver contains a pair which key is aKey, else
// returns False.
{
    Pair *next = first;
    while ( next ) {
        if ( next->giveKey() == aKey ) {
            return true;
        }

        next = next->giveNext();
    }

    return false;
}


void Dictionary :: printYourself()
// Prints the receiver on screen.
{
    Pair *next;

    printf("Dictionary : \n");

    next = first;
    while ( next ) {
        next->printYourself();
        next = next->giveNext();
    }
}


void
Dictionary :: formatAsString(std :: string &str)
{
    Pair *next;
    char buffer [ 64 ];

    next = first;
    while ( next ) {
        sprintf( buffer, " %c %e", next->giveKey(), next->giveValue() );
        str += buffer;
        next = next->giveNext();
    }
}


void Dictionary :: saveContext(DataStream &stream)
{
    int nitems = 0;
    int key;
    double value;
    Pair *next;

    next = first;
    while ( next ) {
        nitems++;
        next = next->giveNext();
    }

    // write size
    if ( !stream.write(nitems) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    // write raw data
    next = first;
    while ( next ) {
        key = next->giveKey();
        value = next->giveValue();
        if ( !stream.write(key) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        if ( !stream.write(value) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        next = next->giveNext();
    }
}


void Dictionary :: restoreContext(DataStream &stream)
{
    int size;
    int key;
    double value;

    // delete currently occupied space
    this->clear();

    // read size
    if ( !stream.read(size) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    // read particular pairs
    for ( int i = 1; i <= size; i++ ) {
        if ( !stream.read(key) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        if ( !stream.read(value) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        this->at(key) = value;
    }
}


std :: ostream &operator << ( std :: ostream & out, const Dictionary & r )
{
    int count = 0;
    Pair *next = r.first;
    while ( next ) {
        count++;
        next = next->giveNext();
    }

    out << count;
    next = r.first;
    while ( next ) {
        out << " " << next->giveKey() << " " << next->giveValue();
        next = next->giveNext();
    }
    return out;
}
} // end namespace oofem
