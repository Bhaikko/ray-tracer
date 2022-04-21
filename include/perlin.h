#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

class perlin
{
    private:
        static const int point_count = 256;
        vec3* ranvec;
        int* perm_x;
        int* perm_y;
        int* perm_z;

        static int* perlin_generate_perm();
        static void permute(int* p, int n);
        static double perlin_interp(vec3 c[2][2][2], double u, double v, double w);

    public:
        perlin();
        ~perlin();

        double noise(const point3& p) const;
};

#endif