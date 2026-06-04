/**
 *       Copyright 2020-2023 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * Polyspace Software complexity Guidelines are violations that may impact
 * readability or maintainability of your code.
 *
 * This example will show you how selected Guidelines will be handled by the
 * Polyspace Bug Finder.
 * Each violation is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */

#define _USE_MATH_DEFINES // Necessary for M_Pi on some compilers
#include <math.h>

extern double getAngle(void);
extern double getDeviation(void);

/*============================================================================
 *  TOO MANY CALLS
 *==========================================================================*/
double bug_sc05(void) { /* Violation: Too many calls */
    double x = getAngle();
    double y = getDeviation();

    return fmod(trunc(100 * atan2(sqrt(sin(fabs(x) * cos(x))) / sqrt(fabs(x + y) + 1),
                                  atan(M_PI * sin(x) * cos(x) / (cos(x + y) + cos(x - y) - M_PI)))),
                ceil(atan(asin(x) + acos(x) + cosh(y) + sinh(y) + tanh(x + y))));
}

double num(double x, double y) {
    return trunc(100 * atan2(sqrt(sin(fabs(x) * cos(x))) / sqrt(fabs(x + y) + 1),
                             atan(M_PI * sin(x) * cos(x) / (cos(x + y) + cos(x - y) - M_PI))));
}

double den(double x, double y) {
    return ceil(atan(asin(x) + acos(x) + cosh(y) + sinh(y) + tanh(x + y)));
}

double corrected_sc05(void) { /* Fix: Refactored function */
    double x = getAngle();
    double y = getDeviation();

    return fmod(num(x, y), den(x, y));
}


/*============================================================================
 *  TOO MANY GOTO
 *==========================================================================*/
int bug_sc13(int i) { /* Violation: Too many goto */
    if (i <= 0) {
        goto error;
    }

loop:
    if (i % 2) {
        i = i / 2;
    } else {
        i = 3 * i + 1;
    }

    if (i != 1) {
        goto loop;
    }

    return i;

error:
    return 0;
}

int corrected_sc13(int i) { /* Fix: Corrected control flow */
    if (i <= 0) {
        return 0;
    }

    while (i != 1) {
        if (i % 2) {
            i = i / 2;
        } else {
            i = 3 * i + 1;
        }
    }

    return i;
}


/*============================================================================
 *  TOO MANY FUNCTION PARAMETERS
 *==========================================================================*/
double bug_sc07(/* Violation: Too many function parameters */
                double x1,
                double y1,
                double z1,
                double x2,
                double y2,
                double z2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

typedef struct {
    double x;
    double y;
    double z;
} point;

double corrected_sc07(/* Fix: Use compound types */
                      point p1,
                      point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) +
                (p1.z - p2.z) * (p1.z - p2.z));
}


/*============================================================================
 *  TOO MANY CALL LEVELS
 *==========================================================================*/
/* These functions return 0 if OK */
int executeStepA(void);
int executeStepB(void);
int executeStepC(void);
int executeStepD(void);
int executeStepE(void);
int executeStepF(void);
int executeStepG(void);

int bug_sc14(void) { /* Violation: Too many control flow levels */
    if (executeStepA() == 0) {
        if (executeStepB() == 0) {
            if (executeStepC() == 0) {
                if (executeStepD() == 0) {
                    if (executeStepE() == 0) {
                        if (executeStepF() == 0) {
                            if (executeStepG() == 0) {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }

    return 1;
}

int corrected_sc14(void) { /* Fix: Reworked flow levels */
    if (executeStepA() == 0) {
        return 1;
    }
    if (executeStepB() == 0) {
        return 1;
    }
    if (executeStepC() == 0) {
        return 1;
    }
    if (executeStepD() == 0) {
        return 1;
    }
    if (executeStepE() == 0) {
        return 1;
    }
    if (executeStepF() == 0) {
        return 1;
    }
    if (executeStepG() == 0) {
        return 1;
    }
    return 0;
}
