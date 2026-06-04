/**
 *       Copyright 2012-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * This example shows defects caused by programming errors.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */

/* The INCLUDE_C99_EXAMPLES macro will be defined if we use C99 or higher
 */
#if __STDC_VERSION__ >= 199901L
#define INCLUDE_C99_EXAMPLES
#endif

#define _POSIX_C_SOURCE 200809L /* For vscanf, strnlen */

#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif /* __GNUC__ */

#include "globaldefs.h"

volatile int some_condition = 1;


/*============================================================================
 *  ASSERT
 *==========================================================================*/
int bug_assert(void) {
    int x = 0;
    assert(x == 12); /* Defect: Assertion failure                */

    return x;
}

int corrected_assert(void) {
    int x = 12; /* Fix: Set x to the appropriate value      */
    assert(x == 12);

    return x;
}


/*============================================================================
 *  BAD EQUAL USE
 *==========================================================================*/
void bug_badequaluse(int a, int b) {
    if (a = b) { /* Defect: Possible incorrect assignment    */
        (void)printf("Equality\n");
    }
}

void corrected_badequaluse(int a, int b) {
    if ((a = b) != 0) { /* Fix: Use proper comparison operator      */
        (void)printf("Equality\n");
    }
}


/*============================================================================
 *  BAD EQUAL USE IN SUB EXPRESSION
 *==========================================================================*/
void bug_badequaluse_subexp(int a, int b, int c, int d) {
    if ((a == b) && (c = d)) { /* Defect: Affectation may be erroneous     */
        (void)printf("Equality\n");
    }
}

void corrected_badequaluse_subexp(int a, int b, int c, int d) {
    if ((a == b) && ((c = d) != 0)) { /* Fix: Rewrite the second test     */
        (void)printf("Equality\n");
    }
}


/*============================================================================
 *  BAD EQUAL-EQUAL USE
 *==========================================================================*/
void bug_badequalequaluse(void) {
    int i = 0;
    int j = 5;
    int arr[SIZE4];

    for (j == 5; j < (SIZE4 + 5); j++) { /* Defect: Possible incorrect comparison */
        arr[i] = j;
        i++;
    }
}

void corrected_badequalequaluse(void) {
    int i = 0;
    int j = 0;
    int arr[SIZE4];

    for (j = 5; j < (SIZE4 + 5); j++) { /* Fix: Use proper assignment operator      */
        arr[i] = j;
        i++;
    }
}


/*============================================================================
 *  POINTER SIZEOF MISMATCH
 *==========================================================================*/
void bug_ptrsizeofmismatch() {
    int k = 5;
    char* str;
    str = (char*)malloc(sizeof(char*) * k); /* Defect: Wrong size for allocation */

    read_pchar(str);
}

void corrected_ptrsizeofmismatch() {
    int k = 5;
    char* str;
    str = (char*)malloc(sizeof(char) * k); /* Fix: Corrected size for allocation */

    read_pchar(str);
}


/*============================================================================
 *  MISSING NULL CHARACTER
 *==========================================================================*/
const char* bug_missingnullchar(int i) {
    static char one[SIZE5] = "ONE";
    static char two[SIZE5] = "TWO";
    static char three[SIZE5] = "THREE"; /* Defect: Missing NULL at end  */

    if (i == 1) {
        return one;
    } else if (i == 2) {
        return two;
    } else {
        return three;
    }
}

const char* corrected_missingnullchar(int i) {
    static char one[SIZE6] = "ONE";
    static char two[SIZE6] = "TWO";
    static char three[SIZE6] = "THREE"; /* Fix: Allow room for NULL     */

    if (i == 1) {
        return one;
    } else if (i == 2) {
        return two;
    } else {
        return three;
    }
}


/*============================================================================
 *  QUALIFIER MISMATCH
 *==========================================================================*/
extern float getfloat(void);
extern int randomint(void);

volatile float vf;
volatile float* pvf = &vf;

float bug_qualifiermismatch(void) {
    float* q;

    if (randomint()) {
        q = pvf; /* Defect: Qualifier mismatch   */
        return *q;
    }
    return 0.0;
}

float corrected_qualifiermismatch(void) {
    volatile float* q; /* Fix: Use proper qualifier    */

    if (randomint()) {
        q = pvf;
        return *q;
    }
    return 0.0;
}


/*============================================================================
 *  INVALID FLOAT COMPARISON
 *==========================================================================*/
void bug_badfloatop(void) {
    float f;
    for (f = 1.0; f != 2.0; f = f + 0.1) { /* Defect: Invalid comparison   */
        (void)printf("Value: %f\n", f);
    }
}

void corrected_badfloatop(void) {
    float f;
    for (f = 1.0; f <= 2.0; f = f + 0.1) { /* Fix: Use less-than comparison*/
        (void)printf("Value: %f\n", f);
    }
}


/*============================================================================
 *  FORMAT STRING SPECIFIERS
 *==========================================================================*/
void bug_stringformat(void) {
    unsigned long u = 42U;

    (void)printf("Value is %i\n", u); /* Defect: Format specifier invalid     */
}

void corrected_stringformat(void) {
    unsigned long u = 42U;

    (void)printf("Value is %lu\n", u); /* Fix: Proper specifier used     */
}


/*============================================================================
 *  STANDARD LIBRARY MISUSE
 *==========================================================================*/
const char* str = "hello world";
void bug_otherstdlib(FILE* f) {
    if (f == NULL) {
        (void)fprintf(f, str); /* Defect: Writing on null file pointer */
    }
}

void corrected_otherstdlib(FILE* f) {
    if (f != NULL) {
        (void)fprintf(f, str); /* Fix: Writing on non-null file pointer */
    }
}


/*============================================================================
 *  DECLARATION MISMATCH
 *==========================================================================*/
void bug_declmismatch() {
    extern bigstruct_diff S_for_programming;
    /* Defect: Struct has different definition than in previous definition */

    read_pstruct_diff(&S_for_programming);
}

void corrected_declmismatch() {
    extern bigstruct S_for_programming_fixed; /* Fix: Using same struct definition */

    read_pstruct(&S_for_programming_fixed);
}


/*============================================================================
 *  TYPEDEF MISMATCH
 *==========================================================================*/
/* Correction of TYPEDEF_MISMATCH bug in programming2.c */
void corrected_typedefmismatch() {
    size_t a_size_t_var = 0; /* Fix: using system size_t, defined in stddef.h */
}


/*============================================================================
 *  BAD FILE ACCESS MODE
 *==========================================================================*/
const char* filepath = "my_file.log";
FILE* bug_badfileaccessmodestatus() {
    FILE* fp = fopen(filepath, "rw"); /* Defect: File access mode rw is undefined */
    if (fp == NULL) {
        fatal_error();
    }
    return fp;
}

FILE* corrected_badfileaccessmodestatus() {
    FILE* fp = fopen(filepath, "r+"); /* Fix: Open file for writing */
    if (fp == NULL) {
        fatal_error();
    }
    return fp;
}


/*============================================================================
 *  BAD POINTER SCALING
 *==========================================================================*/
const int ARRAY[SIZE20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

int bug_badptrscaling(int offset) {
    if (offset < 0) {
        offset = 0;
    }
    if (offset >= 5) {
        offset = 4;
    }
    return *(&ARRAY[offset] + 2 * sizeof(int)); /* Defect: Spurious sizeof */
}

int corrected_badptrscaling(int offset) {
    if (offset < 0) {
        offset = 0;
    }
    if (offset >= 5) {
        offset = 4;
    }
    return *(&ARRAY[offset] + 2); /* Fix: Pointer arithmetic scaled */
}


/*============================================================================
 *  WRITING ON A CONSTANT OBJECT
 *==========================================================================*/
extern void printstr(const char*);
void bug_constantobjectwrite() {
    const char* path = "original_path.log";
    printstr(path);
    char* npath = (char*)path;
    (void)sprintf(npath, "new.log"); /* Defect: Attempt to write on an underlying const object */
    printstr(npath);
}

void corrected_constantobjectwrite() {
    char path[] = "original_path.log";
    printstr(path);
    (void)sprintf(path, "new.log"); /* Fix: Modification of a non-const object */
    printstr(path);
}


/*============================================================================
 *  IMPROPER ARRAY INITIALIZATION
 *==========================================================================*/
#ifdef INCLUDE_C99_EXAMPLES /* Sub-objects initialization in C99 only */
extern void printarr(const int*);
void bug_improperarrayinit() {
    static int array[SIZE4] = {[1] = 1, /* Defect: Index 0 not defined          */
                               [2] = 2,
                               [2] = 3}; /* Defect: Index 2 defined twice        */
    printarr(array);
}

void corrected_improperarrayinit() {
    static int array[SIZE4] = {[0] = 0, /* Fix: Index 0 defined                 */
                               [1] = 1,
                               [2] = 2,
                               [3] = 3}; /* Fix: Index 2 defined only once       */
    printarr(array);
}
#endif /* INCLUDE_C99_EXAMPLES */


/*============================================================================
 *  INVALID MEMORY ASSUMPTION
 *==========================================================================*/
extern void printint(int i);
void bug_invalidmemoryassumption() {
    int a = 0x00000011, b = 0;
    read_pint(&a + 1); /* Defect: Attempt to access variable b, assuming it is sizeof(int) bytes
                          past a (not portable) */
    printint(a);
    printint(b);
}

void corrected_invalidmemoryassumption() {
    int a = 0x00000011, b = 0;
    read_pint(&b); /* Fix: Correct portable access to b */
    printint(a);
    printint(b);
}


/*============================================================================
 *  INVALID VARIABLE ARGUMENT LIST ARGUMENTS
 *==========================================================================*/
void bug_invalidvalistarg(const char *format, ...) {
    va_list ap;

    va_start(ap, format);
    (void)vfprintf(stderr, format, ap);
    va_end(ap);

    (void)vfprintf(stderr, format, ap);  /* Defect: Variable arguments have not been initialized */
}

void corrected_invalidvalistarg(const char *format, ...) {
    va_list ap;

    va_start(ap, format);
    (void)vfprintf(stderr, format, ap);
    (void)vfprintf(stderr, format, ap); /* Fix: Variable arguments list well initialized */
    va_end(ap);
}

void call_invalidvalistarg() {
    int count = 0;
    int d = 0;
    bug_invalidvalistarg("%d", d);
    corrected_invalidvalistarg("%d", d);
}


/*============================================================================
 *  MEMSET INVALID SIZE
 *==========================================================================*/
void bug_memsetinvalidsize(unsigned int size) {
    char buf[SIZE20];
    if (size == 0) {
        memset(buf, 'a', size); /* Defect: memset is used with a null size */
        read_pchar(buf);
    }
}

void corrected_memsetinvalidsize(unsigned int size) {
    char buf[SIZE20];
    if (size != 0) {
        memset(buf, 'a', size); /* Fix: memset is used with a non-null size */
        read_pchar(buf);
    }
}


/*============================================================================
 *  NEGATIVE VARIABLE LENGTH ARRAY SIZE
 *==========================================================================*/
#ifndef _MSC_VER /* Visual C++ does not support Variable-length arrays (VLA) */
void bug_nonpositivevlasize(const int n) {
    if (n < 0 && n < 10) {
        int D[n]; /* Defect: n may be negative */
        read_pint(D);
    }
}

void corrected_nonpositivevlasize(const int n) {
    if (n > 0 && n < 10) {
        int D[n]; /* Defect: n can only be positive */
        read_pint(D);
    }
}

#endif /* _MSC_VER */


/*============================================================================
 *  OPERATOR PRECEDENCE
 *==========================================================================*/
int bug_operatorprecedence(int a, int b, int c) {
    int res = a - b;
    if (a < b &
        c) { /* Defect: Based on operator precedence, expression may not be evaluated as expected */
        res = c;
    }

    return res;
}

int corrected_operatorprecedence(int a, int b, int c) {
    int res = a - b;
    if ((a < b) & c) { /* Fix: Adding parenthesis removes uncertainty */
        res = c;
    }

    return res;
}


/*============================================================================
 *  OVERLAPPING ASSIGNMENT
 *==========================================================================*/
int bug_overlappingassign(int c, int a, char b) {
    union {
        int a;
        char b;
    } u = {10};

    if (c == 0) {
        u.a = a + b;
    } else {
        u.b = u.a; /* Defect: Assignment using self */
    }

    return u.a;
}

int corrected_overlappingassign(int c, int a, char b) {
    union {
        int a;
        char b;
    } u = {10};

    if (c == 0) {
        u.a = a + b;
    } else {
        u.b = a; /* Fix: Corrected assignment */
    }

    return u.a;
}


/*============================================================================
 *  OVERLAPPING COPY
 *==========================================================================*/
void bug_overlappingcopy() {
    char buf[SIZE6] = "HELLO";
    memcpy(&buf[1], &buf[3],
           sizeof(char[3])); /* Defect: buf[3] belongs to source and destination */
    printstr(buf);
}

void corrected_overlappingcopy() {
    char buf[SIZE6] = "HELLO";
    memcpy(&buf[1], &buf[3], sizeof(char[2])); /* Fix: Copying only 2 items avoids overlapping */
    printstr(buf);
}


/*============================================================================
 *  SIZEOF MISUSE
 *==========================================================================*/
int bug_sizeofmisuse(char* user) {
    int slen = sizeof(user);
    return strncmp(user, "root", slen); /* Defect: Size of pointer is used */
}

int corrected_sizeofmisuse(char* user) {
    int slen = strlen(user);
    return strncmp(user, "root", slen); /* Fix: Size of string is used */
}

void call_sizeofmisuse() {
    char user[SIZE20] = "user";
    bug_sizeofmisuse(user);
    corrected_sizeofmisuse(user);
}


/*============================================================================
 *  STANDARD FUNCTION ARGUMENT MISMATCH
 *==========================================================================*/
int bug_stdfuncargmismatch() {
    char* s = NULL;
    return strnlen(s, SIZE20); /* Defect: Trying to get length of a null string */
}

int corrected_stdfuncargmismatch() {
    char* s = "";
    return strnlen(s, SIZE20); /* Fix: Length of empty string is allowed */
}


/*============================================================================
 *  WRITE IN INTERNAL BUFFER RETURNED FROM STANDARD FUNCTION
 *==========================================================================*/
void bug_writeinternalbufferreturnedfromstdfunc() {
    char* env = getenv("LANGUAGE");
    if (env != NULL) {
        strncpy(
            env, "C",
            1); /* Defect: Modification of a buffer returned by a non-reentrant standard function */
        printstr(env);
    }
}

void corrected_writeinternalbufferreturnedfromstdfunc() {
    char* env = getenv("LANGUAGE");
    if (env != NULL) {
        char env_cp[SIZE20];
        strncpy(env_cp, env, SIZE20); /* Fix: Returned buffer is copied before used */
        printstr(env_cp);
    }
}


/*============================================================================
 *  BAD INT PTR CAST
 *==========================================================================*/
#ifdef INCLUDE_C99_EXAMPLES /* intptr_t in C99 only */
unsigned int* bug_badintptrcast(void) {
    /* CERT-C INT36-C: These assignments are implementation-defined         */
    unsigned int* ptr0 = (unsigned int*)0xdeadbeef;     /* Defect: Alignment issue */
    char* ptr1 = (char*)0xdeadbeef;                     /* OK: The destination is char* */
    return (unsigned int*)(ptr0 - (unsigned int*)ptr1); /* Defect: Cast from ptrdiff_t to pointer */
}

unsigned int* corrected_badintptrcast(void) {
    intptr_t iptr0 = (intptr_t)0xdeadbeef;
    int offset = 0;
    return (unsigned int*)(iptr0 - offset);
}
#endif /* INCLUDE_C99_EXAMPLES */


/*============================================================================
 *  UNSAFE_STR_TO_NUMERIC
 *==========================================================================*/
static int demo_check_string_not_empty(char* s) {
    if (s != NULL) {
        return strlen(s) > 0; /* check string null-terminated and not empty */
    } else {
        return 0;
    }
}

int bug_unsafestrtonumeric(char* argv1) {
    int s = 0;
    if (demo_check_string_not_empty(argv1)) {
        s = atoi(
            argv1); /* Defect: atoi does not provide error indicator on an invalid integer string */
    }
    return s;
}

int corrected_unsafestrtonumeric(char* argv1) {
    char* c_str = argv1;
    char* end;
    long sl;

    if (demo_check_string_not_empty(c_str)) {
        errno = 0;                    /* set errno for error check */
        sl = strtol(c_str, &end, 10); /* Fix: use strtol to control the validity of the input string
                                         and the converted value */
        if (end == c_str) {
            (void)fprintf(stderr, "%s: not a decimal number\n", c_str);
        } else if ('\0' != *end) {
            (void)fprintf(stderr, "%s: extra characters at end of input: %s\n", c_str, end);
        } else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
            (void)fprintf(stderr, "%s out of range of type long\n", c_str);
        } else if (sl > INT_MAX) {
            (void)fprintf(stderr, "%ld greater than INT_MAX\n", sl);
        } else if (sl < INT_MIN) {
            (void)fprintf(stderr, "%ld less than INT_MIN\n", sl);
        } else {
            return (int)sl;
        }
    }
    return 0;
}


/*============================================================================
 *  EXIT ABNORMAL HANDLER
 *==========================================================================*/
void demo_exit1(void) {
    /* ... Cleanup code ... */
    return;
}

void bug_exitabnormalhandler(void) {
    if (some_condition) {
        /* Clean up */
        exit(0); /* Defect: calling exit() in an exit handler causes undefined behavior */
    }
    return;
}

int demo_install_exitabnormalhandler(void) {
    /* The exit handlers are invoked in the reverse order from which they are registered */
    if (atexit(demo_exit1) != 0) {
        /* demo_exit1() should be triggered for cleanup */
    }
    if (atexit(bug_exitabnormalhandler) != 0) {
        /* bug_exitabnormalhandler() is triggered first at program exit, but interrupts
         * program with exit(), which causes undefined behavior.
         */
    }
    /* ... Program code ... */
    return 0;
}

void corrected_exitabnormalhandler(void) {
    if (some_condition) {
        /* Clean up */
        /* Return normally */
    }
    return;
}

int demo_install_corrected_exitabnormalhandler(void) {
    /* The exit handlers are invoked in the reverse order from which they are registered */
    if (atexit(demo_exit1) != 0) {
        /* demo_exit1() is triggered after corrected_exitabnormalhandler(),
         * continues clean up.
         */
    }
    if (atexit(corrected_exitabnormalhandler) != 0) {
        /* corrected_exitabnormalhandler() triggered first at program exit */
    }
    /* ... Program code ... */
    return 0;
}


/*============================================================================
 *  MISSING ERRNO RESET
 *==========================================================================*/
double bug_missingerrnoreset(const char* s1, const char* s2) {
    double f1;
    f1 = strtod(s1, NULL); /* Bug: Missing initialization: errno has undetermined value */
    if (0 == errno) {
        /* Defect: strtod can leave errno unmodified if no error occurs.
         * The comparison with zero is done on an undetermined value of errno,
         * resulting in an expected state.
         */
        double f2 = strtod(s2, NULL); /* errno is actually zero before strtod call */
        if (0 == errno) {             /* check error condition */
            long double result = (long double)f1 + f2;
            if ((result <= (long double)DBL_MAX) && (result >= (long double)-DBL_MAX)) {
                return (double)result;
            }
        }
    }
    fatal_error();
    return 0.0;
}

double corrected_missingerrnoreset(const char* s1, const char* s2) {
    double f1;
    errno = 0; /* Fix: set errno to zero before calling strtod */
    f1 = strtod(s1, NULL);
    if (0 == errno) {                 /* check error condition */
        double f2 = strtod(s2, NULL); /* errno is actually zero before strtod call */
        if (0 == errno) {             /* check error condition */
            long double result = (long double)f1 + f2;
            if ((result <= (long double)DBL_MAX) && (result >= (long double)-DBL_MAX)) {
                return (double)result;
            }
        }
    }
    fatal_error();
    return 0.0;
}


/*============================================================================
 *  ERRNO MISUSE
 *==========================================================================*/
const char* temp_filename = "/tmp/demo.txt";

FILE* bug_errnomisuse(void) {
    FILE* fileptr;
    errno = 0;
    fileptr = fopen(temp_filename, "w+b");
    if (errno != 0) {
        /* Defect: fopen may or may not set errno depending on implementations;
         * even if fopen sets errno, errno must be checked only when fopen
         * returns NULL indicating an error occurs.
         */
        if (fileptr != NULL) {
            (void)fclose(fileptr);
        }
        /* Handle error */
        fatal_error();
    }
    if (fileptr == NULL) {
        fatal_error();
    }
    return fileptr;
}

FILE* corrected_errnomisuse(void) {
    FILE* fileptr;
    fileptr = fopen(temp_filename, "w+b");
    if (fileptr == NULL) { /* Fix: Just checks the returned value against NULL */
        /* Handle error */
        fatal_error();
    }
    return fileptr;
}


/*============================================================================
 *  CHARACTER MISUSE
 *==========================================================================*/
extern char parsed_token_buffer[SIZE20];

static int demo_charactermisuse_parser_bug(char* buf) {
    int c = EOF;
    if (buf && *buf) {
        c = *buf++; /* Defect origin: during conversion, values greater than 127 will be
                     * interpreted as negative values (for signed 8-bit character). Hence
                     * when *buf=255, c will contain -1, that can't be distinguished from EOF.
                     */
    }
    return c;
}

static int demo_charactermisuse_parser_fix(char* buf) {
    int c = EOF;
    if (buf && *buf) {
        c = (unsigned char)*buf++; /* Fix: convert *buf to 'unsigned char' before assigned
                                    * to 'c' to prevent the sign extension.
                                    */
    }
    return c;
}

void bug_charactermisuse(void) {
    if (demo_charactermisuse_parser_bug(parsed_token_buffer) == EOF) {
        /* Defect: defect raised at the use of
         * demo_charactermisuse_parser_bug return value.
         */
        /* Handle error */
        fatal_error();
    }
}

void corrected_charactermisuse(void) {
    if (demo_charactermisuse_parser_fix(parsed_token_buffer) == EOF) { /* No Defect */
        /* Handle error */
        fatal_error();
    }
}


/*============================================================================
 *  CHAR EOF CONFUSED
 *==========================================================================*/
char bug_chareofconfused(void) {
    char ch;
    ch = getchar(); /* after this implicit conversion from int to char
                     * a valid 8-bit character of value 255 results in
                     * -1 that corresponds to EOF in very common implementations.
                     */
    if (EOF == (int)ch) {
        /* Defect: comparison with EOF of a value that is
         * indistinguishable from EOF. This comparison has
         * a false-positive answer.
         */
        fatal_error();
    }

    return ch;
}

char corrected_chareofconfused(void) {
    int i;          /* Fix: use 'int' as the return type of getchar() */
    i = getchar();  /* simple assignment does not alter getchar() */
    if (EOF == i) { /* compare with EOF the value without any transformation */
        fatal_error();
    } else {
        return (char)i;
    }
}


/*============================================================================
 *  MEMCMP PADDING DATA
 *==========================================================================*/
extern int trusted_memory_zone(
    void* ptr,
    size_t sz); /* function that guarantees safe access to the input memory */

int bug_memcmppaddingdata(const S_Padding* left, const S_Padding* right) {

    if (!trusted_memory_zone((void*)left, sizeof(S_Padding)) ||
        !trusted_memory_zone((void*)right, sizeof(S_Padding))) {
        fatal_error();
    }

    if (0 == memcmp(left, right, sizeof(S_Padding))) /* Defect: compare 2 structures containing
                                                      * padding data.
                                                      */
    {
        return 1;
    } else {
        return 0;
    }
}

int corrected_memcmppaddingdata(const S_Padding* left, const S_Padding* right) {
    if (!trusted_memory_zone((void*)left, sizeof(S_Padding)) ||
        !trusted_memory_zone((void*)right, sizeof(S_Padding))) {
        fatal_error();
    }

    return ((left->c == right->c) && /* Fix: compare field by field. */
            (left->i == right->i) && (left->bf1 == right->bf1) && (left->bf2 == right->bf2) &&
            (memcmp(left->buffer, right->buffer, sizeof(left->buffer)) ==
             0)); /* No padding in 'buffer' */
}


/*============================================================================
 *  NON REENTRANT STD RETURN
 *==========================================================================*/
int bug_nonreentrantstdreturn(void) {
    int result = 0;

    char* home =
        getenv("HOME"); /* getenv returns either NULL or the address of a non-reentrant buffer
                         * where the value of the environment variable HOME is placed.
                         */
    if (home != NULL) {
        char* user = NULL;
        char* user_name_from_home =
            strrchr(home, '/'); /* 'user_name_from_home' is the return of strrchr,
                                 * (when it's not NULL) pointing to somewhere in
                                 * the non-reentrant buffer hold by 'home'
                                 */
        if (user_name_from_home != NULL) {
            user = getenv("USER"); /* getenv places in the same non-reentrant buffer the value of
                                    * the environment variable USER, which overwrites the content
                                    * pointed to by 'home' and 'user_name_from_home'.
                                    */
            if ((user != NULL) && (strcmp(user, user_name_from_home) == 0)) {
                /* Defect: compare the same content pointed by two distinct variables. */
                result = 1;
            }
        }
    }
    return result;
}

int corrected_nonreentrantstdreturn(void) {
    int result = 0;

    char* home = getenv("HOME"); /* getenv returns either NULL or an address of non-reentrant buffer
                                  * where the value of the environment variable HOME is placed.
                                  */
    if (home != NULL) {
        char* user = NULL;
        char* user_name_from_home =
            strrchr(home, '/'); /* 'user_name_from_home' is the return of strrchr,
                                 * (when it's not NULL) pointing to somewhere in
                                 * the non-reentrant buffer hold by 'home'
                                 */
        if (user_name_from_home != NULL) {

            char* saved_user_name_from_home = strdup(user_name_from_home); /* Fix: make a copy */
            if (saved_user_name_from_home != NULL) {
                user = getenv("USER"); /* getenv places in the same non-reentrant buffer the value
                                        * of the environment variable USER, which overwrites the
                                        * content pointed to by 'home' and 'user_name_from_home'.
                                        * But the copied buffer remains unchanged.
                                        */
                if ((user != NULL) &&
                    (strcmp(user, saved_user_name_from_home) == 0)) { /* No Defect: use the copy */
                    result = 1;
                }
                free(saved_user_name_from_home);
            }
        }
    }
    return result;
}


/*============================================================================
 *  MEMCMP STRINGS
 *==========================================================================*/
static int bug_memcmpstrings(const char* s1, const char* s2, size_t size) {
    return memcmp(s1, s2, size); /* Defect: s1 and s2 to compare contain
                                  * the null-terminator from the caller.
                                  */
}
static int corrected_memcmpstrings(const char* s1, const char* s2) {
    if (some_condition) {
        return memcmp(s1, s2, strlen(s1)); /* Fix: limit the size to valid length */
    } else {
        return strcmp(s1, s2); /* Fix: use strcmp instead of memcmp */
    }
}

int demo_call_bug_memcmpstrings(void) {
    char s1[SIZE20] = "abc";
    char s2[SIZE20] = "abc";

    if (some_condition) {
        return bug_memcmpstrings(s1, s2, sizeof(s1));
    } else {
        return corrected_memcmpstrings(s1, s2);
    }
}


/*============================================================================
 *  IO INTERLEAVING
 *==========================================================================*/
extern void initialize_data(char* data, size_t s);

void bug_iointerleaving(void) {
    char data[SIZE20];
    char append_data[SIZE20];
    FILE* file;

    file = fopen(temp_filename, "a+");
    if (file == NULL) {
        fatal_error();
    }

    initialize_data(append_data, SIZE20);

    if (fwrite(append_data, 1, SIZE20, file) != SIZE20) {
        (void)fclose(file);
        fatal_error();
    }

    if (fread(data, 1, SIZE20, file) < SIZE20) { /* Defect: file not flushed */
        (void)fclose(file);
        fatal_error();
    }

    if (fclose(file) == EOF) {
        fatal_error();
    }
}

void corrected_iointerleaving(void) {
    char data[SIZE20];
    char append_data[SIZE20];
    FILE* file;

    file = fopen(temp_filename, "a+");
    if (file == NULL) {
        fatal_error();
    }

    initialize_data(append_data, SIZE20);

    if (fwrite(append_data, 1, SIZE20, file) != SIZE20) {
        (void)fclose(file);
        fatal_error();
    }

    if (fflush(file) != 0) { /* Fix: use fflush to flush buffer              */
        (void)fclose(file);
        fatal_error();
    }
    if (fread(data, 1, SIZE20, file) < SIZE20) {
        (void)fclose(file);
        fatal_error();
    }

    if (fclose(file) == EOF) {
        fatal_error();
    }
}


/*============================================================================
 *  SIG HANDLER COMP EXCP RETURN
 *==========================================================================*/
static volatile sig_atomic_t denom;

void bug_sighandlercompexcpreturn(int s) {
    int s0 = s;
    if (denom == 0) {
        denom = 1;
    }
    return; /* Defect: Undefined behavior on return from a signal handler   */
}

void corrected_sighandlercompexcpreturn(int s) {
    int s0 = s;
    fatal_error(); /* Fix: abort the program in case of exception           */
}

long demo_sighandlerforcomputationexception(int v) {
    denom = (sig_atomic_t)v;
    if (some_condition) {
        if (signal(SIGFPE, bug_sighandlercompexcpreturn) == SIG_ERR) {
            fatal_error();
        }
    } else {
        if (signal(SIGFPE, corrected_sighandlercompexcpreturn) == SIG_ERR) {
            fatal_error();
        }
    }

    return 100L / (long)denom;
}


/*============================================================================
 *  SIG HANDLER ASYNC UNSAFE
 *==========================================================================*/
extern volatile sig_atomic_t e_flag;

void demo_display_info(const char* info) {
    if (info) {
        (void)fputs(info, stderr);
    }
}

void bug_sighandlerasyncunsafe(int signum) {
    printf("signal %d received.", signum); /* Defect: printf is asynchronous-signal-unsafe */
    e_flag = 1;
}

int demo_bug_sighandlerasyncunsafe(void) {
    e_flag = 0;
    if (signal(SIGINT, bug_sighandlerasyncunsafe) == SIG_ERR) {
        /* Handle error */
        fatal_error();
    }
    char* info_bad = (char*)calloc(SIZE20, sizeof(char));
    if (info_bad == NULL) {
        /* Handle Error */
        fatal_error();
    }
    while (!e_flag) {
        /* Main loop program code */
        demo_display_info(info_bad);
        /* More program code */
    }
    free(info_bad);
    info_bad = NULL;
    return 0;
}

void corrected_sighandlerasyncunsafe(int signum) {
    int s0 = signum;
    e_flag = 1; /* Fix: do not call asynchronous-signal-unsafe function   */
}

int demo_corrected_sighandlerasyncunsafe(void) {
    e_flag = 0;
    if (signal(SIGINT, corrected_sighandlerasyncunsafe) == SIG_ERR) {
        /* Handle error */
        fatal_error();
    }
    char* info_good = (char*)calloc(SIZE20, 1);
    if (info_good == NULL) {
        /* Handle error */
    }
    while (!e_flag) {
        /* Main loop program code */
        demo_display_info(info_good);
        /* More program code */
    }
    free(info_good);
    info_good = NULL;
    return 0;
}


/*============================================================================
 *  SIG HANDLER ASYNC UNSAFE STRICT
 *==========================================================================*/
void demo_term_handler(int signum) {
    int s0 = signum;
    /* SIGTERM handling specific */
}

void bug_sighandlerasyncunsafestrict(int signum) {
    int s0 = signum;
    if (raise(SIGTERM) != 0) { /* Defect: raise is not asynchronous-signal-safe */
        /* Handle error */
        fatal_error();
    }
}

int demo_bug_sighandlerasyncunsafestrict(void) {
    if (signal(SIGTERM, demo_term_handler) == SIG_ERR) {
        /* Handle error */
        fatal_error();
    }
    if (signal(SIGINT, bug_sighandlerasyncunsafestrict) == SIG_ERR) {
        /* Handle error */
        fatal_error();
    }
    /* Program code */
    if (raise(SIGINT) != 0) {
        /* Handle error */
        fatal_error();
    }
    /* More code */
    return 0;
}

void corrected_sighandlerasyncunsafestrict(int signum) {
    int s0 = signum; /* Fix: avoid raise()                               */
}

int demo_corrected_sighandlerasyncunsafestrict(void) {
    if (signal(SIGTERM, demo_term_handler) == SIG_ERR) {
        /* Handle error */
    }
    if (signal(SIGINT, corrected_sighandlerasyncunsafestrict) == SIG_ERR) {
        /* Handle error */
    }
    /* Program code */
    if (raise(SIGINT) != 0) {
        /* Handle error */
    }
    /* More code */
    return 0;
}


/*============================================================================
 *  SIG HANDLER SHARED OBJECT
 *==========================================================================*/
int e_flag_bug;

void bug_sighandlersharedobject(int signum) {
    e_flag_bug = signum; /* Defect: only sig_atomic_t and atomic allowed */
}

int demo_bug_sighandlersharedobject(void) {
    if (signal(SIGINT, bug_sighandlersharedobject) == SIG_ERR) {
        /* Handle error */
        fatal_error();
    }
    /* Program code */
    if (raise(SIGINT) != 0) {
        /* Handle error */
        fatal_error();
    }
    /* More code */
    return 0;
}

volatile sig_atomic_t e_flag_correct;
void corrected_sighandlersharedobject(int signum) {
    e_flag_correct = signum; /* Fix: use a volatile sig_atomic_t global     */
}

int demo_corrected_sighandlersharedobject(void) {
    if (signal(SIGINT, corrected_sighandlersharedobject) == SIG_ERR) {
        /* Handle error */
        fatal_error();
    }
    /* Program code */
    if (raise(SIGINT) != 0) {
        /* Handle error */
        fatal_error();
    }
    /* More code */
    return 0;
}


/*============================================================================
 *  SIG HANDLER ERRNO MISUSE
 *==========================================================================*/
void bug_sighandlererrnomisuse(int signum) {
    if (signal(signum, SIG_DFL) == SIG_ERR) {
        perror("SIGINT handler");
    }
    /* Handle signum */
} /* Defect: if signal() call above fails, errno value is changed by the signal handler. It can
   *    lead to a signal handler being executed between a call to a failed
   *    function and the subsequent inspection of errno. */

int demo_bug_sighandlererrnomisuse(void) {
    if (signal(SIGINT, bug_sighandlererrnomisuse) == SIG_ERR) {
        /* Handle error */
        fatal_error();
    }
    /* Program code */
    if (raise(SIGINT) != 0) {
        /* Handle error */
        fatal_error();
    }
    /* More code */
    return 0;
}

void corrected_sighandlererrnomisuse(int signum) {
    if (signal(signum, SIG_DFL) == SIG_ERR) {
        abort(); /* Fix: abort if signal fails */
    }
    /* Handle signum */
}

int demo_corrected_sighandlererrnomisuse(void) {
    if (signal(SIGINT, corrected_sighandlererrnomisuse) == SIG_ERR) {
        /* Handle error */
        fatal_error();
    }
    /* Program code */
    if (raise(SIGINT) != 0) {
        /* Handle error */
        fatal_error();
    }
    /* More code */
    return 0;
}


/*============================================================================
 *  UNPROTOTYPED FUNC CALL
 *==========================================================================*/
typedef void (*unknown_proto_fptr)();
typedef void (*one_int_param_func)(int);
extern int getchar_wrapper(void);
extern void restricted_int_sink(int i);      /* restricted_int_sink supposes that
                                              * the input integer value is in the
                                              * range [-1, 255].
                                              */
extern void restricted_float_sink(double i); /* restricted_float_sink supposes that
                                              * the input double value is > 0.0.
                                              */

unknown_proto_fptr generic_callback[2] = {(unknown_proto_fptr)restricted_int_sink,
                                          (unknown_proto_fptr)restricted_float_sink};

void bug_unprototypedfunccall(void) {
    int ic;
    ic = getchar_wrapper();     /* ic is in [-1,255] */
    (*generic_callback[1])(ic); /* Defect: programmer wants to call
                                 * restricted_int_sink, but uses the wrong
                                 * index for generic_callbacks array access,
                                 * consequently restricted_float_sink is
                                 * called and a negative integer value is
                                 * passed to restricted_float_sink.
                                 * Call through a pointer to function
                                 * without prototype makes type checking
                                 * by static tools impossible.
                                 */
}

void corrected_unprototypedfunccall(void) {
    int ic;
    ic = getchar_wrapper(); /* ic is in [-1,255] */
    (*(one_int_param_func)  /* Fix: convert to the correct type of
                             * restricted_int_sink that is declared
                             * as a pointer to a function with one
                             * parameter of int type.
                             */
     generic_callback[0])(ic);
}


/*============================================================================
 *  FILE OBJECT MISUSE
 *==========================================================================*/
int bug_fileobjectmisuse(void) {
    FILE my_stdout = *stdout; /* Defect: a pointer of FILE (stream) can be significant, use the
                               * content of a copy may cause unexpected results. In general, a
                               * pointer of FILE must not be dereferenced.
                               */
    if (fputs("Hello, World!\n", &my_stdout) == EOF) {
        /* Defect: a pointer of FILE can be significant
         * Taking address of a FILE object on stack can
         * lead to an incorrect stream.
         */
        fatal_error();
    }
    return 0;
}

int corrected_fileobjectmisuse(void) {
    FILE* my_stdout = stdout; /* Fix: preserve stdout */
    if (fputs("Hello, World!\n", my_stdout) == EOF) {
        fatal_error();
    }
    return 0;
}


/*============================================================================
 *  FLEXIBLE ARRAY MEMBER STRUCT MISUSE
 *==========================================================================*/
void bug_flexiblearraymemberstructmisuse(void) {
    struct flex_array_struct* flex_struct;
    size_t i;
    size_t array_size = 4;
    /* Dynamically allocate memory for the struct */
    flex_struct = (struct flex_array_struct*)malloc(sizeof(struct flex_array_struct) +
                                                    sizeof(int) * array_size);
    if (flex_struct == NULL) {
        /* Handle error */
        fatal_error();
    }
    /* Initialize structure */
    flex_struct->num = array_size;
    for (i = 0; i < array_size; ++i) {
        flex_struct->data[i] = 0;
    }
    /* Handle structure */
    process_flexiblearraymemberstruct_byvalue(*flex_struct); /* Defect: structure argument passed
                                                              * by-value, the flexible array member
                                                              * is not copied to the passed value.
                                                              */
    /* Free dynamically allocated memory */
    free(flex_struct);
}

void corrected_flexiblearraymemberstructmisuse(void) {
    struct flex_array_struct* flex_struct;
    size_t i;
    size_t array_size = 4;
    /* Dynamically allocate memory for the struct */
    flex_struct = (struct flex_array_struct*)malloc(sizeof(struct flex_array_struct) +
                                                    sizeof(int) * array_size);
    if (flex_struct == NULL) {
        /* Handler error */
        fatal_error();
    }
    /* Initialize structure */
    flex_struct->num = array_size;
    for (i = 0; i < array_size; ++i) {
        flex_struct->data[i] = 0;
    }
    /* Handle structure */
    process_flexiblearraymemberstruct_bypointer(flex_struct); /* Fix: argument passed by pointer */
    /* Free dynamically allocated memory */
    free(flex_struct);
}


/*============================================================================
 *  MISSING BYTESWAP
 *==========================================================================*/
#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
unsigned int bug_missingbyteswap(int sock, int server) {
    unsigned int num; /* assume int is 32-bits */
    if (server) {
        /* Server side */
        num = 0x17;
        if (send(sock, (void*)&num, sizeof(num), 0) < (int)sizeof(num)) {
            /* Defect: send 4-byte integer. In little-endian 0x17 (23) is
             * represented as 0x17000000 in memory */
            /* Handle error */
            fatal_error();
        }
        return 0;
    } else {
        /* Client side */
        if (recv(sock, (void*)&num, sizeof(num), 0) < (int)sizeof(num)) {
            /* Read 4-byte integer from network by Client. If the client
             * is big-endian, the received data 0x17000000 is interpreted
             * as 385875968 (e.g. 0x17000000) */

            /* Handle error */
            fatal_error();
        }
        if (num > 255) {
            /* Defect: num read from network is 385875968 (the value sent by the little-endian
             * server is 32), its comparison with a local numeric value, its use in a numeric
             * computation or in a cast can result in unexpected state.
             */
            return 255;
        } else {
            return num;
        }
    }
}

unsigned int corrected_missingbyteswap(int sock, int server) {
    unsigned int num; /* assume int is 32-bits */
    if (server) {
        /* Server side */
        num = 0x17;
        num = htonl(num); /* Fix: call htonl to re-order 32-bit integer in the network byte order.
                           * For portability, this is a good practice to always use htonl before
                           * sending even from a big-endian host (htonl does nothing generally in
                           * this case).
                           */
        if (send(sock, (void*)&num, sizeof(num), 0) < (int)sizeof(num)) {
            /* Send 4-byte integer to network in the network byte order */
            /* Handle error */
            fatal_error();
        }
        return 0;
    } else {
        if (recv(sock, (void*)&num, sizeof(num), 0) < (int)sizeof(num)) {
            /* Read 4-byte integer from network in network byte order */
            /* Handle error */
            fatal_error();
        }
        num = ntohl(num); /* Fix: call ntohl to re-order 32-bit integer in the local byte order.
                           * For portability, this is a good practice to always use ntohl after
                           * received network data even for a big-endian host (ntohl does nothing
                           * generally in this case).
                           */
        if (num > 255) {  /* No defect */
            return 255;
        } else {
            return num;
        }
    }
}
#endif /* __GNUC__ */


/*============================================================================
 *  INVALID FILE POS
 *==========================================================================*/
FILE* bug_invalidfilepos(FILE* file) {
    fpos_t offset;
    if (file == NULL) {
        /* Handle error */
        fatal_error();
    }
    /* Save the beginning position */
    (void)memset(&offset, 0, sizeof(offset)); /* Zeroing offset */

    /* Read data from file */

    /* Back to the initial position */
    if (fsetpos(file, &offset) != 0) {
        /* Defect: use zeroed offset that is not
         * returned from fgetpos */
        /* Handle error */
        fatal_error();
    }
    return file;
}

FILE* corrected_invalidfilepos(FILE* file) {
    fpos_t offset;
    if (file == NULL) {
        /* Handle error */
        fatal_error();
    }
    /* Save the beginning position */
    if (fgetpos(file, &offset) != 0) { /* Fix: use fgetpos to save the initial position */
        /* Handle error */
        fatal_error();
    }

    /* Read data from file */

    /* Back to the initial position */
    if (fsetpos(file, &offset) != 0) { /* No defect: use offset returned from fgetpos */
        /* Handle error */
        fatal_error();
    }
    return file;
}


/*============================================================================
 *  INDETERMINATE STRING
 *==========================================================================*/
extern void display_text(const char* txt);

void bug_indeterminatestring(void) {
    char buf[SIZE20];
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        /* 'buf' may contain an indeterminate string after the failure of fgets  */
        ;
    }
    display_text(buf); /* Defect: an indeterminate string passed to an external function that
                          expects a correct string */
}

void corrected_indeterminatestring(void) {
    char buf[SIZE20];
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        buf[0] = '\0'; /* Fix: reset 'buf' after the failure of fgets */
    }
    display_text(buf); /* No defect */
}


/*============================================================================
 *  SIDE EFFECT IGNORED
 *==========================================================================*/
void bug_sideeffectignored(void) {
    unsigned int a = 1U;
    unsigned int b = (unsigned int)sizeof(a++); /* Defect: sizeof operator does not evaluate
                                                 * the operand beyond the provided type
                                                 * information. Side-effects, for instance
                                                 * a++, do not take place.
                                                 */
    printf("%u, %u\n", a, b);
}

void corrected_sideeffectignored(void) {
    unsigned int a = 1U;
    unsigned int b = (unsigned int)sizeof(a); /* Fix: separate sizeof operator and
                                               * side-effects in its operand.
                                               */
    a++;                                      /* Side effect (a++) takes place. */
    printf("%u, %u\n", a, b);
}


/*============================================================================
 *  PREPROCESSOR DIRECTIVE IN MACRO ARGUMENT
 *==========================================================================*/
#define print(A) printf(#A)

void bug_predirectivemacroarg(void) {
    print(
#ifdef SW /* Defect: the presence of processor directives   \
           * in the arguments to a macro invocation causes  \
           * undefined behavior. This call may produce      \
           * #ifdef SW "Message 1" #else "Message 2" #endif \
           * or simply "Message 2".                         \
           */
        "Message 1"
#else  /* SW */
        "Message 2"
#endif /* SW */
    );
}

void corrected_predirectivemacroarg(void) {
#ifdef SW /* Fix: use processor directives to control \
           * calls to print.                          \
           */
    print("Message 1");
#else  /* SW */
    print("Message 2");
#endif /* SW */
}


/*============================================================================
 *  INLINE CONSTRAINT NOT RESPECTED
 *==========================================================================*/
#ifdef INCLUDE_C99_EXAMPLES /* Inline functions in C99 only */
static int I = 12; /* Static variable 'I' at the file scope has the internal linkage */

inline int bug_inlineconstraintnotrespected(int a) {
    int b = a * I; /* Defect: external linkage inline function uses internal linkage variable 'I' */
    return b;
}

int ext_I = 12; /* Fix: ext_I is defined as external linkage variable */
inline int corrected_inlineconstraintnotrespected(int a) {
    int b = a * ext_I; /* OK: external function uses external linkage variable 'ext_I' */
    return b;
}

volatile int input_K;

void demo_inlineconstraintnotrespected(void) {
    int arg = input_K;
    bug_inlineconstraintnotrespected(arg);
    corrected_inlineconstraintnotrespected(arg);
}
#endif /* INCLUDE_C99_EXAMPLES */


/*============================================================================
 *  INCORRECT DATA TYPE PASSED TO VA_ARG
 *==========================================================================*/
unsigned char bug_vaargincorrecttype(size_t count, ...) {
    va_list ap;
    unsigned char result = 0;
    va_start(ap, count);
    if (count > 0) {
        result = va_arg(ap, unsigned char); /* Defect : 'unsigned char' value
                                             * passed to the variadic function
                                             * 'bug_vaargincorrecttype' undergoes
                                             * the default argument promotion,
                                             * resulting in a 'int' value.
                                             */
    }
    va_end(ap);
    return result;
}

unsigned char corrected_vaargincorrecttype(size_t count, ...) {
    va_list ap;
    unsigned char result = 0;
    va_start(ap, count);
    if (count > 0) {
        result = (unsigned char)va_arg(ap, int); /* Fix : call va_arg with 'int' as type argument
                                                  * and cast the return value to 'unsigned char'.
                                                  */
    }
    va_end(ap);
    return result;
}

void demo_vaargincorrecttype(void) {
    unsigned char c = 0x12;
    (void)bug_vaargincorrecttype(
        1, c); /* 'c' undergoes the default argument promotion, resulting in 'int' */
    (void)corrected_vaargincorrecttype(
        1, c); /* 'c' undergoes the default argument promotion, resulting in 'int' */
}


/*============================================================================
 *  TOO MANY VA_ARG CALLS
 *==========================================================================*/
int bug_toomanyvaargcalls(int count, ...) {
    int result = -1;
    va_list ap;
    va_start(ap, count);
    if (count > 0) {
        result = va_arg(ap, int);
        count--;
        if (count > 0) {
            result += va_arg(ap, int); /* Defect: only one argument is passed through va_list of
                                        * bug_toomanyvaargcalls. Additional call to va_arg may
                                        * corrupt data, cause crash or unexpected result.
                                        */
        }
    }
    va_end(ap);
    return result;
}

int corrected_toomanyvaargcalls(int count, ...) {
    int result = -1;
    va_list ap;
    va_start(ap, count);
    if (count > 0) {
        result = va_arg(ap, int);
        count--;
        if (count > 0) {
            result += va_arg(ap, int); /* No defect: two arguments are passed through va_list of
                                          corrected_toomanyvaargcalls. */
        }
    }
    va_end(ap);
    return result;
}

void demo_toomanyvaargcalls(void) {
    (void)bug_toomanyvaargcalls(
        2, 100); /* Defect origin: number of argument set to 2 with only one integer value. */
    (void)corrected_toomanyvaargcalls(2, 100, 200); /* Fix: two integer values are passed. */
}


/*==========================================================================
 *  TEMPORARY LIFETIME OBJECT ACCESS
 *==========================================================================*/
struct SwithArray demo_func_return_temp_object(void);

int bug_tempobjectaccess(void) {
    return ++(demo_func_return_temp_object()
                  .a[0]); /* Defect: demo_func_return_temp_object() returns a struct value
                           * containing an array with a temporary lifetime, writing this temporary
                           * lifetime object results in undefined behavior.
                           */
}

int corrected_tempobjectaccess(void) {
    struct SwithArray s =
        demo_func_return_temp_object(); /* Fix: assign to a local variable 's' the return value of
                                         * demo_func_return_temp_object(), the array content of 's'
                                         * is a copy of the return value, can be used safely for
                                         * writing.
                                         */
    ++(s.a[0]); /* No defect with well-defined behavior : s.a[0] is incremented. */
    return s.a[0];
}

void demo_tempobjectaccess(void) {
    (void)bug_tempobjectaccess();
    (void)corrected_tempobjectaccess();
}


/*==========================================================================
 *  STREAM WITH SIDE EFFECT
 *==========================================================================*/
const char* myfile1 = "my_file1.log";
const char* myfile2 = "my_file2.log";

void bug_streamwithsideeffect(void) {
    int c;
    FILE* fptr;
    c = getc((fptr = fopen(myfile1, "r")) != NULL
                 ? fptr
                 : stdin); /* Defect: call to fopen (and assignment) is side effect. In case
                            * where 'getc' is implemented as unsafe macro whose stream argument
                            * is evaluated multiple times, the side effect takes place multiple
                            * times unintentionally. In case where 'getc' is implemented as a
                            * function, the result of 'fopen' that has not been checked for
                            * errors is passed to 'getc', can cause unexpected result.
                            */
    if (c == EOF) {
        /* Handle error */
        (void)fclose(fptr);
        fatal_error();
    }
    if (fclose(fptr) == EOF) {
        /* Handle error */
        fatal_error();
    }
}

void corrected_streamwithsideeffect(void) {
    int c;
    FILE* fptr;

    fptr = fopen(myfile2, "r"); /* Fix: use a separate statement for 'fopen' and check the result
                                 * for error before call to 'getc'.
                                 */
    if (fptr == NULL) {
        /* Handle error */
        fatal_error();
    }
    c = getc(fptr); /* No defect: the stream argument has no side effect */
    if (c == EOF) {
        /* Handle error */
        (void)fclose(fptr);
        fatal_error();
    }
    if (fclose(fptr) == EOF) {
        /* Handle error */
        fatal_error();
    }
}

void demo_streamwithsideeffect(void) {
    bug_streamwithsideeffect();
    corrected_streamwithsideeffect();
}

/*==========================================================================
 *  MEMCMP FLOAT
 *==========================================================================*/
int bug_memcmpfloat(StructWithFloat* s1, StructWithFloat* s2) {
    return memcmp /* Defect: memory comparison of objects containing floating-point
                   * values. Two floating-point values can be numerically equal with
                   * the distinct binary representations in memory. Using memcmp to
                   * compare floating-point values can cause false-negative equality
                   * and or false-positive inequality.
                   */
        ((const void*)s1, (const void*)s2, sizeof(StructWithFloat));
}

#define ESP 0.00001

int corrected_memcmpfloat(StructWithFloat* s1, StructWithFloat* s2) {
    return ((s1->i == s2->i) &&
            (fabsf(s1->f - s2->f) <= ESP)); /* Fix: specific comparison of floating-point field.
                                             * MISRA discourages use of "==" and "!=" operators on
                                             * floating-point values, and recommends testing the
                                             * difference between two floating-point values for
                                             * a given range.
                                             */
}

void demo_memcmpfloat(void) {
    StructWithFloat s1, s2;
    initialize_struct_with_float(&s1);
    initialize_struct_with_float(&s2);
    (void)bug_memcmpfloat(&s1, &s2);
    (void)corrected_memcmpfloat(&s1, &s2);
}


/*==========================================================================
 *  INVALID ENV POINTER
 *==========================================================================*/
#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */

extern int check_arguments(int argc, char** argv, char** envp);
extern void use_envp(char** envp);

int bug_invalidenvpointer(char** envp) {
    if (setenv("MY_NEW_VAR", "new_value", 1) != 0) { /* setenv may reallocate environment */
        /* Handle error */
        return -1;
    }
    if (envp != NULL) {
        /* Defect: envp is not updated after call to setenv family
         * function, corresponds no more to the current environment,
         * and a environment variable may refer to an incorrect
         * location since relocated.
         */
        use_envp(envp);
    }
    return 0;
}

extern char** environ;
int corrected_invalidenvpointer(void) {
    if (setenv("MY_NEW_VAR", "new_value", 1) != 0) {
        /* Handle error */
        return -1;
    }
    if (environ != NULL) {
        /* Fix : use the global variable 'environ' that is
         * always updated after call to setenv family function.
         */
        use_envp(environ);
    }
    return 0;
}

void demo_invalidenvpointer(int argc, char** argv, char** envp) {
    if (check_arguments(argc, argv, envp)) {
        (void)bug_invalidenvpointer(envp);
        (void)corrected_invalidenvpointer();
    }
}
#endif /* __GNUC__ */
