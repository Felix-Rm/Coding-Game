#include "helpers.h"

void float_mod(float &v, float m)
{
    if (v > 0)
        while (v >= m)
            v -= m;
    if (v < 0)
        while (v <= m)
            v += m;
}