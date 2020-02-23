#ifndef STRUCTURES
#define STRUCTURES

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
	unsigned maxPoints;
};

#endif

