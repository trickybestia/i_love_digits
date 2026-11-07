#include <math.h>
#include <iostream>

#include "tfloat.h"

TFloat::TFloat() {
    set_tfloat(&this->n, 0.0);
}

TFloat::TFloat(double d) {
    set_tfloat(&this->n, d);
}

// TFloat TFloat::operator+(TFloat b) {
//     TFloat res;

//     sum_tfloat(&(this->n), &(b.n), &(res.n));

//     return res;
// }

TFloat operator+(TFloat a, TFloat b) {
    TFloat res;

    sum_tfloat(&a.n, &b.n, &res.n);

    return res;
}

TFloat operator+(TFloat a, double b) {
    TFloat _b, res;

    set_tfloat(&_b.n, b);

    sum_tfloat(&a.n, &_b.n, &res.n);

    return res;
}

TFloat operator+(double a, TFloat b) {
    TFloat _a, res;

    set_tfloat(&_a.n, a);

    sum_tfloat(&_a.n, &b.n, &res.n);

    return res;    
}

TFloat operator-(TFloat a, TFloat b) {
    TFloat res;

    subtract_tfloat(&a.n, &b.n, &res.n);

    return res;
}

TFloat operator-(TFloat a, double b) {
    TFloat _b, res;

    set_tfloat(&_b.n, b);

    subtract_tfloat(&a.n, &_b.n, &res.n);

    return res;
}

TFloat operator-(double a, TFloat b) {
    TFloat _a, res;

    set_tfloat(&_a.n, a);

    subtract_tfloat(&_a.n, &b.n, &res.n);

    return res;    
}

TFloat operator*(TFloat a, TFloat b) {
    TFloat res;

    multiply_tfloat(&a.n, &b.n, &res.n);

    return res;
}

TFloat operator*(TFloat a, double b) {
    TFloat _b, res;

    set_tfloat(&_b.n, b);

    multiply_tfloat(&a.n, &_b.n, &res.n);

    return res;
}

TFloat operator*(double a, TFloat b) {
    TFloat _a, res;

    set_tfloat(&_a.n, a);

    multiply_tfloat(&_a.n, &b.n, &res.n);

    return res;    
}

TFloat operator/(TFloat a, TFloat b) {
    TFloat res;

    divide_tfloat(&a.n, &b.n, &res.n);

    return res;
}

TFloat operator/(TFloat a, double b) {
    TFloat _b, res;

    set_tfloat(&_b.n, b);

    divide_tfloat(&a.n, &_b.n, &res.n);

    return res;
}

TFloat operator/(double a, TFloat b) {
    TFloat _a, res;

    set_tfloat(&_a.n, a);

    divide_tfloat(&_a.n, &b.n, &res.n);

    return res;    
}

// TFloat TFloat::operator-(TFloat b) {
//     TFloat res;

//     subtract_tfloat(&(this->n), &(b.n), &(res.n));

//     return res;    
// }

// TFloat TFloat::operator*(TFloat b) {
//     TFloat res;

//     multiply_tfloat(&(this->n), &(b.n), &(res.n));
    
//     return res;
// }

// TFloat TFloat::operator/(TFloat b) {
//     TFloat res;

//     divide_tfloat(&(this->n), &(b.n), &(res.n));
    
//     return res;    
// }

TFloat& TFloat::operator=(double b) {
    set_tfloat(&(this->n), b);

    return *this;
}

TFloat TFloat::operator+=(double b) {
    tfloat _b, res;
    set_tfloat(&_b, b);

    sum_tfloat(&(this->n), &_b, &res);

    this->n = res;

    return *this;
}

TFloat TFloat::operator-=(double b) {
    tfloat _b, res;
    set_tfloat(&_b, b);

    subtract_tfloat(&(this->n), &_b, &res);

    this->n = res;

    return *this;
}

TFloat TFloat::operator*=(double b) {
    tfloat _b, res;
    set_tfloat(&_b, b);

    multiply_tfloat(&(this->n), &_b, &res);

    this->n = res;

    return *this;
}

TFloat TFloat::operator/=(double b) {
    tfloat _b, res;
    set_tfloat(&_b, b);

    divide_tfloat(&(this->n), &_b, &res);

    this->n = res;

    return *this;
}
