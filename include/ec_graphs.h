#ifndef EC_GRAPHS
#define EC_GRAPHS

struct ec_parameters {
	int a;
	int b;
	char sign1;
	char sign2;
	double *points;// [n];
	unsigned n;
};

void ec_init(struct ec_parameters *ec);
//void ec_y_square(const double x, const double a, const double b);
double x_4_y0(const double a, const double b);
double critical_proportion(const double a, const double b);

#endif

