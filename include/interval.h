#pragma once

#include "common.h"

class Interval {
  public:
    double min, max;

    Interval(): min(+infinity), max(-infinity) {}
    Interval(double min, double max): min(min), max(max) {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }
};

const static Interval empty   (+infinity, -infinity);
const static Interval universe(-infinity, +infinity);