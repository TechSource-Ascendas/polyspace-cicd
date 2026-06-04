/**
 *       Copyright 2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * This header file contains global definitions needed by the example sources.
 */

#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

#define fatal_error() abort()

enum { SIZE2 = 2, SIZE3 = 3, SIZE4 = 4, SIZE5 = 5, SIZE6 = 6, SIZE9 = 9, SIZE10 = 10, SIZE16 = 16,
       SIZE20 = 20, SIZE64 = 64, SIZE100 = 100, SIZE128 = 128, SIZE1024 = 1024, SIZE1025 = 1025 };

typedef struct s_padding {
    char c; /* On 32-bit target, padding data can be introduced
             * between 'c' and 'i' with no specific alignment instructions
             */
    int i;
    unsigned int bf1 : 1; /* Padding data can be introduced around the bit-fields if they
                           * can not be regrouped as a whole addressable unit. This concern
                           * is unavoidable even with the alignment instruction "pack"
                           * supported by Microsoft Visual C/C++ or GNU C.
                           */
    unsigned int bf2 : 2;
    unsigned char buffer[SIZE20];
} S_Padding;

enum WidgetEnum { WE_W, WE_X, WE_Y, WE_Z } widget_type;

typedef enum _suit { UNKNOWN_SUIT, SPADES, HEARTS, DIAMONDS, CLUBS } suit;

struct SwithArray {
    int t;
    int a[SIZE6];
};
typedef struct _bigstruct_diff {
    int count;
    int values[SIZE20];
} bigstruct_diff;
typedef struct _bigstruct {
    unsigned int count;
    int values[SIZE20];
} bigstruct;

typedef struct {
    int i;
    float f;
} StructWithFloat;

struct flex_array_struct {
    size_t num;
    int data[];
};

typedef struct buf_mem_st {
    char* data;
    size_t max; /* size of buffer */
} BUF_MEM;

typedef void (*ptr_return_type_void)(void);

extern int tab[SIZE100];
extern void do_something_float(float);
extern void initialize_struct_with_float(StructWithFloat*);
extern void process_flexiblearraymemberstruct_byvalue(struct flex_array_struct s);
extern void process_flexiblearraymemberstruct_bypointer(struct flex_array_struct* s);
extern int readint(void);
extern void read_pstruct_diff(bigstruct_diff*);
extern void read_pstruct(bigstruct*);
extern void read_pchar(char*);
extern void read_pint(int*);
extern void read_str(char*);
extern void print_int(int);
extern void print_pint(const int*);
extern void print_str(const char*);

#endif /* GLOBALDEFS_H */
