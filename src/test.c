/**
 * polyspace_test.c
 *
 * A deliberately buggy C file to verify that Polyspace Bug Finder
 * is working correctly. Each section is labelled with the defect
 * category Polyspace should report.
 *
 * Compile (for reference only – do NOT ship this code):
 *   gcc -Wall -Wextra -o polyspace_test polyspace_test.c
 *
 * Expected Polyspace Bug Finder defects
 * ──────────────────────────────────────
 *  1. NULL_DEREFERENCE        – deref_null()
 *  2. BUFFER_OVERFLOW (write) – buffer_overflow()
 *  3. INTEGER_OVERFLOW        – integer_overflow()
 *  4. DIVISION_BY_ZERO        – divide_by_zero()
 *  5. MEMORY_LEAK             – memory_leak()
 *  6. USE_AFTER_FREE          – use_after_free()
 *  7. UNINIT_VAR              – uninitialized_variable()
 *  8. DEAD_CODE               – dead_code()
 *  9. ARRAY_OUT_OF_BOUNDS     – out_of_bounds()
 * 10. RETURN_NOT_CHECKED      – unchecked_return_value()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ── 1. NULL DEREFERENCE ─────────────────────────────────────────────────── */
void deref_null(void)
{
    int *p = NULL;
    *p = 42;            /* BUG: writing through a null pointer */
}

/* ── 2. BUFFER OVERFLOW (WRITE) ──────────────────────────────────────────── */
void buffer_overflow(void)
{
    char buf[8];
    /* strcpy does not check size – copies 20 chars into an 8-byte buffer */
    strcpy(buf, "This string is too long!");  /* BUG: stack buffer overflow */
}

/* ── 3. INTEGER OVERFLOW ──────────────────────────────────────────────────── */
void integer_overflow(void)
{
    int a = INT_MAX;
    int b = a + 1;      /* BUG: signed integer overflow → undefined behaviour */
    printf("b = %d\n", b);
}

/* ── 4. DIVISION BY ZERO ──────────────────────────────────────────────────── */
int divide_by_zero(int x)
{
    int divisor = x - x;   /* always 0 */
    return 100 / divisor;  /* BUG: division by zero */
}

/* ── 5. MEMORY LEAK ───────────────────────────────────────────────────────── */
void memory_leak(void)
{
    int *arr = (int *)malloc(10 * sizeof(int));
    if (arr == NULL) return;

    arr[0] = 1;
    /* BUG: function returns without calling free(arr) */
}

/* ── 6. USE AFTER FREE ────────────────────────────────────────────────────── */
void use_after_free(void)
{
    int *p = (int *)malloc(sizeof(int));
    if (p == NULL) return;

    *p = 99;
    free(p);
    printf("value = %d\n", *p);  /* BUG: reading freed memory */
}

/* ── 7. UNINITIALIZED VARIABLE ────────────────────────────────────────────── */
int uninitialized_variable(void)
{
    int result;           /* never assigned */
    return result;        /* BUG: returning an uninitialized value */
}

/* ── 8. DEAD CODE ─────────────────────────────────────────────────────────── */
int dead_code(int x)
{
    if (x > 0) {
        return 1;
    } else {
        return -1;
    }
    return 0;  /* BUG: unreachable (dead) code – never executed */
}

/* ── 9. ARRAY OUT OF BOUNDS ───────────────────────────────────────────────── */
void out_of_bounds(void)
{
    int arr[5] = {0, 1, 2, 3, 4};
    /* Valid indices: 0-4. Index 5 is one past the end. */
    arr[5] = 99;           /* BUG: write beyond array bounds */
    printf("%d\n", arr[5]); /* BUG: read beyond array bounds */
}

/* ── 10. UNCHECKED RETURN VALUE ───────────────────────────────────────────── */
void unchecked_return_value(void)
{
   /* malloc return value must always be checked before use */
   int *p = (int *)malloc(sizeof(int));
   *p = 7;  /* BUG: p may be NULL if malloc failed – no NULL check */
   free(p);
}

/* ── MAIN ─────────────────────────────────────────────────────────────────── */
int main(void)
{
    printf("Polyspace Bug Finder – test harness\n");
    printf("====================================\n");
    printf("This file contains intentional defects.\n");
    printf("Run Polyspace Bug Finder on this source to verify detection.\n\n");

    /*
     * NOTE: Calling these functions at runtime will likely crash the
     * program. They are present purely so Polyspace has reachable
     * call-paths to analyse statically.
     */
    printf("Functions with seeded defects:\n");
    printf("  deref_null()              – NULL_DEREFERENCE\n");
    printf("  buffer_overflow()         – BUFFER_OVERFLOW\n");
    printf("  integer_overflow()        – INTEGER_OVERFLOW\n");
    printf("  divide_by_zero(5)         – DIVISION_BY_ZERO\n");
    printf("  memory_leak()             – MEMORY_LEAK\n");
    printf("  use_after_free()          – USE_AFTER_FREE\n");
    printf("  uninitialized_variable()  – UNINIT_VAR\n");
    printf("  dead_code(1)              – DEAD_CODE\n");
    printf("  out_of_bounds()           – ARRAY_OUT_OF_BOUNDS\n");
    printf("  unchecked_return_value()  – RETURN_NOT_CHECKED\n");

    return 0;
}