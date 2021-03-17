#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/ec_graphs.h"
#include "../include/ui.h"
#include "../include/utils.h"

#define REST_NAME_LENGTH 16

int draw_start(int ac, char**av, unsigned *points_to_plot){
	FILE *gnuplot = NULL;
	int c;
	struct option options[] = {
		{"points", required_argument, NULL, 'n'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	gnuplot = popen("gnuplot", "w");
	if(gnuplot == NULL){
		printf("%s: gnuplot is not supported in this system!", av[0]);
		return 1;
	}

	pclose(gnuplot);

	*points_to_plot = POINTS_TO_PLOT;

	while( (c = getopt_long(ac, av, "n:h", options, NULL) )>=0 ){
		switch(c){
			case 'n':
				c = atoi(optarg);
				if(c <= 0) *points_to_plot = 0;
				else if(c > 10000) *points_to_plot = 10000;
				else *points_to_plot = (unsigned)c;
				break;
			case 'h':
				usage(av[0]);
				return 2;
		}
	}

	ui_start();

	instruction();

	return 0;
}

void run(const unsigned points_to_plot){
	FILE *gplot = popen("gnuplot", "w");
	struct ec_parameters ec;
	double precision;
	char *file_name = NULL;
	unsigned count_points, name_length;
	int c, cmd = 0, validity;//, scr_h;
	char run = 1, a_or_b = 1;

	if(gplot == NULL){
		closeNC();
		return ;
	}
	fprintf(gplot, "set grid back\n");


	ec_init(&ec, points_to_plot);
	precision = ec.precision_numerator / (double)(ec.precision_denominator);


	if(ec.max_points > 1){
		name_length = sizeof_2_strings("temp_", ".txt") + REST_NAME_LENGTH;
		file_name = (char*)calloc(name_length + 1, sizeof(char));

		if(file_name == NULL)
			name_length = 0;
		else
			rand_string(file_name, "temp_", ".txt", name_length);
	} else name_length = 0;

	simple_print(0, 4, "EC equation draw.");

	while(run){
		validity = all_xs_4_y0(ec.xs_4_y0, (double)ec.a, (double)ec.b);
		print_menu(cmd, ec, validity, critical_proportion( (double)ec.a, (double)ec.b), a_or_b);

		c = get_input_modify(&cmd, 4, &a_or_b, &ec);
		precision = 1.0/(double)(ec.precision_denominator) * (double)(ec.precision_numerator);

		switch(c){
		  case 127:
			if(cmd == 0){
				if(a_or_b == 1) ec.a = ec.a/10;
				else ec.b = ec.b/10;
			} else if(cmd == 1){
				ec.precision_numerator /= 10;
				ec.precision_denominator /= 10;
				if(ec.precision_denominator == 0)
					ec.precision_denominator = 1;
			}
			break;
		  case '+':
			if(cmd == 0 && a_or_b == 1){
				ec.a = abs(ec.a);
				ec.sign1 = 1;
			} else if(cmd == 0 && a_or_b == 2){
				ec.b = abs(ec.b);
				ec.sign2 = 1;
			}
			break;
		  case '-':
			if(cmd == 0 && a_or_b == 1){
				ec.a = -1*abs(ec.a);
				ec.sign1 = -1;
			} else if(cmd == 0 && a_or_b == 2){
				ec.b = -1*abs(ec.b);
				ec.sign2 = -1;
			}
			break;
		  case '\n':
			if(cmd == 3) run = 0;
			else {
				count_points = set_symmetric_points(ec.points, ec.n, (double)ec.a, (double)ec.b, precision);
				if(count_points > 0 && name_length > 0){
					qsort(ec.points, ec.n, sizeof(struct point), ecc_compare);
					validity = points_to_file(file_name, ec);
				} else validity = 0;


				fprintf(gplot, "plot sqrt(x**3+%d*x+%d) w l t \"upper EC\", -sqrt(x**3+%d*x+%d) w l t \"bottom EC\"", ec.a, ec.b, ec.a, ec.b);

				if(validity > 0)
					fprintf(gplot, ", '%s' w p pointtype 12 ps 2 linecolor 4 lw 2 t \"\"", file_name);

				fprintf(gplot, ", '-' w p pointtype 8 ps 2 linecolor 3 lw 4 t\"[%0.2lf, 0.0]\"\n%0.3lf 0.0\ne\n", ec.xs_4_y0[0], ec.xs_4_y0[0]);

				fflush(gplot);
			}
			break;
		  default:
			if(c >= '0' && c <= '9'){
				if(cmd == 0){
					if(a_or_b == 1) ec.a = ec.a*10 + ( (int)ec.sign1)*(c - '0');
					else ec.b = ec.b*10 + ( (int)ec.sign2)*(c - '0');
				} else if(cmd == 1){
					ec.precision_numerator = ec.precision_numerator*10 + (c - '0');
					ec.precision_denominator *= 10;
				}
			}
		}
	}

	closeNC();

	ec_free(ec);
	if(name_length){
		remove(file_name);
		free((void*)file_name);
	}

	pclose(gplot);
}

