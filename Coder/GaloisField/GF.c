#include "GF.h"

void initialize_tables(Tables *tables, u32 POLY) {
    u32 a;
    int i;
    tables->LOG[15] = 0;
    tables->EXP[15] = 0;
    a = 1;
    for (i = 0; i < 15; ++i, a = next_value(a, 16, POLY)) {
        tables->LOG[tables->EXP[i] = a] = i;
    }
}

u32 next_value(u32 a, u32 Q, u32 POLY) {
    return (a & (Q >> 1)) ? (a << 1) ^ POLY : a << 1;
}

u32 log_value(Tables *tables, u32 a, u32 N) {
    return tables->LOG[a];
}

u32 exp_value(Tables *tables, u32 a, u32 N) {
    return tables->EXP[a];
}

Index index_value(Tables *tables, Value a, u32 N) {
    Index idx;
    idx.i = log_value(tables, a.v, N);
    return idx;
}

Value value_index(Tables *tables, Index a, u32 N) {
    Value val;
    val.v = exp_value(tables, a.i, N);
    return val;
}

int value_equal(Value a, Value b, u32 N) {
    return a.v == b.v;
}

int value_not_equal(Value a, Value b, u32 N) {
    return a.v != b.v;
}

Value value_add(Value a, Value b, u32 N) {
    Value result;
    result.v = a.v ^ b.v;
    return result;
}

Value value_negate(Value a) {
    return a;
}

Value value_subtract(Value a, Value b) {
    return value_add(a, b, 0);
}

Index index_multiply(Index a, Index b, u32 N) {
    Index result;
    u32 tmp = a.i + b.i;
    result.i = (N - a.i <= b.i) ? tmp - N : tmp;
    return result;
}

Value value_multiply(Tables *tables, Value a, Value b, u32 N) {
    Value result;
    result.v = (!a.v || !b.v) ? 0 : value_index(tables, index_multiply(index_value(tables, a, N), index_value(tables, b, N), N), N).v;
    return result;
}

Index rcp_index(Index a, u32 N) {
    Index result;
    result.i = (!a.i) ? 0 : N - a.i;
    return result;
}

Value rcp_value(Tables *tables, Value a, u32 N) {
    Value result;
    result.v = value_index(tables, rcp_index(index_value(tables, a, N), N), N).v;
    return result;
}

Index index_divide(Index a, Index b, u32 N) {
    Index result;
    u32 tmp = a.i - b.i;
    result.i = (a.i < b.i) ? tmp + N : tmp;
    return result;
}

Value value_divide(Tables *tables, Value a, Value b, u32 N) {
    Value result;
    result.v = !a.v ? 0 : value_index(tables, index_divide(index_value(tables, a, N), index_value(tables, b, N), N), N).v;
    return result;
}

Value fma_index_value(Tables *tables, Index a, Index b, Value c, u32 N) {
    return value_add(value_index(tables, index_multiply(a, b, N), N), c, N);
}

Value fma_value_index(Tables *tables, Value a, Index b, Value c, u32 N) {
    return !a.v ? c : value_add(value_index(tables, index_multiply(index_value(tables, a, N), b, N), N), c, N);
}

Value fma_value_value(Tables *tables, Value a, Value b, Value c, u32 N) {
    return (!a.v || !b.v) ? c : value_add(value_index(tables, index_multiply(index_value(tables, a, N), index_value(tables, b, N), N), N), c, N);
}

void exhaustive_test(Tables *tables, uint32_t POLY, uint32_t N) {
    int j, i;
    Value a, b;
		Value result1,result2;
    for (j = 0; j <= N; ++j) {
        a.v = j;
        for (i = 0; i <= N; ++i) {
            b.v = i;
            result1 = value_multiply(tables, a, b, N);
            result2.v = value_index(tables, index_multiply(index_value(tables, a, N), index_value(tables, b, N), N), N).v;
            if (result1.v != result2.v) {
                printf("Multiplication test failed for a = %d, b = %d\n", j, i);
            }
        }
    }
    for (i = 1; i <= N; ++i) {
        a.v = i;
        result1 = rcp_value(tables, a, N);
        result2.v = rcp_value(tables, a, N).v;
        if (result1.v != result2.v) {
            printf("Reciprocal test failed for a = %d\n", i);
        }
    }
}
