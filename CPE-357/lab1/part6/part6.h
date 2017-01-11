#ifndef PART5_H
#define PART5_H

struct point
{
   double x;
   double y;
};

struct rectangle
{
   point top_left;
   point bottom_right;
};

struct point create_point(double x, double y);
struct point is_a_square(rectangle x);

#endif
