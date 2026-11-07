#pragma once

#include <iostream>

#define INTEGER_PART_WIDTH 24
#define FRACTION_PART_WIDTH 24

#include "t.h"

struct TFloat {
    tfloat n;

    TFloat();
    /* explicit чтобы не иметь ambiguous overload */
    TFloat(double d);

    TFloat& operator=(double b);

    TFloat operator+=(double b);
    TFloat operator-=(double b);
    TFloat operator*=(double b);
    TFloat operator/=(double b);

    operator double() const {return todouble_tfloat(&(this->n));}

    friend std::ostream& operator<<(std::ostream& os, const TFloat& a) {
        double d = todouble_tfloat(&a.n);
        os << d;
        return os;
    }

    friend std::istream& operator>>(std::istream &in, TFloat &a)
    {
        double d;
        in >> d;

        set_tfloat(&(a.n), d);

        return in;
    }
};

TFloat operator+(TFloat a, TFloat b);
TFloat operator+(TFloat a, double b);
TFloat operator+(double a, TFloat b);

TFloat operator-(TFloat a, TFloat b);
TFloat operator-(TFloat a, double b);
TFloat operator-(double a, TFloat b);

TFloat operator*(TFloat a, TFloat b);
TFloat operator*(TFloat a, double b);
TFloat operator*(double a, TFloat b);

TFloat operator/(TFloat a, TFloat b);
TFloat operator/(TFloat a, double b);
TFloat operator/(double a, TFloat b);
