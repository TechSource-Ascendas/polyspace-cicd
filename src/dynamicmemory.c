/**
 *       Copyright 2012-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * Dynamic memory errors are common C programming mistakes that lead to
 * erratic problems which can be very difficult to find.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */
#define _POSIX_C_SOURCE 200809L /* For posix_memalign */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globaldefs.h"

/*============================================================================
 *  UNCHECKED MALLOC
 *==========================================================================*/
void bug_unprotectedmemoryallocation(void) {
    int* ptr;
    int i = 42;

    ptr = (int*)malloc(sizeof(int)); /* Defect: Unchecked malloc return    */
    *ptr = i;

    free(ptr);
}

void corrected_unprotectedmemoryallocation(void) {
    int* ptr;
    int i = 42;

    ptr = (int*)malloc(sizeof(int));
    if (ptr == NULL) {
        return;
    } /* Fix: Check that return is not NULL       */
    *ptr = i;

    free(ptr);
}


/*============================================================================
 *  BAD FREE
 *==========================================================================*/
void bug_badfree(int i) {
    int* p = &i;

    free(p); /* Defect: free on a non-allocated memory */
}

void corrected_badfree(int i) {
    int* p = (int*)malloc(sizeof(int));
    if (p != NULL) {
        *p = i;
        print_pint(p);

        free(p); /* Fix: free on a allocated memory */
    }
}


/*============================================================================
 *  DOUBLE FREE
 *==========================================================================*/
void bug_doubledeallocation(void) {

    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) {
        return;
    }

    *pi = 2;
    (void)printf("Freeing the pointer\n");
    free(pi); /* good practice violation (missing reset of
               * freed pointer) prior to double free       */
    (void)printf("Freeing the pointer\n");
    free(pi); /* Defect: pi has already been freed         */
}

void corrected_doubledeallocation(void) {

    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) {
        return;
    }

    *pi = 2;
    (void)printf("Freeing the pointer\n");
    free(pi);
} /* Fix: Do not free pi twice                */


/*============================================================================
 *  USING FREED POINTER
 *==========================================================================*/
int bug_freedptr(void) {
    int j;
    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) {
        return 0;
    }

    *pi = 42;
    free(pi); /* good practice violation (missing reset of
               * freed pointer) prior to reading freed
               * pointer                                   */

    j = *pi + 2; /* Defect: Reading a freed pointer           */
    *pi = 0;

    return j;
}

int corrected_freedptr(void) {
    int j;
    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) {
        return 0;
    }

    *pi = 42;

    j = *pi + 2;
    *pi = 0;

    free(pi); /* Fix: The pointer is freed after last use */
    return j;
}


/*============================================================================
 *  MEMORY LEAK
 *==========================================================================*/
void bug_memleak(void) {
    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) {
        return;
    }

    *pi = 42;
} /* Defect: We forgot to free the pointer    */

void corrected_memleak(void) {
    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) {
        return;
    }

    *pi = 42;
    free(pi); /* Fix: Properly free the pointer           */
}


/*============================================================================
 *  MEMORY LEAK ON AN ARRAY
 *==========================================================================*/
extern int extval(void);
void bug_memleak_array(void) {
    int i;
    int j;
    int* pi = (int*)malloc(10 * sizeof(int));
    if (pi == NULL) {
        return;
    }

    for (i = 0; i < SIZE9; i++) {
        pi[i] = 42 + i;
    }

    for (j = 0; j < SIZE9; j++) {
        if (pi[j] == extval()) {
            return;
        }
    } /* Defect: We forgot to free the buffer     */
}

void corrected_memleak_array(void) {
    int i;
    int j;
    int* pi = (int*)malloc(10 * sizeof(int));
    if (pi == NULL) {
        return;
    }

    for (i = 0; i < SIZE9; i++) {
        pi[i] = 42 + i;
    }

    for (j = 0; j < SIZE9; j++) {
        if (pi[j] == extval()) {
            break;
        }
    }
    free(pi); /* Fix: Pointer is properly freed           */
}


/*============================================================================
 * ALIGNMENT CHANGE
 *==========================================================================*/
#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
void bug_alignmentchange(void) {
    size_t resize = SIZE1024;
    size_t alignment = 1 << 12; /* alignment 4096 bytes */
    int* ptr = NULL;
    int* ptr1;

    if (posix_memalign((void**)&ptr, alignment, sizeof(int)) != 0) {
        /* Allocation with 4096 bytes alignment */
        /* Handle error */
        fatal_error();
    }
    ptr1 = (int*)realloc(ptr, sizeof(int) * resize); /* Defect: realloc does not preserve the strict
                                                      * alignment, the new pointer can have an
                                                      * alignment weaker than the old pointer.
                                                      */
    if (ptr1 == NULL) {
        /* Handle error */
        free(ptr);
        ptr = NULL;
        fatal_error();
    }

    /* Processing using ptr1 supposed to be strictly aligned to 4096 bytes */

    /* Free before exit */
    free(ptr1);
}

void corrected_alignmentchange(void) {
    size_t resize = SIZE1024;
    size_t alignment = 1 << 12; /* alignment 4096 */
    int* ptr = NULL;

    if (posix_memalign((void**)&ptr, alignment, sizeof(int)) != 0) {
        /* Allocation with 4096 bytes alignment */
        /* Handle error */
        fatal_error();
    }
    if (posix_memalign((void**)&ptr, alignment, sizeof(int) * resize) != 0) {
        /* Fix: use the same function posix_memalign with
         * the same alignment to resize the previous memory
         */
        /* Handle error */
        free(ptr);
        ptr = NULL;
        fatal_error();
    }

    /* Processing using ptr supposed to be strictly aligned to 4096 bytes */

    /* Free before exit */
    free(ptr);
}
#endif /* __GNUC__ */
