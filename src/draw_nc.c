#include <stdio.h>
#include <stdlib.h>
#include "../include/ec_graphs.h"
#include "../include/ui.h"


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
	double x_for_y0;
	int c, cmd = 0;//, scr_h;
	char run = 1, a_or_b = 1;

	ec_init(&ec);

	if(gplot == NULL){
		closeNC();
		return ;
	}

	simple_print(0, 4, "EC equation draw.");

	while(run){
		x_for_y0 = x_4_y0( (double)ec.a, (double)ec.b);
		print_menu(cmd, ec, x_for_y0, critical_proportion( (double)ec.a, (double)ec.b), a_or_b);

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
				fprintf(gplot, "plot sqrt(x**3+%d*x+%d) w l t \"upper EC\", -sqrt(x**3+%d*x+%d) w l t \"bottom EC\"\n", ec.a, ec.b, ec.a, ec.b);
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
	pclose(gplot);
}

