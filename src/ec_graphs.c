#include "../include/ec_graphs.h"
#include <math.h>


void ec_init(struct ec_parameters *ec){
	ec->a = 486662;
	ec->b = 1;
	ec->sign1 = ec->sign2 = 1;
	ec->points = 0;
	ec->n = 0;
}

double ec_y_square(const double x, const double a, const double b){
	return x*x*x + a*x + b;
}

// x_4_y0: what is the value of x on the head of elliptic curve (meaning y = 0);
double x_4_y0(const double a, const double b){
	double sqr, d, pi2_per_3, dumm;

	sqr = b*b/4.0;
	dumm = a*a*a/27.0;
	d = sqr + dumm;

	if(d > 0.0){
		sqr = sqrt(d);
		dumm = -0.5*b;
		d = cbrt(dumm + sqr) + cbrt(dumm - sqr);
	} else if(d < 0.0){
		d = 1.5*b/a*sqrt(-3.0/a);
		dumm = acos(d)/3.0;
		pi2_per_3 = atan(1.0)*8.0/3.0;// = \pi*2/3;
		sqr = sqrt(a/-3.0)*2.0;

		d = sqr*cos(dumm);

		dumm -= pi2_per_3;
		if(sqr*cos(dumm) < d) d = sqr*cos(dumm);

		dumm -= pi2_per_3;
		if(sqr*cos(dumm) < d) d = sqr*cos(dumm);
	} else {
		sqr = 3.0*b/a;
		dumm = -1.5*b/a;
		d = (sqr < dumm)?sqr:dumm;// min(sqr, dumm);
	}
	return d;
}

double critical_proportion(const double a, const double b){
	return a*a*a*4.0 + b*b*27.0;// hope != 0;
}

void set_symmetric_points(double *points/*[n]*/, const unsigned n, const double a, const double b, const double precision){
	unsigned i;

	if(n < 1) return;

	if( (n&1) ) i = n/2;
	else i = n/2 - 1;

	points[i] = x_4_y0(a, b);
// TODOs: x_4_y0() on the middle of array points[], and then upper and lower points by step = precision;
}

