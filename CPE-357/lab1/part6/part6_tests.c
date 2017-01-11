#include <stdio.h>

#include "checkit.h"
#include "part6.h"
#include "part6.c"

/* define testing functions */
void test_create_point1(void)
{
   struct point p = create_point(1.9, -2.7);

   checkit_double(p.x, 1.9);
   checkit_double(p.y, -2.7);
}

void test_create_point2(void)
{
   struct point p = create_point(0.2, 12.1);

   checkit_double(p.x, 0.2);
   checkit_double(p.y, 12.1);
}

void test_create_rectangle_positive_points(void) {
    struct point top_left = create_point(0.0, 4.0);
    struct point bottom_right = create_point(4.0, 0.0);
    struct rectangle r = create_rectangle(top_left, bottom_right);

    checkit_double(r.top_left.x, 0.0);
    checkit_double(r.top_left.y, 4.0);
    checkit_double(r.bottom_right.x, 4.0);
    checkit_double(r.bottom_right.y, 0.0);
}

void test_create_rectangle_mixed_points(void) {
    struct point top_left = create_point(-1.0, 1.0);
    struct point bottom_right = create_point(1.0, -1.0);
    struct rectangle r = create_rectangle(top_left, bottom_right);

    checkit_double(r.top_left.x, -1.0);
    checkit_double(r.top_left.y, 1.0);
    checkit_double(r.bottom_right.x, 1.0);
    checkit_double(r.bottom_right.y, -1.0);
}

void test_is_a_square_with_square_rectangle(void) {
    struct point top_left = create_point(0.0, 4.0);
    struct point bottom_right = create_point(4.0, 0.0);
    struct rectangle r = create_rectangle(top_left, bottom_right);

    checkit_int(is_a_square(r), 1);
}

void test_is_not_a_square(void) {
    struct point top_left = create_point(0.0, 4.0);
    struct point bottom_right = create_point(3.9, 0.0);
    struct rectangle r = create_rectangle(top_left, bottom_right);

    checkit_int(is_a_square(r), 0);
}

void test_is_a_square_with_no_width_and_height(void) {
    struct point top_left = create_point(0.0, 0.0);
    struct point bottom_right = create_point (0.0, 0.0);
    struct rectangle r = create_rectangle(top_left, bottom_right);

    checkit_int(is_a_square(r), 1);
}

void test_create_point(void)
{
   test_create_point1();
   test_create_point2();
}

void test_create_rectangle(void) {
    test_create_rectangle_positive_points();
    test_create_rectangle_mixed_points();
}

void test_is_a_square(void) {
    test_is_a_square_with_square_rectangle();
    test_is_not_a_square();
    test_is_a_square_with_no_width_and_height();
}

int main(int arg, char *argv[])
{
   /* call testing function(s) here */
   test_create_point();
   test_create_rectangle();
   test_is_a_square();

   return 0;
}
