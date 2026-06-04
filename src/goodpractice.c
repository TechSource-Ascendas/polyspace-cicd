/**
 *       Copyright 2012-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * Some cases violating good practices are shown here.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>

#include "globaldefs.h"


#ifdef PST_BUG_FINDER /* Avoid link issue in Code Prover mode */
/* Global used for Declaration Mismatch defect in programming.c */
bigstruct S_for_programming;
#endif /* PST_BUG_FINDER */


/*============================================================================
 *  MORE THAN ONE STATEMENT ON THE SAME LINE
 *==========================================================================*/
int bug_morethanonestatement(int s) {
    if (s > 0) { s--; } else { s++; } return s; /* Defect: all in one complex line */
}

int corrected_morethanonestatement(int s) {
    if (s > 0) { /* Fix: More readable in split lines */
        s--;
    } else {
        s++;
    }

    return s;
}


/*============================================================================
 *  HARD CODED BUFFER SIZE
 *==========================================================================*/
extern void treat(int* p);
void bug_hardcodedbuffersize(int x, int y, int z) {
    int coord[3]; /* Defect: Size is hard-coded */
    coord[0] = x;
    coord[1] = y;
    coord[2] = z;
    treat(coord);
}

void corrected_hardcodedbuffersize(int x, int y, int z) {
    int coord[SIZE3]; /* Fix: Size is defined through an enum */
    coord[0] = x;
    coord[1] = y;
    coord[2] = z;
    treat(coord);
}


/*============================================================================
 *  HARD CODED BOUNDARY LOOP
 *==========================================================================*/
int bug_hardcodedloopboundary() {
    int coord[SIZE3] = {1, 2, 3};
    int res = 0;
    int i;
    for (i = 0; i < 3; ++i) { /* Defect: Loop boundary is hard-coded */
        res += coord[i] * coord[i];
    }

    return res;
}

int corrected_hardcodedloopboundary() {
    int coord[SIZE3] = {1, 2, 3};
    int res = 0;
    int i;
    for (i = 0; i < SIZE3; ++i) { /* Fix: Loop boundary defined by an enum */
        res += coord[i] * coord[i];
    }

    return res;
}


/*============================================================================
 *  UNUSED PARAMETER
 *==========================================================================*/
void bug_unusedparameter(int i, int j) { /* Defect: Parameter j is not used */
    print_int(i);
}

void corrected_unusedparameter(int i, int j) {
    (void)j; /* Fix: Parameter j is ignored */
    print_int(i);
}


/*============================================================================
 *  USE of SETJMP/LONGJMP
 *==========================================================================*/
extern int update(int);
static jmp_buf env;
void bug_setjmplongjmpuse_handler(int signum) {
    /* Signal handler */
    longjmp(env, signum); /* Defect: using longjmp function */
}
void bug_setjmplongjmpuse_main(int i) {
    (void)signal(SIGINT, bug_setjmplongjmpuse_handler);
    if (setjmp(env) == 0) { /* Defect: using setjmp function */
        while (1) {
            /* Main loop of program, iterates until SIGINT signal catch */
            i = update(i);
        }
    } else {
        /* Managing longjmp return */
        i = -update(i);
    }

    print_int(i);
    return;
}

volatile sig_atomic_t eflag = 0;
void corrected_setjmplongjmpuse_handler(int signum) {
    eflag = signum; /* Fix: using global variable */
}
void corrected_setjmplongjmpuse_main(int i) {
    /* Fix: Better design to avoid use of setjmp/longjmp */
    void (*old)(int) = signal(SIGINT, corrected_setjmplongjmpuse_handler);
    if (old == SIG_ERR) {
        fatal_error();
    }
    while (!eflag) { /* Fix: using global variable */
        /* Main loop of program, iterates until eflag is changed */
        i = update(i);
    }

    print_int(i);
    return;
}


/*==========================================================================
 *  BITWISE ARITH MIX
 *==========================================================================*/
unsigned int bug_bitwisearithmix(void) {
    unsigned int x = 50;
    x += (x << 2) + 1; /* Defect: arithmetic and bitwise operations on 'x' */
    return x;
}

unsigned int corrected_bitwisearithmix(void) {
    unsigned int x = 50;
    x = 5 * x + 1; /* Fix: only an arithmetic operation on 'x' is needed */
    return x;
}


/*============================================================================
 *  MISSING FREED PTR RESET
 *==========================================================================*/
void bug_missingfreedptrreset(void) {
    static char* str = NULL;

    if (str == NULL) {
        str = (char*)malloc(SIZE20);
    }

    if (str != NULL) {
        free(str); /* Defect: str is invalid */
    }
}

static void sanitize_free(void** p) {
    if ((p != NULL) && (*p != NULL)) {
        free(*p);
        *p = NULL;
    }
}

#define free(X) sanitize_free((void**)&X)

void corrected_missingfreedptrreset(void) {
    static char* str = NULL;

    if (str == NULL) {
        str = (char*)malloc(SIZE20);
    }

    if (str != NULL) {
        free(str); /* Fix: str set to NULL after free() */
    }
}


/*============================================================================
 *  MISSING SWITCH BREAK
 *==========================================================================*/
extern void demo_do_something_for_WE_W(void);
extern void demo_do_something_for_WE_X(void);
extern void demo_report_error(void);

void bug_missingswitchbreak(enum WidgetEnum wt) {
    /*
     * In this non-compliant code example, the case where widget_type is WE_W
     * lacks a break statement. Consequently, statements that should be
     * executed only when widget_type is WE_X are executed even when
     * widget_type is WE_W.
     */
    switch (wt) {
    case WE_W: /* Defect: no break in non-empty switch case */
        demo_do_something_for_WE_W();
    case WE_X: /* Defect: no break in non-empty switch case */
        demo_do_something_for_WE_X();
    default:
        demo_report_error(); /* break omitted for the last switch case */
    }
}

void corrected_missingswitchbreak(enum WidgetEnum wt) {
    switch (wt) {
    case WE_W:
        demo_do_something_for_WE_W();
        /* fall through */ /* Fix: A comment documents the fall through */
    case WE_X:
        demo_do_something_for_WE_X();
        break; /* Fix: add break to fix the bug */
    default:
        demo_report_error(); /* break omitted for the last switch case */
    }
}


/*============================================================================
 *  HARD CODED MEM SIZE
 *==========================================================================*/
extern void fill_ints(int** matrix, size_t nb, size_t s);

void bug_hardcodedmemsize(void) {
    /* Assuming 32-bit pointer, 32-bit integer */
    size_t i, s;

    s = 4;
    int** matrix = (int**)calloc(SIZE20, s); /* Defect: hard coded value 4 */
    if (matrix == NULL) {
        return; /* Indicate calloc() failure */
    }
    fill_ints(matrix, SIZE20, s);
    free(matrix);
}

void corrected_hardcodedmemsize(void) {
    /* Assuming 32-bit pointer, 32-bit integer */
    size_t i, s;

    s = sizeof(int*);
    int** matrix = (int**)calloc(SIZE20, s); /* Fix: Use sizeof(int*) */
    if (matrix == NULL) {
        return; /* Indicate calloc() failure */
    }
    fill_ints(matrix, SIZE20, s);
    free(matrix);
}
