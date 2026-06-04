/**
 *       Copyright 2015-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * This example shows defects caused by tainted data errors.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
#define _XOPEN_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#endif /* __GNUC__ */

#include "globaldefs.h"


/*============================================================================
 *  Sanitizing functions
 *==========================================================================*/
/* Function to sanitize a string */
int sanitize_str(char* s) {
    /* strlen is used here as a kind of firewall for tainted string errors */
    int res = (strlen(s) > 0);
    return res;
}

/* Safer version of atoi */
int sanitize_atoi(const char* sanitized_str, int* result_value) {
    const char* c_str = sanitized_str;
    char* end;
    long sl;

    errno = 0; /* set errno for error check */
    sl = strtol(c_str, &end, 10);
    if (end == c_str) {
        ; /* not a decimal number */
    } else if ('\0' != *end) {
        ; /* extra characters at end of input */
    } else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
        ; /*  out of range of type long */
    } else if (sl > INT_MAX) {
        ; /* greater than INT_MAX */
    } else if (sl < INT_MIN) {
        ; /* less than INT_MIN */
    } else {
        *result_value = (int)sl;
        return 1;
    }
    return 0;
}


/*============================================================================
 *  USING TAINTED DATA AS ARRAY INDEX
 *==========================================================================*/
int bug_taintedarrayindex(int n) {
    return tab[n]; /* Defect: Parameter n is not checked before use */
}

int corrected_taintedarrayindex(int n) {
    if (n >= 0 && n < SIZE100) {
        return tab[n]; /* Fix: Checking n range before use */
    } else {
        return -9999;
    }
}


/*============================================================================
 *  USING TAINTED DATA AS NUMERATOR IN DIVISION
 *==========================================================================*/
int bug_taintedintdivision(int usernum, int userden) {
    int r = usernum / userden; /* Defect: Parameter is not checked before being used as numerator */
    print_int(r);
    return r;
}

int corrected_taintedintdivision(int usernum, int userden) {
    int r = 0;
    if (userden != 0 &&
        !(usernum = INT_MIN && userden == -1)) { /* Fix:  denominator is checked before use */
        /* - 1st test: denominator not null
         * - 2nd test: as INT_MIN is (-INT_MAX-1), (INT_MIN/-1) = (INT_MAX+1) which will overflow */
        r = usernum / userden;
    }
    print_int(r);
    return r;
}


/*============================================================================
 *  USING TAINTED DATA AS DENOMINATOR IN MODULO OPERATION
 *==========================================================================*/
int bug_taintedintmod(int userden) {
    /* Before C99 norm, remainder using negative operands is implementation defined */
    int r = 128 % userden; /* Defect:  Parameter is not checked before being used in modulo */
    print_int(r);
    return r;
}

int corrected_taintedintmod(int userden) {
    int r = 0;
    if (userden > 0) { /* Fix: Parameter is checked to be non-negative */
        r = 128 % userden;
    }
    print_int(r);
    return r;
}


/*============================================================================
 *  USING TAINTED DATA AS LOOP BOUNDARY
 *==========================================================================*/
int bug_taintedloopboundary(int n) {
    int res = 0;
    for (int i = 0; i < n;
         ++i) { /* Defect:  Parameter is not checked before being used as loop boundary */
        res += i;
    }
    return res;
}

int corrected_taintedloopboundary(int n) {
    int res = 0;
    if (n > 0 && n < SIZE128) {
        for (int i = 0; i < n; ++i) { /* Fix: Check parameter range before use */
            res += i;
        }
    }
    return res;
}


/*============================================================================
 *  USING TAINTED DATA WITH SIGN CHANGE
 *==========================================================================*/
void bug_taintedsignchange(int size) {
    char str[SIZE128] = "";
    if (size < SIZE128) {
        memset(str, 'c', size); /* Defect: Size used before non-negative check */
        print_str(str);
    }
}

void corrected_taintedsignchange(int size) {
    char str[SIZE128] = "";
    if (size > 0 && size < SIZE128) {
        memset(str, 'c', size); /* Fix: Check parameter range before use */
        print_str(str);
    }
}


/*============================================================================
 *  USING TAINTED STRING
 *==========================================================================*/
void bug_taintedstring(char* userstr) {
    char str[SIZE128] = "Using ";
    strncat(str, userstr, SIZE100); /* Defect: Parameter not checked before use */
    print_str(str);
}

void corrected_taintedstring(char* userstr) {
    char str[SIZE128] = "Using ";
    if (sanitize_str(userstr)) {
        strncat(str, userstr,
                SIZE100); /* Fix: Check NULL and null char termination before using string */
    }
    print_str(str);
}


/*============================================================================
 *  USING TAINTED STRING FORMAT
 *==========================================================================*/
void bug_taintedstringformat(char* userstr) {
    if (sanitize_str(userstr)) {
        (void)printf(
            userstr); /* Defect: String userstr may contain %. elements, that may lead to crash */
    }
}

void corrected_taintedstringformat(char* userstr) {
    if (sanitize_str(userstr)) {
        (void)printf("%.20s", userstr); /* Fix: String userstr is printed safely as a string */
    }
}


/*============================================================================
 *  USING TAINTED DATA FOR VARIABLE LENGTH ARRAY SIZE
 *==========================================================================*/
#ifndef _MSC_VER /* Visual C++ does not support Variable-length arrays (VLA) */
int bug_taintedvlasize(int size) {
    int tabval[size]; /* Defect: Parameter size not checked before used */
    int res = 0;
    for (int i = 0; i < SIZE10; ++i) {
        tabval[i] = i * i;
        res += tabval[i];
    }
    return res;
}

int corrected_taintedvlasize(int size) {
    int res = 0;
    if (size > SIZE10 && size < SIZE100) {
        int tabval[size]; /* Fix: Parameter size range has been checked before use */
        for (int i = 0; i < SIZE10; ++i) {
            tabval[i] = i * i;
            res += tabval[i];
        }
    }
    return res;
}
#endif /* _MSC_VER */


/*============================================================================
 *  USING TAINTED DATA AS MEMORY ALLOCATION SIZE
 *==========================================================================*/
int* bug_taintedmemoryallocsize(unsigned long size) {
    int* p = (int*)malloc(
        size); /* Defect: Parameter is not checked before being used as allocation size */
    return p;
}

int* corrected_taintedmemoryallocsize(unsigned long size) {
    int* p = NULL;
    size_t checkedsize = size;
    if (checkedsize > 0 && checkedsize < SIZE128) { /* Fix: Check parameter range before use */
        p = (int*)malloc((unsigned int)size);
    }
    return p;
}


/*============================================================================
 *  USING TAINTED DATA AS ENVIRONMENT VARIABLE
 *==========================================================================*/
void bug_taintedenvvariable(char* path) {
    if (sanitize_str(path)) {
        /* Set path in environment */
        (void)putenv(
            path); /* Defect: Path is not checked before being used as environment variable */
    }
}

const char path_path0[] = "/usr/MyApp";
const char path_path1[] = "/usr/local/MyApp";
const char path_path2[] = "/usr/share/local/MyApp";

int corrected_taintedenvvariable(unsigned int idPath) {
    int ret = 999;
    static char env_path[SIZE128] = "";
    strcpy(env_path, "MYAPP_HOME=");
    /* Fix: Select pre-recorded path according to id */
    switch (idPath) {
    case 0:
        strcat(env_path, path_path0);
        break;
    case 1:
        strcat(env_path, path_path1);
        break;
    case 2:
        strcat(env_path, path_path2);
        break;
    default:
        /* Id does not correspond to pre-recorded paths: return error 999 */
        return ret;
    }
    /* Set path in environment */
    ret = putenv(env_path);
    if (ret == -1) {
        fatal_error();
    }
    return ret;
}


/*============================================================================
 *  USING TAINTED DATA AS HOST ID
 *==========================================================================*/
#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
void bug_taintedhostid(long userhid) {
    int r = sethostid(userhid); /* Defect: using a host id without check */
    if (r == -1) {
        fatal_error();
    }
}

extern long called_taintedhostid_sanitize(long);
enum { HI0 = 1, HI1, HI2, HI3 };

void corrected_taintedhostid(int host) {
    /* Fix: Use predefined host identifier */
    long hid = 0;
    switch (host) {
    case HI0:
        hid = 0x7f0100;
        break;
    case HI1:
        hid = 0x7f0101;
        break;
    case HI2:
        hid = 0x7f0102;
        break;
    case HI3:
        hid = 0x7f0103;
        break;
    default:
        /* do nothing */
        break;
    }
    if (hid > 0) {
        int r = sethostid(hid);
        if (r == -1) {
            fatal_error();
        }
    }
}


/*============================================================================
 *  USING TAINTED DATA AS EXTERNAL COMMAND
 *==========================================================================*/
void bug_taintedexternalcmd(char* usercmd) {
    if (sanitize_str(usercmd)) {
        char cmd[SIZE128] = "/usr/bin/cat ";
        strcat(cmd, usercmd);
        (void)system(cmd); /* Defect: Command is used without being checked */
    }
}

enum { CMD0 = 1, CMD1, CMD2 };

void corrected_taintedexternalcmd(int usercmd) {
    /* Fix: Use predefined command */
    char cmd[SIZE128] = "/usr/bin/cat ";
    char arg[SIZE128];
    switch (usercmd) {
    case CMD0:
        strncpy(arg, "sample.c", 8);
        break;
    case CMD1:
        strncpy(arg, "sample.h", 8);
        break;
    case CMD2:
        strncpy(arg, "sample.cpp", 10);
        break;
    default:
        /* do CMD0 */
        strncpy(arg, "sample.c", 8);
    }
    (void)execl(cmd, "cat", arg, 0); /* Fix: Command has been sanitized before being
                                        used, switched system for execl */
}


/*============================================================================
 *  USING TAINTED DATA AS PATH COMMAND
 *==========================================================================*/
void bug_taintedpathcmd() {
    char cmd[SIZE128] = "";
    /* Getting path from environment, considered as tainted data */
    char* userpath = getenv("MYAPP_PATH");
    if (userpath == NULL) {
        return;
    }
    strncpy(cmd, userpath, SIZE100);
    strcat(cmd, "/ls *");
    /* Launching command */
    (void)execl(cmd, "ls", "sample", NULL); /* Defect: Command path is used without being checked */
}

/* Authorized path ids */
enum { PATH0 = 1, PATH1, PATH2 };

void corrected_taintedpathcmd() {
    char cmd[SIZE128] = "";
    /* Getting path from environment, considered as tainted data */
    char* userpathid = getenv("MYAPP_PATH_ID");
    if (userpathid == NULL) {
        return;
    }
    if (sanitize_str(userpathid)) {
        /* Get path id */
        int pathid;
        if (sanitize_atoi(userpathid, &pathid)) {
            /* Fix: Use only pre-registered paths */
            char path[SIZE128] = "";
            switch (pathid) {
            case PATH0:
                strcpy(path, "/usr/local/my_app0");
                break;
            case PATH1:
                strcpy(path, "/usr/local/my_app1");
                break;
            case PATH2:
                strcpy(path, "/usr/local/my_app2");
                break;
            default:
                /* do nothing */
                break;
            }
            if (strlen(path) > 0) {
                strncpy(cmd, path, SIZE100);
                strcat(cmd, "/ls");
                /* Launching command */
                (void)execl(cmd, "ls", "sample", NULL); /* Fix: Only trusted path is used */
            }
        }
    }
}


/*============================================================================
 *  USING TAINTED DATA AS PATH LIB
 *==========================================================================*/
void* bug_taintedpathlib() {
    void* libhandle = NULL;
    char lib[SIZE128] = "";
    char* userpath = getenv("MYAPP_HOME");
    if (userpath == NULL) {
        return NULL;
    }
    strncpy(lib, userpath, SIZE128);
    strcat(lib, "/libMyApp.so");
    libhandle = dlopen(lib, RTLD_LAZY); /* Defect: Library path is used without being checked */
    return libhandle;
}

const char path_path_lib0[] = "/usr/MyApp/lib";
const char path_path_lib1[] = "/usr/local/MyApp/lib";
const char path_path_lib2[] = "/usr/share/local/MyApp/lib";

void* corrected_taintedpathlib(unsigned int idPathLib) {
    void* libhandle = NULL;
    char path_lib[SIZE128] = "";
    /* Fix: Select pre-recorded path according to id */
    switch (idPathLib) {
    case 0:
        strcpy(path_lib, path_path_lib0);
        break;
    case 1:
        strcpy(path_lib, path_path_lib1);
        break;
    case 2:
        strcpy(path_lib, path_path_lib2);
        break;
    default:
        /* Id does not correspond to pre-recorded paths: return NULL */
        return libhandle;
    }
    /* Load library from secure pre-recorded path */
    strcat(path_lib, "/libMyApp.so");
    libhandle = dlopen(path_lib, RTLD_LAZY); /* Fix: Library path has been checked */
    return libhandle;
}
#endif /* __GNUC__ */


/*============================================================================
 *  USING TAINTED OFFSET ON POINTER
 *==========================================================================*/
int bug_taintedptroffset(int i) {
    int* pint = (int*)calloc(SIZE10, sizeof(int));
    int c = 0;
    if (pint) {
        /* Filling array */
        read_pint(pint);
        c = pint[i]; /* Defect: Index is not checked before use */
        free(pint);
    }
    return c;
}

int corrected_taintedptroffset(int i) {
    int* pint = (int*)calloc(SIZE10, sizeof(int));
    int c = 0;
    if (pint) {
        /* Filling array */
        read_pint(pint);
        if (i > 0 && i < SIZE10) { /* Fix: Check the index range */
            c = pint[i];
        }
        free(pint);
    }
    return c;
}
