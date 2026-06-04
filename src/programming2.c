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


/*============================================================================
 *  TYPEDEF MISMATCH
 *==========================================================================*/

#ifdef PST_BUG_FINDER /* Avoid link issue in Code Prover mode */
/* See Correction of TYPEDEF_MISMATCH bug in programming.cpp */
typedef unsigned char size_t; /* Defect: size_t has different definition */
void bug_typedefmismatch() {
    size_t a_size_t_var = 0;
}
#endif
