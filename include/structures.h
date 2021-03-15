#ifndef STRUCTURES
#define STRUCTURES

#define MINIMUM_LIMIT_FOR_POINTS 200
#define POINTS_TO_PLOT 91
#define MAX_PRECISION 20.0

struct point {
	double x;
	double y;
};

struct ec_parameters {
	int a;
	int b;
	char sign1;
	char sign2;
	double *xs_4_y0;// [3];
	struct point *points;// [maxPoints];
	unsigned n;
	unsigned max_points;
	unsigned precision_numerator;
	unsigned precision_denominator;
};

#endif

