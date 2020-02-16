#ifndef EC_GRAPHS
#define EC_GRAPHS

void ec_init(struct ec_parameters *ec, const int memSize);
//void ec_y_square(const double x, const double a, const double b);
double x_4_y0(const double a, const double b);
int all_xs_4_y0(double *xs/*[3]*/, const double a, const double b);
double critical_proportion(const double a, const double b);
unsigned set_symmetric_points(struct point *points/*[n]*/, const unsigned n, const double a, const double b, const double precision);
void ec_free(struct ec_parameters ec);

#endif

