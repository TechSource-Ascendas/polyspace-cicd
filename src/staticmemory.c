/**
 *       Copyright 2012-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * Static memory errors are programming mistakes that lead to possible data
 * corruption and erratic problems.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */

#define _POSIX_C_SOURCE 200112L /* For setenv */
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
#include <unistd.h>
#endif /* __GNUC__ */

#include "globaldefs.h"

/*============================================================================
 *  OUT OF ARRAY BOUNDS
 *==========================================================================*/
void bug_outboundarray(void) {
    int i;
    unsigned long fib[SIZE10];

    for (i = 0; i < SIZE10; i++) {
        if (i < 2) {
            fib[i] = 1;
        } else {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
        (void)printf("  calculated %d %lu\n", i, fib[i]);
    }
    (void)printf("Fibonacci raised %lu\n", fib[i]); /* Defect: Out of bounds */
}

void corrected_outboundarray(void) {
    int i;
    unsigned long fib[SIZE10];

    for (i = 0; i < SIZE10; i++) {
        if (i < 2) {
            fib[i] = 1;
        } else {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
    }
    (void)printf("Fibonacci raised %lu\n", fib[i - 1]); /* Fix: Corrected index */
}


/*============================================================================
 *  OUT-OF-BOUNDS MEMORY ACCESS
 *==========================================================================*/
void bug_outboundptr(void) {
    char* ptr;
    int i;

    ptr = (char*)malloc(SIZE10);
    if (ptr == NULL) {
        return;
    }
    for (i = 0; i <= (SIZE10 - 1); i++) { /* After the loop, i is outside the buffer  */
        ptr[i] = 43;
    }

    ptr[i] = 4; /* Defect: Out of bounds access             */

    free(ptr);
}

void corrected_outboundptr(void) {
    char* ptr;
    int i;

    ptr = (char*)malloc(SIZE10);
    if (ptr == NULL) {
        return;
    }
    for (i = 0; i < (SIZE10 - 1); i++) { /* Fix: one iteration less                  */
        ptr[i] = 43;
    }

    ptr[i] = 4;

    free(ptr);
}


/*============================================================================
 *  NULL POINTER DEREFERENCE
 *==========================================================================*/
void useint(int i);

void bug_nullptr(void) {
    int a = 2;
    int* x = &a;
    int** ptr = &x;

    x = NULL;

    useint(**ptr); /* Defect: Null pointer dereference         */
}

void corrected_nullptr(void) {
    int a = 2;
    int* x = &a; /* Fix: Handle a pointer to an actual value */
    int** ptr = &x;

    useint(**ptr);
}


/*============================================================================
 *  LOCAL ADDRESS ESCAPE
 *==========================================================================*/
int* bug_localaddrescape() {
    int i = 2;
    return &i; /* Defect: returning address of local */
}

int corrected_localaddrescape() {
    int i = 2;
    return i; /* Fix: returning value of local */
}


/*============================================================================
 *  OBJECT SIZE MISMATCH IN CAST
 *==========================================================================*/
int* bug_objectsizemismatch_alloc() {
    int s = sizeof(char);
    int* p = (int*)malloc(13 * s); /* Defect: size mismatch */

    return p;
}

int* corrected_objectsizemismatch_alloc() {
    int* p = (int*)malloc(sizeof(int)); /* Fix: Good size */

    return p;
}


/*============================================================================
 *  MISUSE OF MEMORY STANDARD LIBRARY
 *==========================================================================*/
void bug_memstdlib(void) {
    const char* str = "test";
    char buffer[SIZE2];

    memmove(buffer, str, strlen(str)); /* Defect: Memory overflow of buffer  */
    buffer[sizeof(buffer) - 1] = '\0';
    print_str(buffer);
}

void corrected_memstdlib(void) {
    const char* str = "test";
    char buffer[SIZE2];

    memmove(buffer, str, sizeof(buffer) - 1); /* Fix: Use destination size    */
    buffer[sizeof(buffer) - 1] = '\0';
    print_str(buffer);
}


/*============================================================================
 *  MISUSE OF STRING STANDARD LIBRARY
 *==========================================================================*/
double bug_strstdlib(void) {
    char* str = NULL;
    double d;

    d = atof(str); /* Defect: Invalid use of atof      */

    return d;
}

double corrected_strstdlib(void) {
    char* str = "3.14";
    double d;

    d = atof(str); /* Fix: Use proper atof arguments   */

    return d;
}


/*============================================================================
 *  NULL POINTER ARITHMETIC
 *==========================================================================*/
extern int randint(void);
extern void useptr(int* loc);

void bug_nullptrarith(void) {
    int* p = NULL;
    int x = randint();

    if (randint()) {
        p++; /* Defect: NULL pointer arithmetic  */
    } else {
        p = &x;
    }

    useptr(p);
}

void corrected_nullptrarith(void) {
    int* p = NULL;
    int x = randint();

    p = &x; /* Fix: No arithmetic on NULL       */

    useptr(p);
}


/*============================================================================
 *  POINTER CAST
 *==========================================================================*/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types" /* Silence the error on GCC14+ */

char* bug_ptrcast(int* i) {
    read_pint(i);
    return i; /* Defect: Wrong type of cast */
}

int* corrected_ptrcast(int* i) {
    read_pint(i);
    return i; /* Fix: Same type of cast */
}

#pragma GCC diagnostic pop

/*============================================================================
 *  FUNCTION POINTER CAST
 *==========================================================================*/
#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
extern void func_return_type_void(void);
extern char func_return_type_char(void);

extern void read_ptr(ptr_return_type_void*);
void bug_funccast() {
    ptr_return_type_void ptr;

    ptr = func_return_type_char; /* Defect: Wrong return type */
    read_ptr(&ptr);
}

void corrected_funccast() {
    ptr_return_type_void ptr;

    ptr = func_return_type_void; /* Fix: Correct return type */
    read_ptr(&ptr);
}
#endif /* __GNUC__ */


/*============================================================================
 *  PATH BUFFER OVERFLOW
 *==========================================================================*/
#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
const char* path = "my_path";
void bug_pathbufferoverflow() {
    char buf[SIZE128]; /* Defect: buffer may be too small */

    if (realpath(path, buf) != NULL) {
        (void)printf("resolved %s is %s\n", path, buf);
    }
}

void corrected_pathbufferoverflow() {
    char buf[PATH_MAX]; /* Fix: using wider buffer */

    if (realpath(path, buf) != NULL) {
        (void)printf("resolved %s is %s\n", path, buf);
    }
}
#endif /* __GNUC__ */


/*============================================================================
 *  BUFFER OVERFLOW USING STANDARD STRING FUNCTIONS
 *==========================================================================*/
void bug_strlibbufferoverflow(float f) {
    char buf[SIZE10];
    (void)sprintf(buf, "Result=%12.2E", f); /*Defect: String is more than 10 char wide */
}

void corrected_strlibbufferoverflow(float f) {
    char buf[SIZE20];
    (void)sprintf(buf, "Result=%12.2E", f); /*Fix: Buffer is wide enough */
}


/*============================================================================
 *  BUFFER UNDERFLOW USING STANDARD STRING FUNCTIONS
 *==========================================================================*/
const char* str_orig = "Original string";
void bug_strlibbufferunderflow(int pos) {
    char buf[SIZE20];
    if (pos < 0) {
        pos = -1; /* Defect: Wrong negative default value for pos */
    }
    (void)sprintf(&buf[pos], "Result=%4d", pos);
    print_str(buf);
}

void corrected_strlibbufferunderflow(int pos) {
    char buf[SIZE20];
    if (pos < 0) {
        pos = 0; /* Fix: pos is now positive */
    }
    (void)sprintf(&buf[pos], "Result=%4d", pos);
    print_str(buf);
}


/*============================================================================
 *  BUFFER OVERFLOW DUE TO STRING FORMAT ERROR
 *==========================================================================*/
void bug_strformatbufferoverflow() {
    char buf[SIZE10];
    if (sscanf(str_orig, "%10s", buf) >
        0) { /* Defect: Format implies too long string for buffer size */
        print_str(buf);
    }
}

void corrected_strformatbufferoverflow() {
    char buf[SIZE10];
    if (sscanf(str_orig, "%9s", buf) > 0) { /* Fix: Format is coherent with buffer size */
        print_str(buf);
    }
}


/*============================================================================
 * PUTENV AUTO VAR
 *==========================================================================*/
#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
void bug_putenvautovar(int turn_on) {
    char env[SIZE1024];
    int retval = sprintf(env, "TEST=%s", turn_on ? "1" : "0");
    if (retval <= 0) {
        /* Handle error */
        fatal_error();
    }
    retval = putenv(env); /* Defect: putenv does not copy the input string argument,
                           * it directly integrates the automatic duration buffer 'env'
                           * into the process environment.
                           */
    if (retval != 0) {
        /* Handle error */
        fatal_error();
    }
}

void corrected_putenvautovar(int turn_on) {
    int retval = setenv("TEST", turn_on ? "1" : "0", 1); /* Fix: use setenv. Note that setenv may
                                                          * not be re-entrant, and can cause memory
                                                          * leaks.
                                                          */
    if (retval != 0) {
        /* Handle error */
        fatal_error();
    }
}
#endif /* __GNUC__ */


/*============================================================================
 * PTR TO DIFF ARRAY
 *==========================================================================*/
size_t bug_ptrtodiffarray(void) {
    int nums[SIZE20];
    int end;
    int* next_num_ptr = nums;
    size_t free_elements;
    free_elements = &end - next_num_ptr; /* Defect: subtraction between pointers that do not
                                          * point to a same object is undefined behavior. Programmer
                                          * incorrectly assumes that the nums array is adjacent
                                          * to the end variable in memory.
                                          */
    return free_elements;
}

size_t corrected_ptrtodiffarray(void) {
    int nums[SIZE20];
    int* next_num_ptr = nums;
    size_t free_elements;
    free_elements = &(nums[SIZE20 - 1]) - next_num_ptr; /* Fix: the two pointers involved in the
                                                         * subtraction operation point to a same
                                                         * array object.
                                                         */
    return free_elements + 1;
}
