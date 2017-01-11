#include "part6.h"
/*
struct point {
    double x;
    double y;
};

struct rectangle {
    struct point top_left;
    struct point bottom_right;
};
*/
struct point create_point(double x, double y)
{
   struct point p;
   p.x = x;
   p.y = y;
   return p;
}

struct rectangle create_rectangle(struct point p1, struct point p2) {
    struct rectangle r;
    r.top_left = p1;
    r.bottom_right = p2;
    return r;
}

int is_a_square(struct rectangle x) {
    double width = x.bottom_right.x - x.top_left.x;
    double height = x.top_left.y - x.bottom_right.y;
    if (width == height) {
        return 1;
    } else {
        return 0;
    }
}
