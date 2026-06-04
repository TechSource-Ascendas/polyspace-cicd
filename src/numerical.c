/**
 *       Copyright 2012-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * Numeric defects are programming mistakes that may harm correct execution
 * of your code.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */


#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "globaldefs.h"

/*============================================================================
 *  INTEGRAL DIVISION BY ZERO
 *==========================================================================*/
int bug_intzerodiv(int p) {
    int i;
    int j = 1;

    i = 1024 / (j - p); /* Defect: Division by zero                 */
    return i;
}

int corrected_intzerodiv(int p) {
    int i;
    int j = 1;

    if (j != p) { /* Fix: Guard against division by zero      */
        i = 1024 / (j - p);
    } else {
        i = 0;
    }
    return i;
}

void call_intzerodiv(void) {
    volatile int random = 0;
    if (random == 0) {
        bug_intzerodiv(1);
    }
    if (random == 0) {
        corrected_intzerodiv(1);
    }
}


/*============================================================================
 *  FLOAT DIVISION BY ZERO
 *==========================================================================*/
float bug_floatzerodiv(float p) {
    float i;
    float j = 1.0;

    i = 1024.0 / (j - p); /* Defect: Division by zero             */
    return i;
}

float corrected_floatzerodiv(float p) {
    float i;
    float j = 1.0;
    float tmp;

    tmp = j - p;
    if (tmp < 0.0) {
        i = -1.0;
    } else if (tmp > FLT_EPSILON) { /* Fix: Guard against division by zero  */
        i = 1024.0 / tmp;
    } else {
        i = 0.0;
    }
    return i;
}

void call_floatzerodiv(void) {
    volatile int random = 0;
    if (random == 0) {
        bug_floatzerodiv(1.0);
    }
    if (random == 0) {
        corrected_floatzerodiv(1.0);
    }
}


/*============================================================================
 *  INTEGER CONVERSION OVERFLOW
 *==========================================================================*/
short bug_intconvovfl(int th) {
    long value;

    if (th == 42) {
        value = 0x00FE6F00L;
    } else {
        value = 0x00BE6B00L;
    }
    return value; /* Defect: Integer conversion overflow  */
}

short corrected_intconvovfl(int th) {
    long value;

    if (th == 42) {
        value = 0x00FE6F00L;
    } else {
        value = 0x00BE6B00L;
    }
    return (short)(value & 0xFFFF); /* Fix: Explicitly cast the value       */
}


/*============================================================================
 *  UNSIGNED INTEGER CONVERSION OVERFLOW
 *==========================================================================*/
unsigned short bug_uintconvovfl(int th) {
    unsigned long value;

    if (th == 42) {
        value = 0x00FE6F00L;
    } else {
        value = 0x00BE6B00L;
    }
    return value; /* Defect: Integer conversion overflow  */
}

unsigned short corrected_uintconvovfl(int th) {
    unsigned long value;

    if (th == 42) {
        value = 0x00FE6F00L;
    } else {
        value = 0x00BE6B00L;
    }
    return (unsigned short)(value & 0xFFFF); /* Fix: Explicitly cast the value       */
}


/*============================================================================
 *  SIGN CHANGE INTEGER CONVERSION OVERFLOW
 *==========================================================================*/
int bug_signchange(unsigned int rd_uint) {
    unsigned int ui1 = 0;
    int i1 = 0;

    if (rd_uint) {
        ui1 = UINT_MAX;
        i1 = ui1; /* Defect: Sign change overflow */
    }
    return i1;
}

int corrected_signchange(unsigned int rd_uint) {
    int i1 = 0;

    if (rd_uint) {
        i1 = -1; /* Fix: 2-complement arithmetic */
    }
    return i1;
}


/*============================================================================
 *  FLOAT CONVERSION OVERFLOW
 *==========================================================================*/
void bug_floatconvovfl(int rd_int) {
    unsigned int exp1 = 90;
    float f1 = 0.0F;

    if (rd_int) {
        f1 = exp(exp1); /* Defect : Float conversion overflow */
    }

    do_something_float(f1);
}

void corrected_floatconvovfl(int rd_int) {
    unsigned int exp1 = 90;
    float f1 = 0.0F;

    if (rd_int) {
        f1 = (float)fmin(FLT_MAX, exp(exp1)); /* Fix : Float conversion with control of overflow */
    }

    do_something_float(f1);
}


/*============================================================================
 *  INTEGRAL OVERFLOW
 *==========================================================================*/
int bug_intovfl(int val) {
    int first;
    int second;
    int result;

    if (val < 10) {
        first = INT_MAX - 2;
        second = 50;
    } else {
        first = 80;
        second = INT_MAX - 30;
    }

    result = first + second; /* Defect: Integral overflow            */
    return result;
}

long corrected_intovfl(int val) {
    int first;
    int second;
    long result;

    if (val < 10) {
        first = INT_MAX - 2;
        second = 50;
    } else {
        first = 80;
        second = INT_MAX - 30;
    }

    result = (long)first + (long)second; /* Fix: No integral overflow using wider type */
    return result;
}


/*============================================================================
 *  UNSIGNED OVERFLOW
 *==========================================================================*/
unsigned int bug_uintovfl(void) {
    unsigned int bareval = UINT_MAX;
    unsigned int result = 0;

    result = bareval << 2; /* Defect: Unsigned overflow        */

    return result;
}

unsigned int corrected_uintovfl(void) {
    unsigned int bareval = UINT_MAX;
    unsigned int result = 0;

    result = (bareval & 0x3FFF) << 2; /* Fix: Mask out first bits         */

    return result;
}


/*============================================================================
 *  FLOAT OVERFLOW
 *==========================================================================*/
float bug_floatovfl(void) {
    float f = 1.0;

    f = f / (1.0e-39f); /* Defect: Operation raises infinity    */

    return f;
}

float corrected_floatovfl(void) {
    float f = 1.0;

    f = f / (1.0e-18); /* Fix: Use a negligible but still in float range value          */

    return f;
}


/*============================================================================
 *  ABSORPTION OF FLOAT OPERAND
 *==========================================================================*/
double bug_floatabsorption(double large, double small) {
    double d = 0.;
    if ((large >= 1.0e20) && (-1.0 <= small) && (small <= 1.))

    {
        d = large + small; /* Defect: Operation is redundant      */
    }

    return d;
}

double corrected_floatabsorption(double large, double small) {
    double d = 0.;
    if ((large >= 1.0e10) && (-1.0 <= small) && (small <= 1.))

    {
        d = large + small; /* Fix: Do not sum negligible parts    */
    }

    return d;
}


/*============================================================================
 *  INVALID USE OF INTEGER STANDARD LIBRARY
 *==========================================================================*/
div_t bug_intstdlib(int num, int denom) {
    div_t test;

    if (denom == 0) {
        test = div(num, denom); /* Defect: Generates a division by zero */
    } else if ((num == INT_MIN) && (denom == -1)) {
        test = div(num, denom); /* Defect: Generates an overflow on first field of div_t struct */
    } else {
        test = div(num, denom);
    }

    return test;
}

div_t corrected_intstdlib(int num, int denom) {
    div_t test = {0, 0};

    if ((denom != 0) && !(num == INT_MIN && denom == -1)) {
        test = div(num, denom); /*Fix: Exclude problematic values */
    }

    return test;
}


/*============================================================================
 *  INVALID USE OF FLOAT STANDARD LIBRARY
 *==========================================================================*/
double bug_floatstdlib(int checkval) {
    double res;
    double entry;
    if (checkval > 2) {
        entry = 10.0;
    } else {
        entry = -1.0;
    }

    res = asin(entry); /* Defect: Argument should be within [-1;1]     */
    return res;
}

double corrected_floatstdlib(int checkval) {
    double res;
    double entry;
    if (checkval > 2) {
        entry = 1.0;
    } else {
        entry = -1.0;
    }

    res = asin(entry); /* Fix: Argument is within [-1;1]               */
    return res;
}


/*============================================================================
 *  SHIFT NEGATIVE
 *==========================================================================*/
int bug_shiftneg(int val) {
    int baseval;
    int res;

    if (val > 5) {
        baseval = -3;
    } else {
        baseval = -15;
    }

    res = baseval << 3; /* Defect: Shift of a negative value            */
    return res;
}

int corrected_shiftneg(int val) {
    int baseval;
    int res;

    if (val > 5) {
        baseval = -3;
    } else {
        baseval = -15;
    }

    res = baseval / 8; /* Fix: Use a division instead of a shift */
    return res;
}


/*============================================================================
 *  CAPACITY OVERFLOW IN SHIFT
 *==========================================================================*/
unsigned short bug_shiftovfl(void) {
    const unsigned short mask2bits = 0x0003U;
    const int num = 42;
    unsigned short res;

    res = mask2bits << num; /* Defect: Capacity overflow */

    return res;
}

unsigned short corrected_shiftovfl(void) {
    const unsigned short mask2bits = 0x0003U;
    const int num = 5; /* Fix: Use a shift value that fits */
    unsigned short res;

    res = mask2bits << num;

    return res;
}


/*============================================================================
 *  BAD PLAIN CHAR USE
 *==========================================================================*/
void bug_badplaincharuse(void) {
    /* In this non-compliant code example, the char-type variable c may be
       signed or unsigned. Assuming 8-bit, two's complement character types,
       this code may print out either i/c = 5 (unsigned) or i/c = -17 (signed).
       It is much more difficult to reason about the correctness of a program
       without knowing if these integers are signed or unsigned.
    */
    char c = 200;
    int i = 1000;
    (void)printf("i/c = %d\n",
                 i / c); /* Defect: negative plain char type used as arithmetic value */
}

void corrected_badplaincharuse(void) {
    signed char c = -56; /* signed char, negative value assumed */
    int i = 1000;
    (void)printf("i/c = %d\n", i / c);
}


/*============================================================================
 *  BITWISE NEG
 *==========================================================================*/
static void demo_sprintf(const char* format, ...) {
    int rc;
    va_list ap;
    char buf[sizeof("256")];

    va_start(ap, format);
    rc = vsprintf(buf, format, ap);
    if (rc == -1 || rc >= sizeof(buf)) {
        /* Handle error */
    }
    va_end(ap);
}

void bug_bitwiseneg(void) {
    int stringify = 0x80000000;
    demo_sprintf("%u", stringify >> 24); /* Defect: programmer expects 0x80, but stringify >> 24
                                          * results in 0xffffff80 because of sign extension, will
                                          * crash demo_sprintf */
}

void corrected_bitwiseneg(void) {
    unsigned int stringify = 0x80000000;
    demo_sprintf("%u", stringify >> 24);
}


/*============================================================================
 *  PRECISION LOSS OF CONVERSION FROM INTEGER TO FLOAT
 *==========================================================================*/
void bug_inttofloatprecisionloss(void) {
    long int big = 1234567890;
    float approx = big; /* Defect: Integer value 1234567890 is not representable in 'float'. */
    (void)printf("%ld\n", (big - (long int)approx));
}

void corrected_inttofloatprecisionloss(void) {
    long int big = 1234567890;
    double approx = big; /* No Defect: Integer value 1234567890 is representable in 'double'. */
    (void)printf("%ld\n", (big - (long int)approx));
}
