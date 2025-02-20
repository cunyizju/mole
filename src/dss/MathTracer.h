// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef _MATH_TRACER_H__
#define _MATH_TRACER_H__

#include "DSSAfx.h"

DSS_NAMESPASE_BEGIN

/**
 * @author Richard Vondracek
 */

class MathTracer
{
private:
    char m_string [ 128 ];

public:
    double min_pivot;
    double stabil_pivot;
    int break_flag;
    long act_block;
    long act_row;

    MathTracer();

    virtual void Write(double a);
    virtual void Write(int a);
    virtual void Writeln();
    virtual void Writeln(const char *str);
    virtual void Write(const char *str);

    //virtual void DrawProgress(double e);

    // true - continue factorization
    // false - break factorization
    virtual bool CallUnstableDialog();

    //virtual void PrintUnstablePivot(long pivot);

    char *NowString();
    void CS(void);
    char *MC_();

    clock_t ClockStart(void);
    char *MeasureClock(clock_t &clock);

protected:
    time_t m_temporary_measure_start;
    clock_t m_clock_start;
};


DSS_NAMESPASE_END

#endif //_MATH_TRACER_H__
