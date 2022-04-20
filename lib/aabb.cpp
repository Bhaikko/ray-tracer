#include "./../include/aabb.h"

aabb::aabb(const point3& a, const point3& b)
{
    minimum = a;
    maximum = b;
}

bool aabb::hit(const ray& r, double t_min, double t_max) const
{
    for (int a = 0; a < 3; a++) {
        double t0 = fmin(
            (minimum[a] - r.origin()[a]) / r.direction()[a],
            (maximum[a] - r.origin()[a]) / r.direction()[a]
        );
        
        double t1 = fmax(
            (minimum[a] - r.origin()[a]) / r.direction()[a],
            (maximum[a] - r.origin()[a]) / r.direction()[a]
        );

        t_min = fmax(t0, t_min);
        t_max = fmin(t1, t_max);

        if (t_max <= t_min) {
            return false;
        }
    }

    return true;
}