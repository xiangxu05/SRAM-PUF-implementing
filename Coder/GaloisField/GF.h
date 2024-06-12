#ifndef _GF_H
#define _GF_H

#include "system.h"
#include "usart.h"
#define Q(M) (1 << (M))
#define N(M) (Q(M) - 1)
#define POLY_CHECK(M, POLY) (((1 << (M)) & ~((1 << (M)) - 1)) == (POLY))

typedef struct {
    u32 v;
} Value;

typedef struct {
    u32 i;
} Index;

typedef struct {
    u32 LOG[16];
    u32 EXP[16];
} Tables;

void initialize_tables(Tables *tables, u32 POLY);
u32 next_value(u32 a, u32 Q, u32 POLY);
u32 log_value(Tables *tables, u32 a, u32 N);
u32 exp_value(Tables *tables, u32 a, u32 N);
Index index_value(Tables *tables, Value a, u32 N);
Value value_index(Tables *tables, Index a, u32 N);
int value_equal(Value a, Value b, u32 N);
int value_not_equal(Value a, Value b, u32 N);
Value value_add(Value a, Value b, u32 N);
Value value_negate(Value a);
Value value_subtract(Value a, Value b);
Index index_multiply(Index a, Index b, u32 N);
Value value_multiply(Tables *tables, Value a, Value b, u32 N);
Index rcp_index(Index a, u32 N);
Value rcp_value(Tables *tables, Value a, u32 N);
Index index_divide(Index a, Index b, u32 N);
Value value_divide(Tables *tables, Value a, Value b, u32 N);
Value fma_index_value(Tables *tables, Index a, Index b, Value c, u32 N);
Value fma_value_index(Tables *tables, Value a, Index b, Value c, u32 N);
Value fma_value_value(Tables *tables, Value a, Value b, Value c, u32 N);

void exhaustive_test(Tables *tables, u32 POLY, u32 N);
#endif
