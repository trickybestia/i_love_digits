#ifndef T_H
#define T_H

#ifndef INTEGER_PART_WIDTH
#define INTEGER_PART_WIDTH 24
#endif

#ifndef FRACTION_PART_WIDTH
#define FRACTION_PART_WIDTH 12
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {ERR_TFLOAT_SUCCESS, ERR_TFLOAT_DIVZERO, 
ERR_TFLOAT_OVERFLOW} tfloat_err_t;

typedef struct tfloat {
    signed char sign;
    unsigned int integer;
    unsigned int fraction;
} tfloat;

static inline double mypow (int base, int power) {
    double res = 1.0;

    if (power > 0)
        for (int i = 0; i < power; i++)
            res = res * (double)base;

    else if (power < 0)
        for (int i = 0; i < (power * -1); i++)
            res = res / (double)base;

    return res;
}

#define MASK (1U << (FRACTION_PART_WIDTH - 1))
static inline double todouble_tfloat(const tfloat* res) {
    double n = 0, d = 0.5;
    unsigned int f = res->fraction;

    n = res->integer;

    for (int i = 0; i < FRACTION_PART_WIDTH; i++) {
        n = n + (((f & MASK) >> (FRACTION_PART_WIDTH - 1)) * d);

        f = f << 1;

        d = d / 2;
    }

    n = n * res->sign;

    return n;
}

static inline void set_tfloat(tfloat* res, double n) {
    unsigned int integer_part, shift = FRACTION_PART_WIDTH - 1;
    double fractional_part, d = 0.5;

    res->integer = 0;
    res->fraction = 0;

    res->sign = (n >= 0.0) ? 1 : -1;
    n = (n < 0.0) ? (n * -1.0) : n;

    integer_part = (int)n;
    fractional_part = n - (double)integer_part;

    /* No need to change anything */
    res->integer = integer_part;

    while (fractional_part > 0 && shift > 0) {
        if (fractional_part >= d) {
            res->fraction = res->fraction | (1U << shift);
            fractional_part = fractional_part - d;
        }

        d = d / 2;
        shift = shift - 1;
    }
}

static inline tfloat_err_t sum_tfloat(const tfloat* a, const tfloat* b, tfloat* res) {
    signed long al, bl, resl;

    res->integer = 0;
    res->fraction = 0;

    al = (((signed long)a->integer << FRACTION_PART_WIDTH) +
          (a->fraction)) * a->sign;
    bl = (((signed long)b->integer << FRACTION_PART_WIDTH) +
          (b->fraction)) * b->sign;

    resl = al + bl;

    res->sign = (resl >= 0) ? 1 : -1;
    resl = (resl < 0) ? (resl * -1) : resl;

    res->integer = (resl >> FRACTION_PART_WIDTH) &
        ((unsigned int)mypow(2, INTEGER_PART_WIDTH) - 1);
    res->fraction = (resl) &
        ((unsigned int)mypow(2, FRACTION_PART_WIDTH) - 1);

    return ERR_TFLOAT_SUCCESS;
}

static inline tfloat_err_t subtract_tfloat (const tfloat* a, const tfloat* b,
                                     tfloat* res) {
    signed long al, bl, resl;

    res->integer = 0;
    res->fraction = 0;

    al = (((signed long)a->integer << FRACTION_PART_WIDTH)
          + (a->fraction)) * a->sign;
    bl = (((signed long)b->integer << FRACTION_PART_WIDTH)
          + (b->fraction)) * b->sign;

    resl = al - bl;

    res->sign = (resl >= 0) ? 1 : -1;
    resl = (resl < 0) ? (resl * -1) : resl;

    res->integer = (resl >> FRACTION_PART_WIDTH)
        & ((unsigned int)mypow(2, INTEGER_PART_WIDTH) - 1);
    res->fraction = (resl) &
        ((unsigned int)mypow(2, FRACTION_PART_WIDTH) - 1);

    return ERR_TFLOAT_SUCCESS;
}

static inline tfloat_err_t multiply_tfloat(const tfloat* a, const tfloat* b,
                                    tfloat* res) {
    __int128 al = 0, bl = 0;
    __int128 resl = 1;

    res->integer = 0;
    res->fraction = 0;
    res->sign = a->sign * b->sign;

    al = ((signed long)a->integer << FRACTION_PART_WIDTH) + (a->fraction);
    bl = ((signed long)b->integer << FRACTION_PART_WIDTH) + (b->fraction);

    resl = (al * bl) >> FRACTION_PART_WIDTH;

    res->integer = (resl >> FRACTION_PART_WIDTH) &
        ((unsigned int)mypow(2, INTEGER_PART_WIDTH) - 1);
    res->fraction = (resl) &
        ((unsigned int)mypow(2, FRACTION_PART_WIDTH) - 1);

    return ERR_TFLOAT_SUCCESS;
}

static inline tfloat_err_t divide_tfloat(const tfloat* a, const tfloat* b,
                                  tfloat* res) {
    __int128 al = 0, bl = 0;
    __int128 resl = 1;

    res->integer = 0;
    res->fraction = 0;
    res->sign = a->sign * b->sign;

    al = ((signed long)a->integer << FRACTION_PART_WIDTH) + (a->fraction);
    bl = ((signed long)b->integer << FRACTION_PART_WIDTH) + (b->fraction);

    if (bl == 0) {
        return ERR_TFLOAT_DIVZERO;
    }

    resl = ((al << FRACTION_PART_WIDTH) / bl);

    res->integer = (resl >> FRACTION_PART_WIDTH) &
        ((unsigned int)mypow(2, INTEGER_PART_WIDTH) - 1);
    res->fraction = (resl) &
        ((unsigned int)mypow(2, FRACTION_PART_WIDTH) - 1);

    return ERR_TFLOAT_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#endif
