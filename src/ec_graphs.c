#include "../include/ec_graphs.h"
#include <stdlib.h>
#include <math.h>

#define SWAP(a, b) dumm=(a); (a)=(b); (b)=dumm

void ec_init(struct ec_parameters *ec, const int memSize){
	ec->a = 486662;
	ec->b = 1;
	ec->sign1 = ec->sign2 = 1;
	ec->xs_4_y0 = NULL;
	ec->points = NULL;
	ec->n = 0;

	if(memSize >= 0){// if we want to alloc memory;
		ec->xs_4_y0 = (double*)calloc(memSize + 3, sizeof(double));
		if(ec->xs_4_y0 != NULL){
			ec->points = (ec->xs_4_y0) + 3;
			ec->n = (unsigned)memSize;
		}
	}
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

// all_xs_4_y0: same as above but it writes all 3 values into xs[] and returns binary check which are important {0, 1, 3, 7};
int all_xs_4_y0(double *xs/*[3]*/, const double a, const double b){
	double sqr, pi2_per_3, dumm;
	int validity = 0;// significance;

	sqr = b*b/4.0;
	dumm = a*a*a/27.0;
	xs[0] = sqr + dumm;

	if(xs[0] > 0.0){
		sqr = sqrt(xs[0]);
		dumm = -0.5*b;
		xs[0] = cbrt(dumm + sqr) + cbrt(dumm - sqr);
		validity = 1;
	} else if(xs[0] < 0.0){
		xs[0] = 1.5*b/a*sqrt(-3.0/a);
		dumm = acos(xs[0])/3.0;
		pi2_per_3 = atan(1.0)*8.0/3.0;// = \pi*2/3;
		sqr = sqrt(a/-3.0)*2.0;

		xs[0] = sqr*cos(dumm);

		dumm -= pi2_per_3;
		xs[1] = sqr*cos(dumm);

		dumm -= pi2_per_3;
		xs[2] = sqr*cos(dumm);

		if(xs[0] > xs[1]){// swap;
			SWAP(xs[0], xs[1]);
		}
		if(xs[0] > xs[2]){
			SWAP(xs[0], xs[2]);
		}
		if(xs[1] > xs[2]){
			SWAP(xs[1], xs[2]);
		}
		validity = 7;
	} else {
		xs[0] = -1.5*b/a;
		xs[1] = 3.0*b/a;
		if(xs[1] < xs[0]){
			SWAP(xs[0], xs[1]);
		}
		validity = 3;
	}

	return validity;
}

double critical_proportion(const double a, const double b){
	return a*a*a*4.0 + b*b*27.0;// hope != 0;
}

unsigned set_symmetric_points(double *points/*[n]*/, const unsigned n, const double a, const double b, const double precision){
	double xs[3], b2 = b, ystep = 0.0;
	unsigned i = 1, j, validity, count_xs;

	if(n < 1) return 0;

	validity = all_xs_4_y0(xs/*[3]*/, a, b2);
	count_xs = 1;
	if( (validity&2) ) count_xs++;
	if( (validity&4) ) count_xs++;
	for(j = 0; j < count_xs && i < n; j++, i+=2){
		points[i-1] = xs[j];
		points[ i ] = 0.0;
	}

	while(i < n){
		ystep += precision;
		b2 = b - ystep*ystep;

		validity = all_xs_4_y0(xs, a, b2);
		count_xs = 1;
		if( (validity&2) ) count_xs++;
		if( (validity&4) ) count_xs++;

		j = 0;
		while(j < count_xs && i < n){
			points[i-1] = xs[j];
			points[ i ] = ystep;
			i += 2;

			if(i < n){
				points[i-1] = xs[j];
				points[ i ] = -1.0*ystep;
				i += 2;
			}

			j++;
		}
	}

	return (i - 1)/2;
}

void ec_free(struct ec_parameters ec){
	if(ec.xs_4_y0 != NULL) free((void*)(ec.xs_4_y0) );
}

