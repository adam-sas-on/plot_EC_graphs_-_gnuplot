#include <stdio.h>
#include <stdlib.h>
#include "../include/ec_graphs.h"
#include "../include/ui.h"
#include "../include/utils.h"

#define REST_NAME_LENGTH 16

int draw_start(int ac, char**av){
	FILE *gnuplot = NULL;

	gnuplot = popen("gnuplot", "w");
	if(gnuplot == NULL){
		printf("%s: gnuplot is not supported in this system!", av[0]);
		return 1;
	}

	pclose(gnuplot);

	ui_start();

	instruction();

	return 0;
}

void run(){
	FILE *gplot = popen("gnuplot", "w");
	struct ec_parameters ec;
	char *file_name = NULL;
	unsigned i, count_points, name_length;
	int c, cmd = 0, validity;//, scr_h;
	char run = 1, a_or_b = 1;

	if(gplot == NULL){
		closeNC();
		return ;
	}
	fprintf(gplot, "set grid back\n");


	ec_init(&ec, 101);

	if(ec.n > 1){
		name_length = sizeof_2_strings("temp_", ".txt") + REST_NAME_LENGTH;
		file_name = (char*)calloc(name_length + 1, sizeof(char));

		if(file_name == NULL) name_length = 0;
	} else name_length = 0;

	simple_print(0, 4, "EC equation draw.");

	while(run){
		validity = all_xs_4_y0(ec.xs_4_y0, (double)ec.a, (double)ec.b);
		print_menu(cmd, ec, validity, critical_proportion( (double)ec.a, (double)ec.b), a_or_b);

		c = get_input_modify(&cmd, 3, &a_or_b);

		switch(c){
		  case 127:
			if(cmd == 0){
				if(a_or_b == 1) ec.a = ec.a/10;
				else ec.b = ec.b/10;
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
			if(cmd == 2) run = 0;
			else {
				count_points = set_symmetric_points(ec.points, ec.n, (double)ec.a, (double)ec.b, 2.0);

				fprintf(gplot, "plot sqrt(x**3+%d*x+%d) w l t \"upper EC\", -sqrt(x**3+%d*x+%d) w l t \"bottom EC\"", ec.a, ec.b, ec.a, ec.b);

				for(i = 0; i < count_points; i++){
					fprintf(gplot, ", '-' w p pointtype 12 ps 2 linecolor 4 lw 2 t \"\"");
				}
				fprintf(gplot, "\n");

				for(i = 0; i < count_points; i++){
					fprintf(gplot, "%0.3lf %0.3lf\n", ec.points[i*2], ec.points[i*2 + 1]);
					fprintf(gplot, "e\n");
				}
				fflush(gplot);
			}
			break;
		  default:
			if(c >= '0' && c <= '9' && cmd == 0){
				if(a_or_b == 1) ec.a = ec.a*10 + ( (int)ec.sign1)*(c - '0');
				else ec.b = ec.b*10 + ( (int)ec.sign2)*(c - '0');
			}
		}
	}

	closeNC();

	ec_free(ec);
	if(name_length)
		free((void*)file_name);

	pclose(gplot);
}

