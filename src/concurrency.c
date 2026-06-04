/**
 *       Copyright 2012-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * Some concurrency defects are shown here.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */

#include <assert.h>
#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
#include <pthread.h>
#endif /* __GNUC__ */

#include "globaldefs.h"

/*
 * Notes:
 *
 * For this example to work, all the tasks should be configured as such in
 * Polyspace.
 * The entry-points option should be used to set the tasks synchronicity.
 * For example: to declare bug_datarace_task1 as a task
 *      -entry-points "bug_datarace_task1"
 * This applies to all the *_task functions in this file.
 *
 * We also created independent critical sections for the purpose of these examples.
 * They are defined in the code, after the comment
 *         "Functions defining the beginning and end of critical section"
 * The options -critical-section-begin/-critical-section-end have to be used
 * to configure the critical sections.
 * For example:
 *      -critical-section-begin BEGIN_CRITICAL_SECTION:cs1
 *      -critical-section-end END_CRITICAL_SECTION:cs1
 */


/*============================================================================
 *  DATA RACE
 *==========================================================================*/
int bad_glob1;  /* Defect Data race                                     */
int good_glob1; /* Fix: Use a critical section to protect all accesses  */

/* Functions defining the beginning and end of critical section */
void BEGIN_CRITICAL_SECTION(void);
void END_CRITICAL_SECTION(void);

void bug_datarace_task1(void) {
    bad_glob1 = 1;
}

void bug_datarace_task2(void) {
    int local_var;
    local_var = bad_glob1;
    (void)printf("%d", local_var);
}

void corrected_datarace_task1(void) {
    BEGIN_CRITICAL_SECTION();
    good_glob1 = 1;
    END_CRITICAL_SECTION();
}

void corrected_datarace_task2(void) {
    int local_var;
    BEGIN_CRITICAL_SECTION();
    local_var = good_glob1;
    END_CRITICAL_SECTION();
    (void)printf("%d", local_var);
}


/*============================================================================
 *  NON-ATOMIC DATA RACE
 *==========================================================================*/
void use_longlong(long long entry);
long long bad_glob2; /* Defect: Data race                        */

void bug_datarace_task3(void) {
    bad_glob2 += 1;
}

void bug_datarace_task4(void) {
    long long local_var;
    local_var = bad_glob2;
    use_longlong(local_var);
}


long long good_glob2;

void corrected_datarace_task3(void) {
    BEGIN_CRITICAL_SECTION(); /* Fix: Protect with a critical section     */
    good_glob2 += 1;
    END_CRITICAL_SECTION();
}

void corrected_datarace_task4(void) {
    long long local_var;
    BEGIN_CRITICAL_SECTION();
    local_var = good_glob2;
    END_CRITICAL_SECTION();
    use_longlong(local_var);
}


/*============================================================================
 *  DEADLOCK
 *==========================================================================*/
/* Functions defining the beginning and end of critical section */
void acquire_sensor(void);
void release_sensor(void);
void acquire_printer(void);
void release_printer(void);
int global_var1;

void bug_deadlock_task1(void) {
    acquire_sensor();
    acquire_printer();
    global_var1 += 1;
    release_printer();
    release_sensor();
}

void bug_deadlock_task2(void) {
    acquire_printer();
    acquire_sensor(); /* Defect: Deadlock                             */
    global_var1 += 1;
    release_sensor();
    release_printer();
}

/*
 * Following declarations are "cloned" so that both "bug" and "corrected"
 * versions of the code can be depicted in a single example.
 */
/* Functions defining the beginning and end of critical section */
void acquire_sensor2(void);
void release_sensor2(void);
void acquire_printer2(void);
void release_printer2(void);
int global_var2;

void corrected_deadlock_task1(void) {
    acquire_sensor2();
    acquire_printer2();
    global_var2 += 1;
    release_printer2();
    release_sensor2();
}

void corrected_deadlock_task2(void) {
    acquire_sensor2(); /* Fix: Respect critical sections call order    */
    acquire_printer2();
    global_var2 += 1;
    release_printer2();
    release_sensor2();
}


/*============================================================================
 *  DOUBLE LOCK
 *==========================================================================*/
int global_var_dlock;
void bug_doublelock_task(void) {
    BEGIN_CRITICAL_SECTION();
    global_var_dlock += 1;
    BEGIN_CRITICAL_SECTION(); /* Defect: Double lock                      */
    global_var_dlock += 1;
    END_CRITICAL_SECTION();
}

void corrected_doublelock_task(void) {
    BEGIN_CRITICAL_SECTION(); /* Fix: A single lock is enough             */
    global_var_dlock += 1;
    global_var_dlock += 1;
    END_CRITICAL_SECTION();
}


/*============================================================================
 *  DOUBLE UNLOCK
 *==========================================================================*/
int global_var_dunlock;
void bug_doubleunlock_task(void) {
    BEGIN_CRITICAL_SECTION();
    global_var_dunlock += 1;
    END_CRITICAL_SECTION();
    global_var_dunlock += 1;
    END_CRITICAL_SECTION(); /* Defect: Double unlock                      */
}

int global_var_dunlock1;
void corrected_doubleunlock_task(void) {
    BEGIN_CRITICAL_SECTION();
    global_var_dunlock1 += 1;
    global_var_dunlock1 += 1;
    END_CRITICAL_SECTION(); /* Fix: A single unlock is enough             */
}


/*============================================================================
 *  MISSING UNLOCK
 *==========================================================================*/
int global_var_badlock;
void bug_badlock_task(void) {
    BEGIN_CRITICAL_SECTION(); /* Defect: Missing unlock                   */
    global_var_badlock += 1;
}

void corrected_badlock_task(void) {
    BEGIN_CRITICAL_SECTION();
    global_var_badlock += 1;
    END_CRITICAL_SECTION(); /* Fix: Properly close the critical section */
}


/*============================================================================
 *  MISSING LOCK
 *==========================================================================*/
int global_var_badunlock;
void bug_badunlock_task(void) {
    global_var_badunlock += 1;
    END_CRITICAL_SECTION(); /* Defect: Missing lock                   */
}

int global_var_badunlock1;
void corrected_badunlock_task(void) {
    BEGIN_CRITICAL_SECTION(); /* Fix: Properly open the critical section */
    global_var_badunlock1 += 1;
    END_CRITICAL_SECTION();
}


/*============================================================================
 *  DATA RACE ON STANDARD LIBRARY FUNCTIONS
 *==========================================================================*/
void bug_dataracestdlib_task1(void) {
    (void)setlocale(LC_ALL,
                    "en_US"); /* Defect: data race on thread unsafe standard library function */
}

void bug_dataracestdlib_task2(void) {
    (void)setlocale(LC_ALL, "fr_FR");
}

void corrected_dataracestdlib_task1(void) {
    BEGIN_CRITICAL_SECTION(); /* Fix: Protect with a critical section */
    (void)setlocale(LC_ALL, "en_US");
    END_CRITICAL_SECTION();
}

void corrected_dataracestdlib_task2(void) {
    BEGIN_CRITICAL_SECTION();
    (void)setlocale(LC_ALL, "fr_FR");
    END_CRITICAL_SECTION();
}


/*============================================================================
 *  DESTRUCTION OF A LOCKED MUTEX
 *==========================================================================*/
#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */

pthread_mutex_t m_bug;
pthread_mutex_t m_corrected;

void bug_destroylocked_task(void) {
    int r;
    r = pthread_mutex_init(&m_bug, NULL);
    if (r == -1) {
        fatal_error();
    }
    r = pthread_mutex_lock(&m_bug);
    if (r == -1) {
        fatal_error();
    }
    r = pthread_mutex_destroy(&m_bug); /* Defect: Destruction of a locked mutex */
    if (r == -1) {
        fatal_error();
    }
    r = pthread_mutex_unlock(&m_bug);
    if (r == -1) {
        fatal_error();
    }
}

void corrected_destroylocked_task(void) {
    int r;
    r = pthread_mutex_init(&m_corrected, NULL);
    if (r == -1) {
        fatal_error();
    }
    r = pthread_mutex_lock(&m_corrected);
    if (r == -1) {
        fatal_error();
    }
    r = pthread_mutex_unlock(&m_corrected);
    if (r == -1) {
        fatal_error();
    }
    r = pthread_mutex_destroy(&m_corrected); /* Fix: Destroy only after unlock */
    if (r == -1) {
        fatal_error();
    }
}
#endif /* __GNUC__ */
