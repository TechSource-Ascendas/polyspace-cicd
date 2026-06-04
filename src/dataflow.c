/**
 *       Copyright 2012-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * Data flow errors are programming mistakes that may reveal an incorrect
 * translation of an algorithm to code.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globaldefs.h"


/*============================================================================
 *  USELESS WRITE
 *==========================================================================*/
void bug_uselesswrite(void) {
    extern int getsensor(void);
    int i;

    i = 4 * getsensor(); /* Defect: Useless write                    */
}

void corrected_uselesswrite(void) {
    extern int getsensor(void);

    getsensor(); /* Fix: No useless affectation anymore      */
}


/*============================================================================
 *  USELESS WRITE
 *==========================================================================*/
int bug_uselesswrite_doublewrite(void) {
    extern int getsensor(void);
    int command;
    int value;

    command = getsensor();
    if (command < 0) {
        value = 0;
    } else {
        value = 2 * getsensor();
        value = 1 * getsensor(); /* Defect: Double write                 */
    }
    return value;
}

int corrected_uselesswrite_doublewrite(void) {
    extern int getsensor(void);
    int command;
    int value;

    command = getsensor();
    if (command < 0) {
        value = 0;
    } else {
        getsensor(); /* Fix: Only one affectation            */
        value = 1 * getsensor();
    }
    return value;
}


/*============================================================================
 *  NOT INITIALIZED VARIABLE
 *==========================================================================*/
int bug_noninitvar(void) {
    extern int getsensor(void);
    int command;
    int value;

    command = getsensor();
    if (command == 2) {
        value = getsensor();
    }

    return value; /* Defect: Variable may not be initialized  */
}

int corrected_noninitvar(void) {
    extern int getsensor(void);
    int command;
    int value = 0; /* Fix: Add a default initialization        */

    command = getsensor();
    if (command == 2) {
        value = getsensor();
    }

    return value;
}


/*============================================================================
 *  READING UNINITIALIZED POINTED VALUE
 *==========================================================================*/
int bug_noninitvar1(void) {
    int j;
    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) {
        return 0;
    }

    j = *pi + 2; /* Defect: Using uninitialized memory       */
    free(pi);

    return j;
}

int corrected_noninitvar1(void) {
    int j;
    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) {
        return 0;
    }
    *pi = 0; /* Fix: Initialize the allocated memory     */

    j = *pi + 2;
    free(pi);

    return j;
}


/*============================================================================
 *  NOT INITIALIZED POINTER
 *==========================================================================*/
int* bug_noninitptr(int* prev) {
    int j = 42;
    int* pi;

    if (prev == NULL) {
        pi = (int*)malloc(sizeof(int));
        if (pi == NULL) {
            return NULL;
        }
    }

    *pi = j; /* Defect: Writing to uninitialized pointer */

    return pi;
}

int* corrected_noninitptr(int* prev) {
    int j = 42;
    int* pi;

    if (prev == NULL) {
        pi = (int*)malloc(sizeof(int));
        if (pi == NULL) {
            return NULL;
        }
    } else {
        pi = prev; /* Fix: Pointer always initialized          */
    }

    *pi = j;

    return pi;
}

void call_noninitptr() {
    /* Prevent tainted pointer defect */
    int* pi = (int*)malloc(sizeof(int));
    corrected_noninitptr(pi);
    free(pi);
}


/*============================================================================
 *  USELESS IF
 *==========================================================================*/
void bug_uselessif() {
    int i = 1;
    if (i > 0) { /* Defect: test is always true */
        print_int(i);
    }
}

void corrected_uselessif(int i) {
    if (i > 0) { /* Fix: test is not always true */
        print_int(i);
    }
}


/*============================================================================
 *  VARIABLE SHADOWING
 *==========================================================================*/
typedef enum _color { red, green, blue } color;
int bug_varshadowing(void) {
    int red; /* Defect: Variable shadows the enum above */
    red = readint();
    return red;
}

int corrected_varshadowing(void) {
    int myred; /* Fix: Use a distinct name */
    myred = readint();
    return myred;
}


/*============================================================================
 *  PARTIALLY ACCESSED ARRAY
 *==========================================================================*/
void bug_partiallyaccessedarray(void) {
    int i, mytab[SIZE5];

    for (i = 0; i < SIZE5; i++) {
        mytab[i] = i;
    }

    print_int(mytab[0]);
    print_int(mytab[1]);
    print_int(mytab[2]);
    print_int(mytab[2]); /* Defect: Same index as before             */
    print_int(mytab[4]);
}

void corrected_partiallyaccessedarray(void) {
    int i, mytab[SIZE5];

    for (i = 0; i < SIZE5; i++) {
        mytab[i] = i;
    }

    print_int(mytab[0]);
    print_int(mytab[1]);
    print_int(mytab[2]);
    print_int(mytab[3]); /* Fix: Index correctly incremented         */
    print_int(mytab[4]);
}

int bug_partiallyaccessedarray_fib(void) {
    int i;
    unsigned long fib[SIZE10];
    for (i = 0; i < SIZE10; i++) {
        if (i < 2) {
            fib[i] = 1;
        } else {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
    }
    return fib[i - 2]; /* Defect: fib[9] never read, as returning wrong index */
}

int corrected_partiallyaccessedarray_fib(void) {
    int i;
    unsigned long fib[SIZE10];
    for (i = 0; i < SIZE10; i++) {
        if (i < 2) {
            fib[i] = 1;
        } else {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
    }
    return fib[i - 1]; /* Fix: fib[9] is returned */
}


/*============================================================================
 *  DEACTIVATED CODE
 *==========================================================================*/
int do_anything(void);

int bug_deactivatedcode(void) {
    int retval;
    retval = do_anything();

    if (0 /*|| retval < 0*/) { /* Defect: Deactivated (debug) code     */
        retval = do_anything();
    }

    return retval;
}

int corrected_deactivatedcode(void) {
    int retval;
    retval = do_anything();

    if (retval < 0) { /* Fix: Remove the debug artifact       */
        retval = do_anything();
    }

    return retval;
}


/*============================================================================
 *  DEAD CODE
 *==========================================================================*/
suit nextcard(void);
void do_something_suit(suit s);

void bug_deadcode(void) {
    suit card = nextcard();
    if ((card < SPADES) || (card > CLUBS)) {
        card = UNKNOWN_SUIT;
    }

    if (card > 7) {
        do_something_suit(card); /* Defect: Dead code                    */
    }
}

void corrected_deadcode(void) {
    suit card = nextcard();
    if ((card < SPADES) || (card > CLUBS)) {
        card = UNKNOWN_SUIT;
    }

    if (card > HEARTS) { /* Fix: Use an appropriate value        */
        do_something_suit(card);
    }
}


/*============================================================================
 *  MISSING RETURN
 *==========================================================================*/
int bug_missingreturn(int c) {
    if (c > 0) {
        return 1;
    }
} /* Defect: no return in else case */

int corrected_missingreturn(int c) {
    if (c > 0) {
        return 1;
    }

    return 0;
} /* Fix: added return in else case */


/*============================================================================
 *  NON INITIALIZED POINTER CONVERSION
 *==========================================================================*/
void bug_noninitptrconv(int i) {
    int x;
    if (i > 0) {
        x = 1;
    }

    print_pint(&x); /* Defect: x not initialized before taking address when i is not positive */
}

void corrected_noninitptrconv(int i) {
    int x = 0;
    if (i > 0) {
        x = 1;
    }

    print_pint(&x); /* Fix: x always initialized */
}


/*============================================================================
 *  UNREACHABLE CODE
 *==========================================================================*/
void bug_unreachable(int i) {
    if (i == 1) {
        return;
    } else {
        return;
    }

    print_int(i); /* Defect: unreachable code */
}

void corrected_unreachable(int i) {
    if (i == 1) {
        return;
    }

    print_int(i); /* Fix: reachable code */
}


/*============================================================================
 *  STATIC UNCALLED FUNCTION
 *==========================================================================*/
static void bug_uncalledfunc(void) { /* Defect: Uncalled static function */
    return;
}

static void corrected_uncalledfunc(void) { /* Fix: Static function called */
    return;
}

void call_uncalledfunc() {
    /* Here we call the static function */
    corrected_uncalledfunc();
}
