#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

class perlin
{
    private:
        static const int point_count = 256;
        double* ranfloat;
        int* perm_x;
        int* perm_y;
        int* perm_z;

        static int* perlin_generate_perm();
        static void permute(int* p, int n);

    public:
        perlin();
        ~perlin();

        double noise(const point3& p) const;
};

#endif