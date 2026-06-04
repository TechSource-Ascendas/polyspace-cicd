/**
 *       Copyright 2012-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * This example calls all functions of the example.
 * It has no functional effect or meaning, and is only useful for the purpose
 * of setting the example environment.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bf_testcases.h"
#include "globaldefs.h"
#include "lib_crypto_checkers.h"

extern void unchecked_assert(int expression);


/*============================================================================
 *  RANDOM VARIABLES
 *==========================================================================*/
volatile char pst_random_char = 'c';
volatile char pst_random_unsigned_char = 'c';
volatile int pst_random_int = 1;
volatile long pst_random_long = 1;
volatile unsigned long pst_random_unsigned_long = 1;
volatile unsigned int pst_random_unsigned_int = 1;
volatile double pst_random_double = 1.0;
volatile float pst_random_float = 1.0f;
volatile size_t pst_random_size_t = 1;


/*============================================================================
 *  RANDOM POINTERS INITIALIZATION
 *==========================================================================*/
extern S_Padding* random_s_padding_pointer(void);
extern FILE* random_file_pointer(void);
extern char* random_char_pointer(void);
extern int* random_int_pointer(void);
extern unsigned char* random_unsigned_char_pointer(void);
extern EVP_CIPHER_CTX* random_evp_cipher_ctx_pointer(void);
extern EVP_PKEY_CTX* random_evp_pkey_ctx_pointer(void);
extern EVP_PKEY* random_evp_pkey_pointer(void);
extern RSA* random_rsa_pointer(void);
extern DSA* random_dsa_pointer(void);


/*============================================================================
 *  TAINTED POINTERS INITIALIZATION
 *==========================================================================*/
#ifdef PST_BUG_FINDER
static int tainted_int(void) {
    return strtol(getenv("RAND"), NULL, 10);
}
static int tainted_unsigned_int(void) {
    return rand();
}
static long tainted_long(void) {
    return strtol(getenv("RAND"), NULL, 10);
}
static unsigned long tainted_unsigned_long(void) {
    return rand();
}
static char* tainted_string(void) {
    static char buffer[128];
    char* result = fgets(buffer, sizeof(buffer), stdin);
    return result ? result : NULL;
}
static char* tainted_char_pointer(void) {
    static char buffer[40];
    fread(buffer, 1, sizeof(buffer), stdin);
    return buffer;
}
#endif


/*============================================================================
 *  MAIN
 *==========================================================================*/
#ifndef PSTEST_BUILD
#ifndef PST_BUG_FINDER

int main(int argc, char* argv[], char* envp[]) {
    switch (pst_random_int) {
    case BUG_UNCALLEDFUNC: {
        extern void bug_uncalledfunc(void);
        bug_uncalledfunc();
        break;
    }
    case BUG_USELESSWRITE: {
        extern void bug_uselesswrite(void);
        bug_uselesswrite();
        break;
    }
    case CORRECTED_USELESSWRITE: {
        extern void corrected_uselesswrite(void);
        corrected_uselesswrite();
        break;
    }
    case BUG_USELESSWRITE_DOUBLEWRITE: {
        extern int bug_uselesswrite_doublewrite(void);
        int ret;
        ret = bug_uselesswrite_doublewrite();
        break;
    }
    case CORRECTED_USELESSWRITE_DOUBLEWRITE: {
        extern int corrected_uselesswrite_doublewrite(void);
        int ret;
        ret = corrected_uselesswrite_doublewrite();
        break;
    }
    case BUG_NONINITVAR: {
        extern int bug_noninitvar(void);
        int ret;
        ret = bug_noninitvar();
        break;
    }
    case CORRECTED_NONINITVAR: {
        extern int corrected_noninitvar(void);
        int ret;
        ret = corrected_noninitvar();
        break;
    }
    case BUG_NONINITVAR1: {
        extern int bug_noninitvar1(void);
        int ret;
        ret = bug_noninitvar1();
        break;
    }
    case CORRECTED_NONINITVAR1: {
        extern int corrected_noninitvar1(void);
        int ret;
        ret = corrected_noninitvar1();
        break;
    }
    case BUG_NONINITPTR: {
        extern int* bug_noninitptr(int*);
        int* ret;
        int* arg__0;
        arg__0 = random_int_pointer();
        ret = bug_noninitptr(arg__0);
        break;
    }
    case CALL_NONINITPTR: {
        extern void call_noninitptr(void);
        call_noninitptr();
        break;
    }
    case BUG_USELESSIF: {
        extern void bug_uselessif(void);
        bug_uselessif();
        break;
    }
    case CORRECTED_USELESSIF: {
        extern void corrected_uselessif(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_uselessif(arg__0);
        break;
    }
    case BUG_VARSHADOWING: {
        extern int bug_varshadowing(void);
        int ret;
        ret = bug_varshadowing();
        break;
    }
    case CORRECTED_VARSHADOWING: {
        extern int corrected_varshadowing(void);
        int ret;
        ret = corrected_varshadowing();
        break;
    }
    case BUG_PARTIALLYACCESSEDARRAY: {
        extern void bug_partiallyaccessedarray(void);
        bug_partiallyaccessedarray();
        break;
    }
    case CORRECTED_PARTIALLYACCESSEDARRAY: {
        extern void corrected_partiallyaccessedarray(void);
        corrected_partiallyaccessedarray();
        break;
    }
    case BUG_PARTIALLYACCESSEDARRAY_FIB: {
        extern int bug_partiallyaccessedarray_fib(void);
        int ret;
        ret = bug_partiallyaccessedarray_fib();
        break;
    }
    case CORRECTED_PARTIALLYACCESSEDARRAY_FIB: {
        extern int corrected_partiallyaccessedarray_fib(void);
        int ret;
        ret = corrected_partiallyaccessedarray_fib();
        break;
    }
    case BUG_DEACTIVATEDCODE: {
        extern int bug_deactivatedcode(void);
        int ret;
        ret = bug_deactivatedcode();
        break;
    }
    case CORRECTED_DEACTIVATEDCODE: {
        extern int corrected_deactivatedcode(void);
        int ret;
        ret = corrected_deactivatedcode();
        break;
    }
    case BUG_DEADCODE: {
        extern void bug_deadcode(void);
        bug_deadcode();
        break;
    }
    case CORRECTED_DEADCODE: {
        extern void corrected_deadcode(void);
        corrected_deadcode();
        break;
    }
    case BUG_MISSINGRETURN: {
        extern int bug_missingreturn(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_missingreturn(arg__0);
        break;
    }
    case CORRECTED_MISSINGRETURN: {
        extern int corrected_missingreturn(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_missingreturn(arg__0);
        break;
    }
    case BUG_NONINITPTRCONV: {
        extern void bug_noninitptrconv(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_noninitptrconv(arg__0);
        break;
    }
    case CORRECTED_NONINITPTRCONV: {
        extern void corrected_noninitptrconv(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_noninitptrconv(arg__0);
        break;
    }
    case BUG_UNREACHABLE: {
        extern void bug_unreachable(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_unreachable(arg__0);
        break;
    }
    case CORRECTED_UNREACHABLE: {
        extern void corrected_unreachable(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_unreachable(arg__0);
        break;
    }
    case CALL_UNCALLEDFUNC: {
        extern void call_uncalledfunc(void);
        call_uncalledfunc();
        break;
    }
    case BUG_UNPROTECTEDMEMORYALLOCATION: {
        extern void bug_unprotectedmemoryallocation(void);
        bug_unprotectedmemoryallocation();
        break;
    }
    case CORRECTED_UNPROTECTEDMEMORYALLOCATION: {
        extern void corrected_unprotectedmemoryallocation(void);
        corrected_unprotectedmemoryallocation();
        break;
    }
    case BUG_BADFREE: {
        extern void bug_badfree(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_badfree(arg__0);
        break;
    }
    case CORRECTED_BADFREE: {
        extern void corrected_badfree(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_badfree(arg__0);
        break;
    }
    case BUG_DOUBLEDEALLOCATION: {
        extern void bug_doubledeallocation(void);
        bug_doubledeallocation();
        break;
    }
    case CORRECTED_DOUBLEDEALLOCATION: {
        extern void corrected_doubledeallocation(void);
        corrected_doubledeallocation();
        break;
    }
    case BUG_FREEDPTR: {
        extern int bug_freedptr(void);
        int ret;
        ret = bug_freedptr();
        break;
    }
    case CORRECTED_FREEDPTR: {
        extern int corrected_freedptr(void);
        int ret;
        ret = corrected_freedptr();
        break;
    }
    case BUG_MEMLEAK: {
        extern void bug_memleak(void);
        bug_memleak();
        break;
    }
    case CORRECTED_MEMLEAK: {
        extern void corrected_memleak(void);
        corrected_memleak();
        break;
    }
    case BUG_MEMLEAK_ARRAY: {
        extern void bug_memleak_array(void);
        bug_memleak_array();
        break;
    }
    case CORRECTED_MEMLEAK_ARRAY: {
        extern void corrected_memleak_array(void);
        corrected_memleak_array();
        break;
    }
    case BUG_PASSBYVALUE: {
        extern void bug_passbyvalue(void);
        bug_passbyvalue();
        break;
    }
    case CORRECTED_PASSBYVALUE: {
        extern void corrected_passbyvalue(void);
        corrected_passbyvalue();
        break;
    }
    case BUG_MORETHANONESTATEMENT: {
        extern int bug_morethanonestatement(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_morethanonestatement(arg__0);
        break;
    }
    case CORRECTED_MORETHANONESTATEMENT: {
        extern int corrected_morethanonestatement(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_morethanonestatement(arg__0);
        break;
    }
    case BUG_HARDCODEDBUFFERSIZE: {
        extern void bug_hardcodedbuffersize(int, int, int);
        int arg__0;
        int arg__1;
        int arg__2;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        arg__2 = pst_random_int;
        bug_hardcodedbuffersize(arg__0, arg__1, arg__2);
        break;
    }
    case CORRECTED_HARDCODEDBUFFERSIZE: {
        extern void corrected_hardcodedbuffersize(int, int, int);
        int arg__0;
        int arg__1;
        int arg__2;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        arg__2 = pst_random_int;
        corrected_hardcodedbuffersize(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_HARDCODEDLOOPBOUNDARY: {
        extern int bug_hardcodedloopboundary(void);
        int ret;
        ret = bug_hardcodedloopboundary();
        break;
    }
    case CORRECTED_HARDCODEDLOOPBOUNDARY: {
        extern int corrected_hardcodedloopboundary(void);
        int ret;
        ret = corrected_hardcodedloopboundary();
        break;
    }
    case BUG_UNUSEDPARAMETER: {
        extern void bug_unusedparameter(int, int);
        int arg__0;
        int arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        bug_unusedparameter(arg__0, arg__1);
        break;
    }
    case CORRECTED_UNUSEDPARAMETER: {
        extern void corrected_unusedparameter(int, int);
        int arg__0;
        int arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        corrected_unusedparameter(arg__0, arg__1);
        break;
    }
    case BUG_SETJMPLONGJMPUSE_HANDLER: {
        extern void bug_setjmplongjmpuse_handler(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_setjmplongjmpuse_handler(arg__0);
        break;
    }
    case BUG_SETJMPLONGJMPUSE_MAIN: {
        extern void bug_setjmplongjmpuse_main(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_setjmplongjmpuse_main(arg__0);
        break;
    }
    case CORRECTED_SETJMPLONGJMPUSE_HANDLER: {
        extern void corrected_setjmplongjmpuse_handler(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_setjmplongjmpuse_handler(arg__0);
        break;
    }
    case CORRECTED_SETJMPLONGJMPUSE_MAIN: {
        extern void corrected_setjmplongjmpuse_main(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_setjmplongjmpuse_main(arg__0);
        break;
    }
    case BUG_BITWISEARITHMIX: {
        extern unsigned int bug_bitwisearithmix(void);
        unsigned int ret;
        ret = bug_bitwisearithmix();
        break;
    }
    case CORRECTED_BITWISEARITHMIX: {
        extern unsigned int corrected_bitwisearithmix(void);
        unsigned int ret;
        ret = corrected_bitwisearithmix();
        break;
    }
    case BUG_MISSINGFREEDPTRRESET: {
        extern void bug_missingfreedptrreset(void);
        bug_missingfreedptrreset();
        break;
    }
    case CORRECTED_MISSINGFREEDPTRRESET: {
        extern void corrected_missingfreedptrreset(void);
        corrected_missingfreedptrreset();
        break;
    }
    case BUG_MISSINGSWITCHBREAK: {
        extern void bug_missingswitchbreak(enum WidgetEnum);
        enum WidgetEnum arg__0;
        volatile enum WidgetEnum random_enum = 1;
        arg__0 = random_enum;
        unchecked_assert(arg__0 <= 3);
        bug_missingswitchbreak(arg__0);
        break;
    }
    case CORRECTED_MISSINGSWITCHBREAK: {
        extern void corrected_missingswitchbreak(enum WidgetEnum);
        enum WidgetEnum arg__0;
        volatile enum WidgetEnum random_enum = 1;
        arg__0 = random_enum;
        unchecked_assert(arg__0 <= 3);
        corrected_missingswitchbreak(arg__0);
        break;
    }
    case BUG_HARDCODEDMEMSIZE: {
        extern void bug_hardcodedmemsize(void);
        bug_hardcodedmemsize();
        break;
    }
    case CORRECTED_HARDCODEDMEMSIZE: {
        extern void corrected_hardcodedmemsize(void);
        corrected_hardcodedmemsize();
        break;
    }
    case CALL_INTZERODIV: {
        extern void call_intzerodiv(void);
        call_intzerodiv();
        break;
    }
    case CALL_FLOATZERODIV: {
        extern void call_floatzerodiv(void);
        call_floatzerodiv();
        break;
    }
    case BUG_INTCONVOVFL: {
        extern short bug_intconvovfl(int);
        short ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_intconvovfl(arg__0);
        break;
    }
    case CORRECTED_INTCONVOVFL: {
        extern short corrected_intconvovfl(int);
        short ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_intconvovfl(arg__0);
        break;
    }
    case BUG_UINTCONVOVFL: {
        extern unsigned short bug_uintconvovfl(int);
        unsigned short ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_uintconvovfl(arg__0);
        break;
    }
    case CORRECTED_UINTCONVOVFL: {
        extern unsigned short corrected_uintconvovfl(int);
        unsigned short ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_uintconvovfl(arg__0);
        break;
    }
    case BUG_SIGNCHANGE: {
        extern int bug_signchange(unsigned int);
        int ret;
        unsigned int arg__0;
        arg__0 = pst_random_unsigned_int;
        ret = bug_signchange(arg__0);
        break;
    }
    case CORRECTED_SIGNCHANGE: {
        extern int corrected_signchange(unsigned int);
        int ret;
        unsigned int arg__0;
        arg__0 = pst_random_unsigned_int;
        ret = corrected_signchange(arg__0);
        break;
    }
    case BUG_FLOATCONVOVFL: {
        extern void bug_floatconvovfl(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_floatconvovfl(arg__0);
        break;
    }
    case CORRECTED_FLOATCONVOVFL: {
        extern void corrected_floatconvovfl(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_floatconvovfl(arg__0);
        break;
    }
    case BUG_INTOVFL: {
        extern int bug_intovfl(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_intovfl(arg__0);
        break;
    }
    case CORRECTED_INTOVFL: {
        extern long corrected_intovfl(int);
        long ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_intovfl(arg__0);
        break;
    }
    case BUG_UINTOVFL: {
        extern unsigned int bug_uintovfl(void);
        unsigned int ret;
        ret = bug_uintovfl();
        break;
    }
    case CORRECTED_UINTOVFL: {
        extern unsigned int corrected_uintovfl(void);
        unsigned int ret;
        ret = corrected_uintovfl();
        break;
    }
    case BUG_FLOATOVFL: {
        extern float bug_floatovfl(void);
        float ret;
        ret = bug_floatovfl();
        break;
    }
    case CORRECTED_FLOATOVFL: {
        extern float corrected_floatovfl(void);
        float ret;
        ret = corrected_floatovfl();
        break;
    }
    case BUG_FLOATABSORPTION: {
        extern double bug_floatabsorption(double, double);
        double ret;
        double arg__0;
        double arg__1;
        arg__0 = pst_random_double;
        arg__1 = pst_random_double;
        ret = bug_floatabsorption(arg__0, arg__1);
        break;
    }
    case CORRECTED_FLOATABSORPTION: {
        extern double corrected_floatabsorption(double, double);
        double ret;
        double arg__0;
        double arg__1;
        arg__0 = pst_random_double;
        arg__1 = pst_random_double;
        ret = corrected_floatabsorption(arg__0, arg__1);
        break;
    }
    case BUG_INTSTDLIB: {
        extern div_t bug_intstdlib(int, int);
        div_t ret;
        int arg__0;
        int arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        ret = bug_intstdlib(arg__0, arg__1);
        break;
    }
    case CORRECTED_INTSTDLIB: {
        extern div_t corrected_intstdlib(int, int);
        div_t ret;
        int arg__0;
        int arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        ret = corrected_intstdlib(arg__0, arg__1);
        break;
    }
    case BUG_FLOATSTDLIB: {
        extern double bug_floatstdlib(int);
        double ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_floatstdlib(arg__0);
        break;
    }
    case CORRECTED_FLOATSTDLIB: {
        extern double corrected_floatstdlib(int);
        double ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_floatstdlib(arg__0);
        break;
    }
    case BUG_SHIFTNEG: {
        extern int bug_shiftneg(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_shiftneg(arg__0);
        break;
    }
    case CORRECTED_SHIFTNEG: {
        extern int corrected_shiftneg(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_shiftneg(arg__0);
        break;
    }
    case BUG_SHIFTOVFL: {
        extern unsigned short bug_shiftovfl(void);
        unsigned short ret;
        ret = bug_shiftovfl();
        break;
    }
    case CORRECTED_SHIFTOVFL: {
        extern unsigned short corrected_shiftovfl(void);
        unsigned short ret;
        ret = corrected_shiftovfl();
        break;
    }
    case BUG_BADPLAINCHARUSE: {
        extern void bug_badplaincharuse(void);
        bug_badplaincharuse();
        break;
    }
    case CORRECTED_BADPLAINCHARUSE: {
        extern void corrected_badplaincharuse(void);
        corrected_badplaincharuse();
        break;
    }
    case BUG_BITWISENEG: {
        extern void bug_bitwiseneg(void);
        bug_bitwiseneg();
        break;
    }
    case CORRECTED_BITWISENEG: {
        extern void corrected_bitwiseneg(void);
        corrected_bitwiseneg();
        break;
    }
    case BUG_ASSERT: {
        extern int bug_assert(void);
        int ret;
        ret = bug_assert();
        break;
    }
    case CORRECTED_ASSERT: {
        extern int corrected_assert(void);
        int ret;
        ret = corrected_assert();
        break;
    }
    case BUG_BADEQUALUSE: {
        extern void bug_badequaluse(int, int);
        int arg__0;
        int arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        bug_badequaluse(arg__0, arg__1);
        break;
    }
    case CORRECTED_BADEQUALUSE: {
        extern void corrected_badequaluse(int, int);
        int arg__0;
        int arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        corrected_badequaluse(arg__0, arg__1);
        break;
    }
    case BUG_BADEQUALUSE_SUBEXP: {
        extern void bug_badequaluse_subexp(int, int, int, int);
        int arg__0;
        int arg__1;
        int arg__2;
        int arg__3;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        arg__2 = pst_random_int;
        arg__3 = pst_random_int;
        bug_badequaluse_subexp(arg__0, arg__1, arg__2, arg__3);
        break;
    }
    case CORRECTED_BADEQUALUSE_SUBEXP: {
        extern void corrected_badequaluse_subexp(int, int, int, int);
        int arg__0;
        int arg__1;
        int arg__2;
        int arg__3;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        arg__2 = pst_random_int;
        arg__3 = pst_random_int;
        corrected_badequaluse_subexp(arg__0, arg__1, arg__2, arg__3);
        break;
    }
    case BUG_BADEQUALEQUALUSE: {
        extern void bug_badequalequaluse(void);
        bug_badequalequaluse();
        break;
    }
    case CORRECTED_BADEQUALEQUALUSE: {
        extern void corrected_badequalequaluse(void);
        corrected_badequalequaluse();
        break;
    }
    case BUG_PTRSIZEOFMISMATCH: {
        extern void bug_ptrsizeofmismatch(void);
        bug_ptrsizeofmismatch();
        break;
    }
    case CORRECTED_PTRSIZEOFMISMATCH: {
        extern void corrected_ptrsizeofmismatch(void);
        corrected_ptrsizeofmismatch();
        break;
    }
    case BUG_MISSINGNULLCHAR: {
        extern const signed char* bug_missingnullchar(int);
        const signed char* ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_missingnullchar(arg__0);
        break;
    }
    case CORRECTED_MISSINGNULLCHAR: {
        extern const signed char* corrected_missingnullchar(int);
        const signed char* ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_missingnullchar(arg__0);
        break;
    }
    case BUG_QUALIFIERMISMATCH: {
        extern float bug_qualifiermismatch(void);
        float ret;
        ret = bug_qualifiermismatch();
        break;
    }
    case CORRECTED_QUALIFIERMISMATCH: {
        extern float corrected_qualifiermismatch(void);
        float ret;
        ret = corrected_qualifiermismatch();
        break;
    }
    case BUG_BADFLOATOP: {
        extern void bug_badfloatop(void);
        bug_badfloatop();
        break;
    }
    case CORRECTED_BADFLOATOP: {
        extern void corrected_badfloatop(void);
        corrected_badfloatop();
        break;
    }
    case BUG_STRINGFORMAT: {
        extern void bug_stringformat(void);
        bug_stringformat();
        break;
    }
    case CORRECTED_STRINGFORMAT: {
        extern void corrected_stringformat(void);
        corrected_stringformat();
        break;
    }
    case BUG_OTHERSTDLIB: {
        extern void bug_otherstdlib(FILE*);
        FILE* arg__0;
        arg__0 = random_file_pointer();
        bug_otherstdlib(arg__0);
        break;
    }
    case CORRECTED_OTHERSTDLIB: {
        extern void corrected_otherstdlib(FILE*);
        FILE* arg__0;
        arg__0 = random_file_pointer();
        corrected_otherstdlib(arg__0);
        break;
    }
    case BUG_DECLMISMATCH: {
        extern void bug_declmismatch(void);
        bug_declmismatch();
        break;
    }
    case CORRECTED_DECLMISMATCH: {
        extern void corrected_declmismatch(void);
        corrected_declmismatch();
        break;
    }
    case CORRECTED_TYPEDEFMISMATCH: {
        extern void corrected_typedefmismatch(void);
        corrected_typedefmismatch();
        break;
    }
    case BUG_BADFILEACCESSMODESTATUS: {
        extern FILE* bug_badfileaccessmodestatus(void);
        FILE* ret;
        ret = bug_badfileaccessmodestatus();
        break;
    }
    case CORRECTED_BADFILEACCESSMODESTATUS: {
        extern FILE* corrected_badfileaccessmodestatus(void);
        FILE* ret;
        ret = corrected_badfileaccessmodestatus();
        break;
    }
    case BUG_BADPTRSCALING: {
        extern int bug_badptrscaling(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_badptrscaling(arg__0);
        break;
    }
    case CORRECTED_BADPTRSCALING: {
        extern int corrected_badptrscaling(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_badptrscaling(arg__0);
        break;
    }
    case BUG_CONSTANTOBJECTWRITE: {
        extern void bug_constantobjectwrite(void);
        bug_constantobjectwrite();
        break;
    }
    case CORRECTED_CONSTANTOBJECTWRITE: {
        extern void corrected_constantobjectwrite(void);
        corrected_constantobjectwrite();
        break;
    }
    case BUG_IMPROPERARRAYINIT: {
        extern void bug_improperarrayinit(void);
        bug_improperarrayinit();
        break;
    }
    case CORRECTED_IMPROPERARRAYINIT: {
        extern void corrected_improperarrayinit(void);
        corrected_improperarrayinit();
        break;
    }
    case BUG_INVALIDMEMORYASSUMPTION: {
        extern void bug_invalidmemoryassumption(void);
        bug_invalidmemoryassumption();
        break;
    }
    case CORRECTED_INVALIDMEMORYASSUMPTION: {
        extern void corrected_invalidmemoryassumption(void);
        corrected_invalidmemoryassumption();
        break;
    }
    case CALL_INVALIDVALISTARG: {
        extern void call_invalidvalistarg(void);
        call_invalidvalistarg();
        break;
    }
    case BUG_MEMSETINVALIDSIZE: {
        extern void bug_memsetinvalidsize(unsigned int);
        unsigned int arg__0;
        arg__0 = pst_random_unsigned_int;
        bug_memsetinvalidsize(arg__0);
        break;
    }
    case CORRECTED_MEMSETINVALIDSIZE: {
        extern void corrected_memsetinvalidsize(unsigned int);
        unsigned int arg__0;
        arg__0 = pst_random_unsigned_int;
        corrected_memsetinvalidsize(arg__0);
        break;
    }
    case CALL_NONPOSITIVEVLASIZE: {
        extern void call_nonpositivevlasize(void);
        call_nonpositivevlasize();
        break;
    }
    case BUG_OPERATORPRECEDENCE: {
        extern int bug_operatorprecedence(int, int, int);
        int ret;
        int arg__0;
        int arg__1;
        int arg__2;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        arg__2 = pst_random_int;
        ret = bug_operatorprecedence(arg__0, arg__1, arg__2);
        break;
    }
    case CORRECTED_OPERATORPRECEDENCE: {
        extern int corrected_operatorprecedence(int, int, int);
        int ret;
        int arg__0;
        int arg__1;
        int arg__2;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        arg__2 = pst_random_int;
        ret = corrected_operatorprecedence(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_OVERLAPPINGASSIGN: {
        extern int bug_overlappingassign(int, int, char);
        int arg__0;
        int arg__1;
        char arg__2;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        arg__2 = pst_random_char;
        bug_overlappingassign(arg__0, arg__1, arg__2);
        break;
    }
    case CORRECTED_OVERLAPPINGASSIGN: {
        extern int corrected_overlappingassign(int, int, char);
        int arg__0;
        int arg__1;
        char arg__2;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        arg__2 = pst_random_char;
        corrected_overlappingassign(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_OVERLAPPINGCOPY: {
        extern void bug_overlappingcopy(void);
        bug_overlappingcopy();
        break;
    }
    case CORRECTED_OVERLAPPINGCOPY: {
        extern void corrected_overlappingcopy(void);
        corrected_overlappingcopy();
        break;
    }
    case CALL_SIZEOFMISUSE: {
        extern void call_sizeofmisuse(void);
        call_sizeofmisuse();
        break;
    }
    case BUG_STDFUNCARGMISMATCH: {
        extern int bug_stdfuncargmismatch(void);
        int ret;
        ret = bug_stdfuncargmismatch();
        break;
    }
    case CORRECTED_STDFUNCARGMISMATCH: {
        extern int corrected_stdfuncargmismatch(void);
        int ret;
        ret = corrected_stdfuncargmismatch();
        break;
    }
    case BUG_WRITEINTERNALBUFFERRETURNEDFROMSTDFUNC: {
        extern void bug_writeinternalbufferreturnedfromstdfunc(void);
        bug_writeinternalbufferreturnedfromstdfunc();
        break;
    }
    case CORRECTED_WRITEINTERNALBUFFERRETURNEDFROMSTDFUNC: {
        extern void corrected_writeinternalbufferreturnedfromstdfunc(void);
        corrected_writeinternalbufferreturnedfromstdfunc();
        break;
    }
    case BUG_BADINTPTRCAST: {
        extern unsigned int* bug_badintptrcast(void);
        unsigned int* ret;
        ret = bug_badintptrcast();
        break;
    }
    case CORRECTED_BADINTPTRCAST: {
        extern unsigned int* corrected_badintptrcast(void);
        unsigned int* ret;
        ret = corrected_badintptrcast();
        break;
    }
    case BUG_UNSAFESTRTONUMERIC: {
        extern int bug_unsafestrtonumeric(char*);
        int ret;
        char* arg__0;
        arg__0 = random_char_pointer();
        ret = bug_unsafestrtonumeric(arg__0);
        break;
    }
    case CORRECTED_UNSAFESTRTONUMERIC: {
        extern int corrected_unsafestrtonumeric(char*);
        int ret;
        char* arg__0;
        arg__0 = random_char_pointer();
        ret = corrected_unsafestrtonumeric(arg__0);
        break;
    }
    case DEMO_EXIT1: {
        extern void demo_exit1(void);
        demo_exit1();
        break;
    }
    case BUG_EXITABNORMALHANDLER: {
        extern void bug_exitabnormalhandler(void);
        bug_exitabnormalhandler();
        break;
    }
    case DEMO_INSTALL_EXITABNORMALHANDLER: {
        extern int demo_install_exitabnormalhandler(void);
        int ret;
        ret = demo_install_exitabnormalhandler();
        break;
    }
    case CORRECTED_EXITABNORMALHANDLER: {
        extern void corrected_exitabnormalhandler(void);
        corrected_exitabnormalhandler();
        break;
    }
    case DEMO_INSTALL_CORRECTED_EXITABNORMALHANDLER: {
        extern int demo_install_corrected_exitabnormalhandler(void);
        int ret;
        ret = demo_install_corrected_exitabnormalhandler();
        break;
    }
    case BUG_MISSINGERRNORESET: {
        extern double bug_missingerrnoreset(const char*, const char*);
        double ret;
        const char* arg__0;
        const char* arg__1;
        arg__0 = random_char_pointer();
        arg__1 = random_char_pointer();
        ret = bug_missingerrnoreset(arg__0, arg__1);
        break;
    }
    case CORRECTED_MISSINGERRNORESET: {
        extern double corrected_missingerrnoreset(const char*, const char*);
        double ret;
        const char* arg__0;
        const char* arg__1;
        arg__0 = random_char_pointer();
        arg__1 = random_char_pointer();
        ret = corrected_missingerrnoreset(arg__0, arg__1);
        break;
    }
    case BUG_ERRNOMISUSE: {
        extern FILE* bug_errnomisuse(void);
        FILE* ret;
        ret = bug_errnomisuse();
        break;
    }
    case CORRECTED_ERRNOMISUSE: {
        extern FILE* corrected_errnomisuse(void);
        FILE* ret;
        ret = corrected_errnomisuse();
        break;
    }
    case BUG_CHARACTERMISUSE: {
        extern void bug_charactermisuse(void);
        bug_charactermisuse();
        break;
    }
    case CORRECTED_CHARACTERMISUSE: {
        extern void corrected_charactermisuse(void);
        corrected_charactermisuse();
        break;
    }
    case BUG_CHAREOFCONFUSED: {
        extern char bug_chareofconfused(void);
        char ret;
        ret = bug_chareofconfused();
        break;
    }
    case CORRECTED_CHAREOFCONFUSED: {
        extern char corrected_chareofconfused(void);
        char ret;
        ret = corrected_chareofconfused();
        break;
    }
    case BUG_MEMCMPPADDINGDATA: {
        extern int bug_memcmppaddingdata(const S_Padding*, const S_Padding*);
        int ret;
        const S_Padding* arg__0;
        const S_Padding* arg__1;
        arg__0 = random_s_padding_pointer();
        arg__1 = random_s_padding_pointer();
        ret = bug_memcmppaddingdata(arg__0, arg__1);
        break;
    }
    case CORRECTED_MEMCMPPADDINGDATA: {
        extern int corrected_memcmppaddingdata(const S_Padding*, const S_Padding*);
        int ret;
        const S_Padding* arg__0;
        const S_Padding* arg__1;
        arg__0 = random_s_padding_pointer();
        arg__1 = random_s_padding_pointer();
        ret = corrected_memcmppaddingdata(arg__0, arg__1);
        break;
    }
    case BUG_NONREENTRANTSTDRETURN: {
        extern int bug_nonreentrantstdreturn(void);
        int ret;
        ret = bug_nonreentrantstdreturn();
        break;
    }
    case CORRECTED_NONREENTRANTSTDRETURN: {
        extern int corrected_nonreentrantstdreturn(void);
        int ret;
        ret = corrected_nonreentrantstdreturn();
        break;
    }
    case DEMO_CALL_BUG_MEMCMPSTRINGS: {
        extern int demo_call_bug_memcmpstrings(void);
        int ret;
        ret = demo_call_bug_memcmpstrings();
        break;
    }
    case BUG_CLOSEDRESOURCEUSE_FPRINTF: {
        extern void bug_closedresourceuse_fprintf(void);
        bug_closedresourceuse_fprintf();
        break;
    }
    case CORRECTED_CLOSEDRESOURCEUSE_FPRINTF: {
        extern void corrected_closedresourceuse_fprintf(void);
        corrected_closedresourceuse_fprintf();
        break;
    }
    case BUG_DOUBLERESOURCEOPEN: {
        extern void bug_doubleresourceopen(void);
        bug_doubleresourceopen();
        break;
    }
    case CORRECTED_DOUBLERESOURCEOPEN: {
        extern void corrected_doubleresourceopen(void);
        corrected_doubleresourceopen();
        break;
    }
    case BUG_DOUBLERESOURCECLOSE: {
        extern void bug_doubleresourceclose(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_doubleresourceclose(arg__0);
        break;
    }
    case CORRECTED_DOUBLERESOURCECLOSE: {
        extern void corrected_doubleresourceclose(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_doubleresourceclose(arg__0);
        break;
    }
    case BUG_READONLYRESOURCEWRITE: {
        extern void bug_readonlyresourcewrite(void);
        bug_readonlyresourcewrite();
        break;
    }
    case CORRECTED_READONLYRESOURCEWRITE: {
        extern void corrected_readonlyresourcewrite(void);
        corrected_readonlyresourcewrite();
        break;
    }
    case BUG_RESOURCELEAK: {
        extern void bug_resourceleak(void);
        bug_resourceleak();
        break;
    }
    case CORRECTED_RESOURCELEAK: {
        extern void corrected_resourceleak(void);
        corrected_resourceleak();
        break;
    }
    case BUG_BADNETWORKCONNECTORDER_SERVER: {
        extern void bug_badnetworkconnectorder_server(void);
        bug_badnetworkconnectorder_server();
        break;
    }
    case CORRECTED_BADNETWORKCONNECTORDER_SERVER: {
        extern void corrected_badnetworkconnectorder_server(void);
        corrected_badnetworkconnectorder_server();
        break;
    }
    case BUG_BADNETWORKCONNECTORDER_CLIENT: {
        extern int bug_badnetworkconnectorder_client(void);
        int ret;
        ret = bug_badnetworkconnectorder_client();
        break;
    }
    case CORRECTED_BADNETWORKCONNECTORDER_CLIENT: {
        extern int corrected_badnetworkconnectorder_client(void);
        int ret;
        ret = corrected_badnetworkconnectorder_client();
        break;
    }
    case BUG_BADUMASK: {
        extern void bug_badumask(void);
        bug_badumask();
        break;
    }
    case CORRECTED_BADUMASK: {
        extern void corrected_badumask(void);
        corrected_badumask();
        break;
    }
    case BUG_CHROOTMISUSE: {
        extern FILE* bug_chrootmisuse(void);
        FILE* ret;
        ret = bug_chrootmisuse();
        break;
    }
    case CORRECTED_CHROOTMISUSE: {
        extern FILE* corrected_chrootmisuse(void);
        FILE* ret;
        ret = corrected_chrootmisuse();
        break;
    }
    case BUG_DANGEROUSPERMISSIONS: {
        extern void bug_dangerouspermissions(void);
        bug_dangerouspermissions();
        break;
    }
    case CORRECTED_DANGEROUSPERMISSIONS: {
        extern void corrected_dangerouspermissions(void);
        corrected_dangerouspermissions();
        break;
    }
    case BUG_DANGEROUSSTDFUNC: {
        extern void bug_dangerousstdfunc(void);
        bug_dangerousstdfunc();
        break;
    }
    case CORRECTED_DANGEROUSSTDFUNC: {
        extern void corrected_dangerousstdfunc(void);
        corrected_dangerousstdfunc();
        break;
    }
    case CALL_DATALENGTHMISMATCH: {
        extern void call_datalengthmismatch(void);
        call_datalengthmismatch();
        break;
    }
    case BUG_FUNCPTRABSOLUTEADDR: {
        extern int* bug_funcptrabsoluteaddr(void);
        int* ret;
        ret = bug_funcptrabsoluteaddr();
        break;
    }
    case CORRECTED_FUNCPTRABSOLUTEADDR: {
        extern int* corrected_funcptrabsoluteaddr(void);
        int* ret;
        ret = corrected_funcptrabsoluteaddr();
        break;
    }
    case BUG_MISSINGSWITCHCASE: {
        extern void bug_missingswitchcase(void);
        bug_missingswitchcase();
        break;
    }
    case CORRECTED_MISSINGSWITCHCASE: {
        extern void corrected_missingswitchcase(void);
        corrected_missingswitchcase();
        break;
    }
    case BUG_NONSECURETEMPFILE: {
        extern FILE* bug_nonsecuretempfile(void);
        FILE* ret;
        ret = bug_nonsecuretempfile();
        break;
    }
    case CORRECTED_NONSECURETEMPFILE: {
        extern int corrected_nonsecuretempfile(void);
        int ret;
        ret = corrected_nonsecuretempfile();
        break;
    }
    case BUG_OBSOLETESTDFUNC: {
        extern unsigned short bug_obsoletestdfunc(void);
        unsigned short ret;
        ret = bug_obsoletestdfunc();
        break;
    }
    case CORRECTED_OBSOLETESTDFUNC: {
        extern double corrected_obsoletestdfunc(void);
        double ret;
        ret = corrected_obsoletestdfunc();
        break;
    }
    case BUG_PATHTRAVERSAL: {
        extern FILE* bug_pathtraversal(void);
        FILE* ret;
        ret = bug_pathtraversal();
        break;
    }
    case CORRECTED_PATHTRAVERSAL: {
        extern FILE* corrected_pathtraversal(void);
        FILE* ret;
        ret = corrected_pathtraversal();
        break;
    }
    case BUG_RANDSEEDCONSTANT: {
        extern void bug_randseedconstant(void);
        bug_randseedconstant();
        break;
    }
    case CORRECTED_RANDSEEDCONSTANT: {
        extern void corrected_randseedconstant(void);
        corrected_randseedconstant();
        break;
    }
    case BUG_RANDSEEDPREDICTABLE: {
        extern void bug_randseedpredictable(void);
        bug_randseedpredictable();
        break;
    }
    case CORRECTED_RANDSEEDPREDICTABLE: {
        extern void corrected_randseedpredictable(void);
        corrected_randseedpredictable();
        break;
    }
    case BUG_RELATIVEPATHCMD: {
        extern void bug_relativepathcmd(void);
        bug_relativepathcmd();
        break;
    }
    case CORRECTED_RELATIVEPATHCMD: {
        extern void corrected_relativepathcmd(void);
        corrected_relativepathcmd();
        break;
    }
    case BUG_RELATIVEPATHLIB: {
        extern void bug_relativepathlib(void);
        bug_relativepathlib();
        break;
    }
    case CORRECTED_RELATIVEPATHLIB: {
        extern void corrected_relativepathlib(void);
        corrected_relativepathlib();
        break;
    }
    case BUG_SENSITIVEDATAPRINT: {
        extern void bug_sensitivedataprint(void);
        bug_sensitivedataprint();
        break;
    }
    case CORRECTED_SENSITIVEDATAPRINT: {
        extern void corrected_sensitivedataprint(void);
        corrected_sensitivedataprint();
        break;
    }
    case BUG_SENSITIVEHEAPNOTCLEARED: {
        extern void bug_sensitiveheapnotcleared(void);
        bug_sensitiveheapnotcleared();
        break;
    }
    case CORRECTED_SENSITIVEHEAPNOTCLEARED: {
        extern void corrected_sensitiveheapnotcleared(void);
        corrected_sensitiveheapnotcleared();
        break;
    }
    case BUG_SENSITIVESTACKNOTCLEARED: {
        extern void bug_sensitivestacknotcleared(void);
        bug_sensitivestacknotcleared();
        break;
    }
    case CORRECTED_SENSITIVESTACKNOTCLEARED: {
        extern void corrected_sensitivestacknotcleared(void);
        corrected_sensitivestacknotcleared();
        break;
    }
    case BUG_TOCTOU: {
        extern void bug_toctou(void);
        bug_toctou();
        break;
    }
    case CORRECTED_TOCTOU: {
        extern void corrected_toctou(void);
        corrected_toctou();
        break;
    }
    case BUG_UNSAFESTDCRYPT: {
        extern char* bug_unsafestdcrypt(void);
        char* ret;
        ret = bug_unsafestdcrypt();
        break;
    }
    case CORRECTED_UNSAFESTDCRYPT: {
        extern char* corrected_unsafestdcrypt(void);
        char* ret;
        ret = corrected_unsafestdcrypt();
        break;
    }
    case BUG_UNSAFESTDFUNC: {
        extern char* bug_unsafestdfunc(void);
        char* ret;
        ret = bug_unsafestdfunc();
        break;
    }
    case CORRECTED_UNSAFESTDFUNC: {
        extern char* corrected_unsafestdfunc(void);
        char* ret;
        ret = corrected_unsafestdfunc();
        break;
    }
    case BUG_VULNERABLEPRNG: {
        extern int bug_vulnerableprng(void);
        int ret;
        ret = bug_vulnerableprng();
        break;
    }
    case CORRECTED_VULNERABLEPRNG: {
        extern unsigned char* corrected_vulnerableprng(void);
        unsigned char* ret;
        ret = corrected_vulnerableprng();
        break;
    }
    case BUG_BADPRIVILEGEDROPORDER: {
        extern void bug_badprivilegedroporder(void);
        bug_badprivilegedroporder();
        break;
    }
    case CORRECTED_BADPRIVILEGEDROPORDER: {
        extern void corrected_badprivilegedroporder(void);
        corrected_badprivilegedroporder();
        break;
    }
    case BUG_MISSINGPRIVILEGEDROPCHECK: {
        extern void bug_missingprivilegedropcheck(void);
        bug_missingprivilegedropcheck();
        break;
    }
    case CORRECTED_MISSINGPRIVILEGEDROPCHECK: {
        extern void corrected_missingprivilegedropcheck(void);
        corrected_missingprivilegedropcheck();
        break;
    }
    case BUG_RETURNNOTCHECKED: {
        extern void bug_returnnotchecked(void);
        bug_returnnotchecked();
        break;
    }
    case CORRECTED_RETURNNOTCHECKED: {
        extern void corrected_returnnotchecked(void);
        corrected_returnnotchecked();
        break;
    }
    case BUG_ERRNONOTCHECKED: {
        extern void bug_errnonotchecked(char*, char*);
        char* arg__0;
        char* arg__1;
        arg__0 = random_char_pointer();
        arg__1 = random_char_pointer();
        bug_errnonotchecked(arg__0, arg__1);
        break;
    }
    case CORRECTED_ERRNONOTCHECKED: {
        extern int corrected_errnonotchecked(char*, char*);
        int ret;
        char* arg__0;
        char* arg__1;
        arg__0 = random_char_pointer();
        arg__1 = random_char_pointer();
        ret = corrected_errnonotchecked(arg__0, arg__1);
        break;
    }
    case BUG_READLINKMISUSE: {
        extern void bug_readlinkmisuse(void);
        bug_readlinkmisuse();
        break;
    }
    case CORRECTED_READLINKMISUSE: {
        extern void corrected_readlinkmisuse(void);
        corrected_readlinkmisuse();
        break;
    }
    case BUG_CRYPTOCIPHERPREDICTABLEKEY: {
        extern int bug_cryptocipherpredictablekey(EVP_CIPHER_CTX*, unsigned char*);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        ret = bug_cryptocipherpredictablekey(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOCIPHERPREDICTABLEKEY: {
        extern int corrected_cryptocipherpredictablekey(EVP_CIPHER_CTX*, unsigned char*);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        ret = corrected_cryptocipherpredictablekey(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOCIPHERPREDICTABLEIV: {
        extern int bug_cryptocipherpredictableiv(EVP_CIPHER_CTX*, unsigned char*);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        ret = bug_cryptocipherpredictableiv(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOCIPHERPREDICTABLEIV: {
        extern int corrected_cryptocipherpredictableiv(EVP_CIPHER_CTX*, unsigned char*);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        ret = corrected_cryptocipherpredictableiv(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOCIPHERCONSTANTKEY: {
        extern int bug_cryptocipherconstantkey(EVP_CIPHER_CTX*, unsigned char*);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        ret = bug_cryptocipherconstantkey(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOCIPHERCONSTANTKEY: {
        extern int corrected_cryptocipherconstantkey(EVP_CIPHER_CTX*, unsigned char*);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        ret = corrected_cryptocipherconstantkey(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOCIPHERCONSTANTIV: {
        extern int bug_cryptocipherconstantiv(EVP_CIPHER_CTX*, unsigned char*);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        ret = bug_cryptocipherconstantiv(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOCIPHERCONSTANTIV: {
        extern int corrected_cryptocipherconstantiv(EVP_CIPHER_CTX*, unsigned char*);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        ret = corrected_cryptocipherconstantiv(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOCIPHERNOKEY: {
        extern int bug_cryptociphernokey(EVP_CIPHER_CTX*, unsigned char*, unsigned char*, int);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        unsigned char* arg__2;
        int arg__3;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        arg__2 = random_unsigned_char_pointer();
        arg__3 = pst_random_int;
        ret = bug_cryptociphernokey(arg__0, arg__1, arg__2, arg__3);
        break;
    }
    case CORRECTED_CRYPTOCIPHERNOKEY: {
        extern int corrected_cryptociphernokey(EVP_CIPHER_CTX*, unsigned char*, unsigned char*,
                                               int);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        unsigned char* arg__2;
        int arg__3;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        arg__2 = random_unsigned_char_pointer();
        arg__3 = pst_random_int;
        ret = corrected_cryptociphernokey(arg__0, arg__1, arg__2, arg__3);
        break;
    }
    case BUG_CRYPTOCIPHERNOIV: {
        extern int bug_cryptociphernoiv(EVP_CIPHER_CTX*, unsigned char*, unsigned char*, int);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        unsigned char* arg__2;
        int arg__3;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        arg__2 = random_unsigned_char_pointer();
        arg__3 = pst_random_int;
        ret = bug_cryptociphernoiv(arg__0, arg__1, arg__2, arg__3);
        break;
    }
    case CORRECTED_CRYPTOCIPHERNOIV: {
        extern int corrected_cryptociphernoiv(EVP_CIPHER_CTX*, unsigned char*, unsigned char*, int);
        int ret;
        EVP_CIPHER_CTX* arg__0;
        unsigned char* arg__1;
        unsigned char* arg__2;
        int arg__3;
        arg__0 = random_evp_cipher_ctx_pointer();
        arg__1 = random_unsigned_char_pointer();
        arg__2 = random_unsigned_char_pointer();
        arg__3 = pst_random_int;
        ret = corrected_cryptociphernoiv(arg__0, arg__1, arg__2, arg__3);
        break;
    }
    case BUG_CRYPTOCIPHERNOALGORITHM: {
        extern void bug_cryptociphernoalgorithm(void);
        bug_cryptociphernoalgorithm();
        break;
    }
    case CORRECTED_CRYPTOCIPHERNOALGORITHM: {
        extern void corrected_cryptociphernoalgorithm(unsigned char*, int, unsigned char*, int);
        unsigned char* arg__0;
        int arg__1;
        unsigned char* arg__2;
        int arg__3;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_int;
        arg__2 = random_unsigned_char_pointer();
        arg__3 = pst_random_int;
        corrected_cryptociphernoalgorithm(arg__0, arg__1, arg__2, arg__3);
        break;
    }
    case BUG_CRYPTOCIPHERNODATA: {
        extern void bug_cryptociphernodata(void);
        bug_cryptociphernodata();
        break;
    }
    case CORRECTED_CRYPTOCIPHERNODATA: {
        extern void corrected_cryptociphernodata(unsigned char*, int);
        unsigned char* arg__0;
        int arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_int;
        corrected_cryptociphernodata(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOCIPHERNOFINAL: {
        extern void bug_cryptociphernofinal(unsigned char*, int);
        unsigned char* arg__0;
        int arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_int;
        bug_cryptociphernofinal(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOCIPHERNOFINAL: {
        extern void corrected_cryptociphernofinal(unsigned char*, int);
        unsigned char* arg__0;
        int arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_int;
        corrected_cryptociphernofinal(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOCIPHERBADFUNCTION: {
        extern void bug_cryptocipherbadfunction(unsigned char*, int);
        unsigned char* arg__0;
        int arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_int;
        bug_cryptocipherbadfunction(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOCIPHERBADFUNCTION: {
        extern void corrected_cryptocipherbadfunction(unsigned char*, int);
        unsigned char* arg__0;
        int arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_int;
        corrected_cryptocipherbadfunction(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOCIPHERWEAKCIPHER: {
        extern void bug_cryptocipherweakcipher(unsigned char*, unsigned char*);
        unsigned char* arg__0;
        unsigned char* arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = random_unsigned_char_pointer();
        bug_cryptocipherweakcipher(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOCIPHERWEAKCIPHER: {
        extern void corrected_cryptocipherweakcipher(unsigned char*, unsigned char*);
        unsigned char* arg__0;
        unsigned char* arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = random_unsigned_char_pointer();
        corrected_cryptocipherweakcipher(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOCIPHERWEAKMODE: {
        extern void bug_cryptocipherweakmode(unsigned char*, unsigned char*);
        unsigned char* arg__0;
        unsigned char* arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = random_unsigned_char_pointer();
        bug_cryptocipherweakmode(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOCIPHERWEAKMODE: {
        extern void corrected_cryptocipherweakmode(unsigned char*, unsigned char*);
        unsigned char* arg__0;
        unsigned char* arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = random_unsigned_char_pointer();
        corrected_cryptocipherweakmode(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOPKEYNOPUBLICKEY: {
        extern int bug_cryptopkeynopublickey(unsigned char*, size_t);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        ret = bug_cryptopkeynopublickey(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOPKEYNOPUBLICKEY: {
        extern int corrected_cryptopkeynopublickey(unsigned char*, size_t, EVP_PKEY*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        EVP_PKEY* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_evp_pkey_pointer();
        ret = corrected_cryptopkeynopublickey(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_CRYPTOPKEYNOPRIVATEKEY: {
        extern int bug_cryptopkeynoprivatekey(unsigned char*, size_t);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        ret = bug_cryptopkeynoprivatekey(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOPKEYNOPRIVATEKEY: {
        extern int corrected_cryptopkeynoprivatekey(unsigned char*, size_t, EVP_PKEY*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        EVP_PKEY* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_evp_pkey_pointer();
        ret = corrected_cryptopkeynoprivatekey(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_CRYPTOPKEYNOPEER: {
        extern int bug_cryptopkeynopeer(EVP_PKEY*);
        int ret;
        EVP_PKEY* arg__0;
        arg__0 = random_evp_pkey_pointer();
        ret = bug_cryptopkeynopeer(arg__0);
        break;
    }
    case CORRECTED_CRYPTOPKEYNOPEER: {
        extern int corrected_cryptopkeynopeer(EVP_PKEY*, EVP_PKEY*);
        int ret;
        EVP_PKEY* arg__0;
        EVP_PKEY* arg__1;
        arg__0 = random_evp_pkey_pointer();
        arg__1 = random_evp_pkey_pointer();
        ret = corrected_cryptopkeynopeer(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTOPKEYNOPARAMS: {
        extern int bug_cryptopkeynoparams(EVP_PKEY*);
        int ret;
        EVP_PKEY* arg__0;
        arg__0 = random_evp_pkey_pointer();
        ret = bug_cryptopkeynoparams(arg__0);
        break;
    }
    case CORRECTED_CRYPTOPKEYNOPARAMS: {
        extern int corrected_cryptopkeynoparams(EVP_PKEY*);
        int ret;
        EVP_PKEY* arg__0;
        arg__0 = random_evp_pkey_pointer();
        ret = corrected_cryptopkeynoparams(arg__0);
        break;
    }
    case BUG_CRYPTOPKEYNODATA: {
        extern int bug_cryptopkeynodata(EVP_PKEY_CTX*);
        int ret;
        EVP_PKEY_CTX* arg__0;
        arg__0 = random_evp_pkey_ctx_pointer();
        ret = bug_cryptopkeynodata(arg__0);
        break;
    }
    case CORRECTED_CRYPTOPKEYNODATA: {
        extern int corrected_cryptopkeynodata(EVP_PKEY_CTX*);
        int ret;
        EVP_PKEY_CTX* arg__0;
        arg__0 = random_evp_pkey_ctx_pointer();
        ret = corrected_cryptopkeynodata(arg__0);
        break;
    }
    case BUG_CRYPTOPKEYWEAKPARAMS: {
        extern int bug_cryptopkeyweakparams(EVP_PKEY*);
        int ret;
        EVP_PKEY* arg__0;
        arg__0 = random_evp_pkey_pointer();
        ret = bug_cryptopkeyweakparams(arg__0);
        break;
    }
    case CORRECTED_CRYPTOPKEYWEAKPARAMS: {
        extern int corrected_cryptopkeyweakparams(EVP_PKEY*);
        int ret;
        EVP_PKEY* arg__0;
        arg__0 = random_evp_pkey_pointer();
        ret = corrected_cryptopkeyweakparams(arg__0);
        break;
    }
    case BUG_CRYPTOPKEYINCORRECTKEY: {
        extern int bug_cryptopkeyincorrectkey(unsigned char*, size_t, DSA*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        DSA* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_dsa_pointer();
        ret = bug_cryptopkeyincorrectkey(arg__0, arg__1, arg__2);
        break;
    }
    case CORRECTED_CRYPTOPKEYINCORRECTKEY: {
        extern int corrected_cryptopkeyincorrectkey(unsigned char*, size_t, RSA*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        RSA* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_rsa_pointer();
        ret = corrected_cryptopkeyincorrectkey(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_CRYPTOPKEYINCORRECTINIT: {
        extern int bug_cryptopkeyincorrectinit(unsigned char*, size_t, EVP_PKEY_CTX*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        EVP_PKEY_CTX* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_evp_pkey_ctx_pointer();
        ret = bug_cryptopkeyincorrectinit(arg__0, arg__1, arg__2);
        break;
    }
    case CORRECTED_CRYPTOPKEYINCORRECTINIT: {
        extern int corrected_cryptopkeyincorrectinit(unsigned char*, size_t, EVP_PKEY_CTX*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        EVP_PKEY_CTX* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_evp_pkey_ctx_pointer();
        ret = corrected_cryptopkeyincorrectinit(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_CRYPTORSALOWEXPONENT: {
        extern int bug_cryptorsalowexponent(EVP_PKEY*);
        int ret;
        EVP_PKEY* arg__0;
        arg__0 = random_evp_pkey_pointer();
        ret = bug_cryptorsalowexponent(arg__0);
        break;
    }
    case CORRECTED_CRYPTORSALOWEXPONENT: {
        extern int corrected_cryptorsalowexponent(EVP_PKEY*);
        int ret;
        EVP_PKEY* arg__0;
        arg__0 = random_evp_pkey_pointer();
        ret = corrected_cryptorsalowexponent(arg__0);
        break;
    }
    case BUG_CRYPTORSANOPADDING: {
        extern int bug_cryptorsanopadding(unsigned char*, size_t);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        ret = bug_cryptorsanopadding(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTORSANOPADDING: {
        extern int corrected_cryptorsanopadding(unsigned char*, size_t);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        ret = corrected_cryptorsanopadding(arg__0, arg__1);
        break;
    }
    case BUG_CRYPTORSAWEAKPADDING: {
        extern int bug_cryptorsaweakpadding(unsigned char*, size_t, RSA*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        RSA* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_rsa_pointer();
        ret = bug_cryptorsaweakpadding(arg__0, arg__1, arg__2);
        break;
    }
    case CORRECTED_CRYPTORSAWEAKPADDING: {
        extern int corrected_cryptorsaweakpadding(unsigned char*, size_t, RSA*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        RSA* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_rsa_pointer();
        ret = corrected_cryptorsaweakpadding(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_CRYPTORSABADPADDING: {
        extern int bug_cryptorsabadpadding(unsigned char*, size_t, RSA*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        RSA* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_rsa_pointer();
        ret = bug_cryptorsabadpadding(arg__0, arg__1, arg__2);
        break;
    }
    case CORRECTED_CRYPTORSABADPADDING: {
        extern int corrected_cryptorsabadpadding(unsigned char*, size_t, RSA*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        RSA* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_rsa_pointer();
        ret = corrected_cryptorsabadpadding(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_CRYPTORSANOBLINDING: {
        extern int bug_cryptorsanoblinding(unsigned char*, size_t, RSA*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        RSA* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_rsa_pointer();
        ret = bug_cryptorsanoblinding(arg__0, arg__1, arg__2);
        break;
    }
    case CORRECTED_CRYPTORSANOBLINDING: {
        extern int corrected_cryptorsanoblinding(unsigned char*, size_t, RSA*);
        int ret;
        unsigned char* arg__0;
        size_t arg__1;
        RSA* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_rsa_pointer();
        ret = corrected_cryptorsanoblinding(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_CRYPTOMDWEAKHASH: {
        extern void bug_cryptomdweakhash(unsigned char*, size_t, EVP_PKEY*);
        unsigned char* arg__0;
        size_t arg__1;
        EVP_PKEY* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_evp_pkey_pointer();
        bug_cryptomdweakhash(arg__0, arg__1, arg__2);
        break;
    }
    case CORRECTED_CRYPTOMDWEAKHASH: {
        extern void corrected_cryptomdweakhash(unsigned char*, size_t, EVP_PKEY*);
        unsigned char* arg__0;
        size_t arg__1;
        EVP_PKEY* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_evp_pkey_pointer();
        corrected_cryptomdweakhash(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_CRYPTOMDBADFUNCTION: {
        extern void bug_cryptomdbadfunction(unsigned char*, size_t);
        unsigned char* arg__0;
        size_t arg__1;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        bug_cryptomdbadfunction(arg__0, arg__1);
        break;
    }
    case CORRECTED_CRYPTOMDBADFUNCTION: {
        extern void corrected_cryptomdbadfunction(unsigned char*, size_t, EVP_PKEY*);
        unsigned char* arg__0;
        size_t arg__1;
        EVP_PKEY* arg__2;
        arg__0 = random_unsigned_char_pointer();
        arg__1 = pst_random_size_t;
        arg__2 = random_evp_pkey_pointer();
        corrected_cryptomdbadfunction(arg__0, arg__1, arg__2);
        break;
    }
    case BUG_CRYPTOSSLWEAKPROTOCOL: {
        extern int bug_cryptosslweakprotocol(void);
        int ret;
        ret = bug_cryptosslweakprotocol();
        break;
    }
    case CORRECTED_CRYPTOSSLWEAKPROTOCOL: {
        extern int corrected_cryptosslweakprotocol(void);
        int ret;
        ret = corrected_cryptosslweakprotocol();
        break;
    }
    case BUG_OUTBOUNDARRAY: {
        extern void bug_outboundarray(void);
        bug_outboundarray();
        break;
    }
    case CORRECTED_OUTBOUNDARRAY: {
        extern void corrected_outboundarray(void);
        corrected_outboundarray();
        break;
    }
    case BUG_OUTBOUNDPTR: {
        extern void bug_outboundptr(void);
        bug_outboundptr();
        break;
    }
    case CORRECTED_OUTBOUNDPTR: {
        extern void corrected_outboundptr(void);
        corrected_outboundptr();
        break;
    }
    case BUG_NULLPTR: {
        extern void bug_nullptr(void);
        bug_nullptr();
        break;
    }
    case CORRECTED_NULLPTR: {
        extern void corrected_nullptr(void);
        corrected_nullptr();
        break;
    }
    case BUG_LOCALADDRESCAPE: {
        extern int* bug_localaddrescape(void);
        int* ret;
        ret = bug_localaddrescape();
        break;
    }
    case CORRECTED_LOCALADDRESCAPE: {
        extern int corrected_localaddrescape(void);
        int ret;
        ret = corrected_localaddrescape();
        break;
    }
    case BUG_OBJECTSIZEMISMATCH_ALLOC: {
        extern int* bug_objectsizemismatch_alloc(void);
        int* ret;
        ret = bug_objectsizemismatch_alloc();
        break;
    }
    case CORRECTED_OBJECTSIZEMISMATCH_ALLOC: {
        extern int* corrected_objectsizemismatch_alloc(void);
        int* ret;
        ret = corrected_objectsizemismatch_alloc();
        break;
    }
    case BUG_MEMSTDLIB: {
        extern void bug_memstdlib(void);
        bug_memstdlib();
        break;
    }
    case CORRECTED_MEMSTDLIB: {
        extern void corrected_memstdlib(void);
        corrected_memstdlib();
        break;
    }
    case BUG_STRSTDLIB: {
        extern double bug_strstdlib(void);
        double ret;
        ret = bug_strstdlib();
        break;
    }
    case CORRECTED_STRSTDLIB: {
        extern double corrected_strstdlib(void);
        double ret;
        ret = corrected_strstdlib();
        break;
    }
    case BUG_NULLPTRARITH: {
        extern void bug_nullptrarith(void);
        bug_nullptrarith();
        break;
    }
    case CORRECTED_NULLPTRARITH: {
        extern void corrected_nullptrarith(void);
        corrected_nullptrarith();
        break;
    }
    case BUG_PTRCAST: {
        extern char* bug_ptrcast(int*);
        char* ret;
        int* arg__0;
        arg__0 = random_int_pointer();
        ret = bug_ptrcast(arg__0);
        break;
    }
    case CORRECTED_PTRCAST: {
        extern int* corrected_ptrcast(int*);
        int* ret;
        int* arg__0;
        arg__0 = random_int_pointer();
        ret = corrected_ptrcast(arg__0);
        break;
    }
    case BUG_FUNCCAST: {
        extern void bug_funccast(void);
        bug_funccast();
        break;
    }
    case CORRECTED_FUNCCAST: {
        extern void corrected_funccast(void);
        corrected_funccast();
        break;
    }
    case BUG_PATHBUFFEROVERFLOW: {
        extern void bug_pathbufferoverflow(void);
        bug_pathbufferoverflow();
        break;
    }
    case CORRECTED_PATHBUFFEROVERFLOW: {
        extern void corrected_pathbufferoverflow(void);
        corrected_pathbufferoverflow();
        break;
    }
    case BUG_STRLIBBUFFEROVERFLOW: {
        extern void bug_strlibbufferoverflow(float);
        float arg__0;
        arg__0 = pst_random_float;
        bug_strlibbufferoverflow(arg__0);
        break;
    }
    case CORRECTED_STRLIBBUFFEROVERFLOW: {
        extern void corrected_strlibbufferoverflow(float);
        float arg__0;
        arg__0 = pst_random_float;
        corrected_strlibbufferoverflow(arg__0);
        break;
    }
    case BUG_STRLIBBUFFERUNDERFLOW: {
        extern void bug_strlibbufferunderflow(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_strlibbufferunderflow(arg__0);
        break;
    }
    case CORRECTED_STRLIBBUFFERUNDERFLOW: {
        extern void corrected_strlibbufferunderflow(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_strlibbufferunderflow(arg__0);
        break;
    }
    case BUG_STRFORMATBUFFEROVERFLOW: {
        extern void bug_strformatbufferoverflow(void);
        bug_strformatbufferoverflow();
        break;
    }
    case CORRECTED_STRFORMATBUFFEROVERFLOW: {
        extern void corrected_strformatbufferoverflow(void);
        corrected_strformatbufferoverflow();
        break;
    }
    case BUG_TAINTEDARRAYINDEX: {
        extern int bug_taintedarrayindex(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_taintedarrayindex(arg__0);
        break;
    }
    case CORRECTED_TAINTEDARRAYINDEX: {
        extern int corrected_taintedarrayindex(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_taintedarrayindex(arg__0);
        break;
    }
    case BUG_TAINTEDINTDIVISION: {
        extern int bug_taintedintdivision(int, int);
        int ret;
        int arg__0;
        int arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        ret = bug_taintedintdivision(arg__0, arg__1);
        break;
    }
    case CORRECTED_TAINTEDINTDIVISION: {
        extern int corrected_taintedintdivision(int, int);
        int ret;
        int arg__0;
        int arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        ret = corrected_taintedintdivision(arg__0, arg__1);
        break;
    }
    case BUG_TAINTEDINTMOD: {
        extern int bug_taintedintmod(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_taintedintmod(arg__0);
        break;
    }
    case CORRECTED_TAINTEDINTMOD: {
        extern int corrected_taintedintmod(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_taintedintmod(arg__0);
        break;
    }
    case BUG_TAINTEDLOOPBOUNDARY: {
        extern int bug_taintedloopboundary(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_taintedloopboundary(arg__0);
        break;
    }
    case CORRECTED_TAINTEDLOOPBOUNDARY: {
        extern int corrected_taintedloopboundary(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_taintedloopboundary(arg__0);
        break;
    }
    case BUG_TAINTEDSIGNCHANGE: {
        extern void bug_taintedsignchange(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_taintedsignchange(arg__0);
        break;
    }
    case CORRECTED_TAINTEDSIGNCHANGE: {
        extern void corrected_taintedsignchange(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_taintedsignchange(arg__0);
        break;
    }
    case BUG_TAINTEDSTRING: {
        extern void bug_taintedstring(char*);
        char* arg__0;
        arg__0 = random_char_pointer();
        bug_taintedstring(arg__0);
        break;
    }
    case CORRECTED_TAINTEDSTRING: {
        extern void corrected_taintedstring(char*);
        char* arg__0;
        arg__0 = random_char_pointer();
        corrected_taintedstring(arg__0);
        break;
    }
    case BUG_TAINTEDSTRINGFORMAT: {
        extern void bug_taintedstringformat(char*);
        char* arg__0;
        arg__0 = random_char_pointer();
        bug_taintedstringformat(arg__0);
        break;
    }
    case CORRECTED_TAINTEDSTRINGFORMAT: {
        extern void corrected_taintedstringformat(char*);
        char* arg__0;
        arg__0 = random_char_pointer();
        corrected_taintedstringformat(arg__0);
        break;
    }
    case BUG_TAINTEDVLASIZE: {
        extern int bug_taintedvlasize(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_taintedvlasize(arg__0);
        break;
    }
    case CORRECTED_TAINTEDVLASIZE: {
        extern int corrected_taintedvlasize(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_taintedvlasize(arg__0);
        break;
    }
    case BUG_TAINTEDMEMORYALLOCSIZE: {
        extern int* bug_taintedmemoryallocsize(unsigned long);
        int* ret;
        unsigned long arg__0;
        arg__0 = pst_random_unsigned_long;
        ret = bug_taintedmemoryallocsize(arg__0);
        break;
    }
    case CORRECTED_TAINTEDMEMORYALLOCSIZE: {
        extern int* corrected_taintedmemoryallocsize(unsigned long);
        int* ret;
        unsigned long arg__0;
        arg__0 = pst_random_unsigned_long;
        ret = corrected_taintedmemoryallocsize(arg__0);
        break;
    }
    case BUG_TAINTEDENVVARIABLE: {
        extern void bug_taintedenvvariable(char*);
        char* arg__0;
        arg__0 = random_char_pointer();
        bug_taintedenvvariable(arg__0);
        break;
    }
    case CORRECTED_TAINTEDENVVARIABLE: {
        extern int corrected_taintedenvvariable(unsigned int);
        int ret;
        unsigned int arg__0;
        arg__0 = pst_random_unsigned_int;
        ret = corrected_taintedenvvariable(arg__0);
        break;
    }
    case BUG_TAINTEDHOSTID: {
        extern void bug_taintedhostid(long);
        long arg__0;
        arg__0 = pst_random_long;
        bug_taintedhostid(arg__0);
        break;
    }
    case CORRECTED_TAINTEDHOSTID: {
        extern void corrected_taintedhostid(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_taintedhostid(arg__0);
        break;
    }
    case BUG_TAINTEDEXTERNALCMD: {
        extern void bug_taintedexternalcmd(char*);
        char* arg__0;
        arg__0 = random_char_pointer();
        bug_taintedexternalcmd(arg__0);
        break;
    }
    case CORRECTED_TAINTEDEXTERNALCMD: {
        extern void corrected_taintedexternalcmd(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_taintedexternalcmd(arg__0);
        break;
    }
    case BUG_TAINTEDPATHCMD: {
        extern void bug_taintedpathcmd(void);
        bug_taintedpathcmd();
        break;
    }
    case CORRECTED_TAINTEDPATHCMD: {
        extern void corrected_taintedpathcmd(void);
        corrected_taintedpathcmd();
        break;
    }
    case BUG_TAINTEDPATHLIB: {
        extern void* bug_taintedpathlib(void);
        void* ret;
        ret = bug_taintedpathlib();
        break;
    }
    case CORRECTED_TAINTEDPATHLIB: {
        extern void* corrected_taintedpathlib(unsigned int);
        void* ret;
        unsigned int arg__0;
        arg__0 = pst_random_unsigned_int;
        ret = corrected_taintedpathlib(arg__0);
        break;
    }
    case BUG_TAINTEDPTROFFSET: {
        extern int bug_taintedptroffset(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = bug_taintedptroffset(arg__0);
        break;
    }
    case CORRECTED_TAINTEDPTROFFSET: {
        extern int corrected_taintedptroffset(int);
        int ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = corrected_taintedptroffset(arg__0);
        break;
    }
    case BUG_IOINTERLEAVING: {
        extern void bug_iointerleaving(void);
        bug_iointerleaving();
        break;
    }
    case CORRECTED_IOINTERLEAVING: {
        extern void corrected_iointerleaving(void);
        corrected_iointerleaving();
        break;
    }
    case BUG_SIGHANDLERCOMPEXCPRETURN: {
        extern void bug_sighandlercompexcpreturn(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_sighandlercompexcpreturn(arg__0);
        break;
    }
    case CORRECTED_SIGHANDLERCOMPEXCPRETURN: {
        extern void corrected_sighandlercompexcpreturn(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_sighandlercompexcpreturn(arg__0);
        break;
    }
    case DEMO_SIGHANDLERFORCOMPUTATIONEXCEPTION: {
        extern long demo_sighandlerforcomputationexception(int);
        long ret;
        int arg__0;
        arg__0 = pst_random_int;
        ret = demo_sighandlerforcomputationexception(arg__0);
        break;
    }
    case BUG_SIGHANDLERASYNCUNSAFE: {
        extern void bug_sighandlerasyncunsafe(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_sighandlerasyncunsafe(arg__0);
        break;
    }
    case DEMO_BUG_SIGHANDLERASYNCUNSAFE: {
        extern int demo_bug_sighandlerasyncunsafe(void);
        int ret;
        ret = demo_bug_sighandlerasyncunsafe();
        break;
    }
    case CORRECTED_SIGHANDLERASYNCUNSAFE: {
        extern void corrected_sighandlerasyncunsafe(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_sighandlerasyncunsafe(arg__0);
        break;
    }
    case DEMO_CORRECTED_SIGHANDLERASYNCUNSAFE: {
        extern int demo_corrected_sighandlerasyncunsafe(void);
        int ret;
        ret = demo_corrected_sighandlerasyncunsafe();
        break;
    }
    case BUG_SIGHANDLERASYNCUNSAFESTRICT: {
        extern void bug_sighandlerasyncunsafestrict(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_sighandlerasyncunsafestrict(arg__0);
        break;
    }
    case DEMO_BUG_SIGHANDLERASYNCUNSAFESTRICT: {
        extern int demo_bug_sighandlerasyncunsafestrict(void);
        int ret;
        ret = demo_bug_sighandlerasyncunsafestrict();
        break;
    }
    case CORRECTED_SIGHANDLERASYNCUNSAFESTRICT: {
        extern void corrected_sighandlerasyncunsafestrict(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_sighandlerasyncunsafestrict(arg__0);
        break;
    }
    case DEMO_CORRECTED_SIGHANDLERASYNCUNSAFESTRICT: {
        extern int demo_corrected_sighandlerasyncunsafestrict(void);
        int ret;
        ret = demo_corrected_sighandlerasyncunsafestrict();
        break;
    }
    case DEMO_BUG_SIGHANDLERERRNOMISUSE: {
        extern int demo_bug_sighandlererrnomisuse(void);
        int ret;
        ret = demo_bug_sighandlererrnomisuse();
        break;
    }
    case DEMO_CORRECTED_SIGHANDLERERRNOMISUSE: {
        extern int demo_corrected_sighandlererrnomisuse(void);
        int ret;
        ret = demo_corrected_sighandlererrnomisuse();
        break;
    }
    case BUG_UNPROTOTYPEDFUNCCALL: {
        extern void bug_unprototypedfunccall(void);
        bug_unprototypedfunccall();
        break;
    }
    case CORRECTED_UNPROTOTYPEDFUNCCALL: {
        extern void corrected_unprototypedfunccal(void);
        corrected_unprototypedfunccal();
        break;
    }
    case BUG_FILEEXPOSURETOCHILD: {
        extern void bug_fileexposuretochild(void);
        bug_fileexposuretochild();
        break;
    }
    case CORRECTED_FILEEXPOSURETOCHILD: {
        extern void corrected_fileexposuretochild(void);
        corrected_fileexposuretochild();
        break;
    }
    case BUG_FILEOBJECTMISUSE: {
        extern int bug_fileobjectmisuse(void);
        (void)bug_fileobjectmisuse();
        break;
    }
    case CORRECTED_FILEOBJECTMISUSE: {
        extern int corrected_fileobjectmisuse(void);
        (void)corrected_fileobjectmisuse();
        break;
    }
    case BUG_FLEXIBLEARRAYMEMBERSTRUCTMISUSE: {
        extern void bug_flexiblearraymemberstructmisuse(void);
        bug_flexiblearraymemberstructmisuse();
        break;
    }
    case CORRECTED_FLEXIBLEARRAYMEMBERSTRUCTMISUSE: {
        extern void corrected_flexiblearraymemberstructmisuse(void);
        corrected_flexiblearraymemberstructmisuse();
        break;
    }
    case BUG_INVALIDFILEPOS: {
        extern FILE* bug_invalidfilepos(FILE * file);
        FILE* arg__0;
        arg__0 = random_file_pointer();
        (void)bug_invalidfilepos(arg__0);
        break;
    }
    case CORRECTED_INVALIDFILEPOS: {
        extern FILE* corrected_invalidfilepos(FILE * file);
        FILE* arg__0;
        arg__0 = random_file_pointer();
        (void)corrected_invalidfilepos(arg__0);
        break;
    }
    case BUG_PTRTODIFFARRAY: {
        extern size_t bug_ptrtodiffarray(void);
        (void)bug_ptrtodiffarray();
        break;
    }
    case CORRECTED_PTRTODIFFARRAY: {
        extern size_t corrected_ptrtodiffarray(void);
        (void)corrected_ptrtodiffarray();
        break;
    }
    case BUG_ALIGNMENTCHANGE: {
        extern void bug_alignmentchange(void);
        bug_alignmentchange();
        break;
    }
    case CORRECTED_ALIGNMENTCHANGE: {
        extern void corrected_alignmentchange(void);
        corrected_alignmentchange();
        break;
    }
    case BUG_PUTENVAUTOVAR: {
        extern void bug_putenvautovar(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_putenvautovar(arg__0);
        break;
    }
    case CORRECTED_PUTENVAUTOVAR: {
        extern void corrected_putenvautovar(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_putenvautovar(arg__0);
        break;
    }
    case BUG_SIGHANDLERSHAREDOBJECT: {
        extern void bug_sighandlersharedobject(int);
        int arg__0;
        arg__0 = pst_random_int;
        bug_sighandlersharedobject(arg__0);
        break;
    }
    case CORRECTED_SIGHANDLERSHAREDOBJECT: {
        extern void corrected_sighandlersharedobject(int);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_sighandlersharedobject(arg__0);
        break;
    }
    case BUG_MISSINGBYTESWAP: {
        extern unsigned int bug_missingbyteswap(int, int);
        int arg__0, arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        (void)bug_missingbyteswap(arg__0, arg__1);
        break;
    }
    case CORRECTED_MISSINGBYTESWAP: {
        extern unsigned int corrected_missingbyteswap(int, int);
        int arg__0, arg__1;
        arg__0 = pst_random_int;
        arg__1 = pst_random_int;
        (void)corrected_missingbyteswap(arg__0, arg__1);
        break;
    }
    case BUG_WINMISMATCHDEALLOC: {
        extern void bug_winmismatchdealloc(void);
        bug_winmismatchdealloc();
        break;
    }
    case CORRECTED_WINMISMATCHDEALLOC: {
        extern void corrected_winmismatchdealloc(void);
        corrected_winmismatchdealloc();
        break;
    }
    case BUG_INDETERMINATESTRING: {
        extern void bug_indeterminatestring(void);
        bug_indeterminatestring();
        break;
    }
    case CORRECTED_INDETERMINATESTRING: {
        extern void corrected_indeterminatestring(void);
        corrected_indeterminatestring();
        break;
    }
    case BUG_UNSAFESYSTEMCALL: {
        extern void bug_unsafesystemcall(void);
        (void)bug_unsafesystemcall();
        break;
    }
    case CORRECTED_UNSAFESYSTEMCALL: {
        extern void corrected_unsafesystemcall(void);
        (void)corrected_unsafesystemcall();
        break;
    }
    case BUG_SIDEEFFECTIGNORED: {
        extern void bug_sideeffectignored(void);
        bug_sideeffectignored();
        break;
    }
    case CORRECTED_SIDEEFFECTIGNORED: {
        extern void corrected_sideeffectignored(void);
        corrected_sideeffectignored();
        break;
    }
    case BUG_PREDIRECTIVEMACROARG: {
        extern void bug_predirectivemacroarg(void);
        bug_predirectivemacroarg();
        break;
    }
    case CORRECTED_PREDIRECTIVEMACROARG: {
        extern void corrected_predirectivemacroarg(void);
        corrected_predirectivemacroarg();
        break;
    }
    case DEMO_INLINECONSTRAINTNOTRESPECTED: {
        extern void demo_inlineconstraintnotrespected(void);
        demo_inlineconstraintnotrespected();
        break;
    }
    case DEMO_VAARGINCORRECTTYPE: {
        extern void demo_vaargincorrecttype(void);
        demo_vaargincorrecttype();
        break;
    }
    case DEMO_TOOMANYVAARGCALLS: {
        extern void demo_toomanyvaargcalls(void);
        demo_toomanyvaargcalls();
        break;
    }
    case DEMO_TEMPOBJECTACCESS: {
        extern void demo_tempobjectaccess(void);
        demo_tempobjectaccess();
        break;
    }
    case DEMO_STREAMWITHSIDEEFFECT: {
        extern void demo_streamwithsideeffect(void);
        demo_streamwithsideeffect();
        break;
    }
    case DEMO_PADDINGINFOLEAK: {
        extern void demo_paddinginfoleak(void);
        demo_paddinginfoleak();
        break;
    }
    case DEMO_MEMCMPFLOAT: {
        extern void demo_memcmpfloat(void);
        demo_memcmpfloat();
        break;
    }
    case DEMO_INVALIDENVPOINTER: {
        extern void demo_invalidenvpointer(int, char**, char**);
        demo_invalidenvpointer(argc, argv, envp);
        break;
    }
    case BUG_INTTOFLOATPRECISIONLOSS: {
        extern void bug_inttofloatprecisionloss(void);
        bug_inttofloatprecisionloss();
        break;
    }
    case CORRECTED_INTTOFLOATPRECISIONLOSS: {
        extern void corrected_inttofloatprecisionloss(void);
        corrected_inttofloatprecisionloss();
        break;
    }
    case BUG_TOOMANYCALLS: {
        extern double bug_toomanycalls(void);
        bug_toomanycalls();
        break;
    }
    case CORRECTED_TOOMANYCALLS: {
        extern double corrected_toomanycalls(void);
        corrected_toomanycalls();
        break;
    }
    case BUG_TOOMANYGOTO: {
        extern int bug_toomanygoto(int i);
        int arg__0;
        arg__0 = pst_random_int;
        bug_toomanygoto(arg__0);
        break;
    }
    case CORRECTED_TOOMANYGOTO: {
        extern int corrected_toomanygoto(int i);
        int arg__0;
        arg__0 = pst_random_int;
        corrected_toomanygoto(arg__0);
        break;
    }
    case BUG_TOOMANYLEVELS: {
        extern int bug_toomanylevels(void);
        bug_toomanylevels();
        break;
    }
    case CORRECTED_TOOMANYLEVELS: {
        extern int corrected_toomanylevels(void);
        corrected_toomanylevels();
    }
    default:
        break;
    }
    return 0;
}
#else /* PST_BUG_FINDER */
/*==========================================================================
 *  Main program is required for some checkers demo
 *==========================================================================*/
int main(int argc, char* argv[], char* envp[]) {
    switch (pst_random_int) {
    case DEMO_INVALIDENVPOINTER: {
        extern void demo_invalidenvpointer(int, char**, char**);
        demo_invalidenvpointer(argc, argv, envp);
        break;
    }
    case BUG_TAINTEDARRAYINDEX: {
        extern int bug_taintedarrayindex(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = bug_taintedarrayindex(arg__0);
        break;
    }
    case CORRECTED_TAINTEDARRAYINDEX: {
        extern int corrected_taintedarrayindex(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = corrected_taintedarrayindex(arg__0);
        break;
    }
    case BUG_TAINTEDINTDIVISION: {
        extern int bug_taintedintdivision(int, int);
        int ret;
        int arg__0;
        int arg__1;
        arg__0 = tainted_int();
        arg__1 = tainted_int();
        ret = bug_taintedintdivision(arg__0, arg__1);
        break;
    }
    case CORRECTED_TAINTEDINTDIVISION: {
        extern int corrected_taintedintdivision(int, int);
        int ret;
        int arg__0;
        int arg__1;
        arg__0 = tainted_int();
        arg__1 = tainted_int();
        ret = corrected_taintedintdivision(arg__0, arg__1);
        break;
    }
    case BUG_TAINTEDINTMOD: {
        extern int bug_taintedintmod(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = bug_taintedintmod(arg__0);
        break;
    }
    case CORRECTED_TAINTEDINTMOD: {
        extern int corrected_taintedintmod(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = corrected_taintedintmod(arg__0);
        break;
    }
    case BUG_TAINTEDLOOPBOUNDARY: {
        extern int bug_taintedloopboundary(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = bug_taintedloopboundary(arg__0);
        break;
    }
    case CORRECTED_TAINTEDLOOPBOUNDARY: {
        extern int corrected_taintedloopboundary(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = corrected_taintedloopboundary(arg__0);
        break;
    }
    case BUG_TAINTEDSTRING: {
        extern void bug_taintedstring(char*);
        char* arg__0;
        arg__0 = tainted_char_pointer();
        bug_taintedstring(arg__0);
        break;
    }
    case CORRECTED_TAINTEDSTRING: {
        extern void corrected_taintedstring(char*);
        char* arg__0;
        arg__0 = tainted_char_pointer();
        corrected_taintedstring(arg__0);
        break;
    }
    case BUG_TAINTEDSTRINGFORMAT: {
        extern void bug_taintedstringformat(char*);
        char* arg__0;
        arg__0 = tainted_string();
        bug_taintedstringformat(arg__0);
        break;
    }
    case CORRECTED_TAINTEDSTRINGFORMAT: {
        extern void corrected_taintedstringformat(char*);
        char* arg__0;
        arg__0 = tainted_string();
        corrected_taintedstringformat(arg__0);
        break;
    }
    case BUG_TAINTEDVLASIZE: {
        extern int bug_taintedvlasize(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = bug_taintedvlasize(arg__0);
        break;
    }
    case CORRECTED_TAINTEDVLASIZE: {
        extern int corrected_taintedvlasize(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = corrected_taintedvlasize(arg__0);
        break;
    }
    case BUG_TAINTEDMEMORYALLOCSIZE: {
        extern int* bug_taintedmemoryallocsize(unsigned long);
        int* ret;
        unsigned long arg__0;
        arg__0 = tainted_unsigned_long();
        ret = bug_taintedmemoryallocsize(arg__0);
        break;
    }
    case CORRECTED_TAINTEDMEMORYALLOCSIZE: {
        extern int* corrected_taintedmemoryallocsize(unsigned long);
        int* ret;
        int arg__0;
        arg__0 = tainted_unsigned_long();
        ret = corrected_taintedmemoryallocsize(arg__0);
        break;
    }
    case BUG_TAINTEDENVVARIABLE: {
        extern void bug_taintedenvvariable(char*);
        char* arg__0;
        arg__0 = tainted_string();
        bug_taintedenvvariable(arg__0);
        break;
    }
    case CORRECTED_TAINTEDENVVARIABLE: {
        extern int corrected_taintedenvvariable(unsigned int);
        int ret;
        unsigned int arg__0;
        arg__0 = tainted_int();
        ret = corrected_taintedenvvariable(arg__0);
        break;
    }
    case BUG_TAINTEDHOSTID: {
        extern void bug_taintedhostid(long);
        long arg__0;
        arg__0 = tainted_long();
        bug_taintedhostid(arg__0);
        break;
    }
    case CORRECTED_TAINTEDHOSTID: {
        extern void corrected_taintedhostid(int);
        int arg__0;
        arg__0 = tainted_int();
        corrected_taintedhostid(arg__0);
        break;
    }
    case BUG_TAINTEDEXTERNALCMD: {
        extern void bug_taintedexternalcmd(char*);
        char* arg__0;
        arg__0 = tainted_string();
        bug_taintedexternalcmd(arg__0);
        break;
    }
    case CORRECTED_TAINTEDEXTERNALCMD: {
        extern void corrected_taintedexternalcmd(int);
        int arg__0;
        arg__0 = tainted_int();
        corrected_taintedexternalcmd(arg__0);
        break;
    }
    case BUG_TAINTEDPATHCMD: {
        extern void bug_taintedpathcmd(void);
        bug_taintedpathcmd();
        break;
    }
    case CORRECTED_TAINTEDPATHCMD: {
        extern void corrected_taintedpathcmd(void);
        corrected_taintedpathcmd();
        break;
    }
    case BUG_TAINTEDPATHLIB: {
        extern void* bug_taintedpathlib(void);
        void* ret;
        ret = bug_taintedpathlib();
        break;
    }
    case CORRECTED_TAINTEDPATHLIB: {
        extern void* corrected_taintedpathlib(unsigned int);
        void* ret;
        unsigned int arg__0;
        arg__0 = tainted_unsigned_int();
        ret = corrected_taintedpathlib(arg__0);
        break;
    }
    case BUG_TAINTEDPTROFFSET: {
        extern int bug_taintedptroffset(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = bug_taintedptroffset(arg__0);
        break;
    }
    case CORRECTED_TAINTEDPTROFFSET: {
        extern int corrected_taintedptroffset(int);
        int ret;
        int arg__0;
        arg__0 = tainted_int();
        ret = corrected_taintedptroffset(arg__0);
        break;
    }
    default:
        break;
    }
    return 0;
}
#endif /* PST_BUG_FINDER */
#endif /* PSTEST_BUILD */
