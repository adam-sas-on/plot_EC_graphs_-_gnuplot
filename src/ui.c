#include <ncurses.h>
#include <stdio.h>
#include "../include/structures.h"
#include "../include/ec_graphs.h"

#define ROW_BEGIN 2

void ui_start(){
	initscr();
	raw();// * so that [enter] does not have to be pressed;
	keypad(stdscr, TRUE);// * to get F1, F2 etc..;
	noecho();
}


void clear_from_NC(const int row_begin, const int col_begin){
	move(row_begin, col_begin);
	clrtobot();
	refresh();
}

void simple_print(const int row, const int col, char *message){
	mvprintw(row, col, message);
}

void printw_equation(struct ec_parameters ec, const char a_or_b){
	int absolute_val, scr_row = 0, scr_col = 0;

	move(ROW_BEGIN, 0);
	clrtoeol();

	mvprintw(ROW_BEGIN, 1, "EC: y^2 = x^3 ");
	printw((ec.sign1 < 0)?"- ":"+ ");
	absolute_val = (ec.a < 0)?(-1*ec.a):ec.a;

	if(a_or_b == 1)
		getyx(stdscr, scr_row, scr_col);
	printw("%d*x ", absolute_val);

	printw((ec.sign2 < 0)?"- ":"+ ");
	absolute_val = (ec.b < 0)?(-1*ec.b):ec.b;

	if(a_or_b == 2)
		getyx(stdscr, scr_row, scr_col);
	printw("%d", absolute_val);

	if(scr_row > 0 || scr_col > 0){
		if(a_or_b == 1){
			absolute_val = (ec.a < 0)?(-1*ec.a):ec.a;
		} else
			absolute_val = (ec.b < 0)?(-1*ec.b):ec.b;

		move(scr_row, scr_col);
		attron(A_BOLD);
		printw("%d", absolute_val);
		attroff(A_BOLD);
	}
}

void print_menu(const int command, struct ec_parameters ec, const int x_validity, const double critical_prop, const char a_or_b){
	double *xs_4_y0 = ec.xs_4_y0;
	int row = ROW_BEGIN + 1;
	const double precision = 1.0/(double)(ec.precision_denominator) * (double)(ec.precision_numerator);

	clear_from_NC(ROW_BEGIN, 0);

	mvprintw(row++, 2, "x for y = 0: %0.6lf", xs_4_y0[0]);
	if( (x_validity&2) ) printw(",  %0.3lf", xs_4_y0[1]);
	if( (x_validity&4) ) printw( ", %0.3lf", xs_4_y0[2]);

	mvprintw(row++, 2, "4*a^3 + 27*b^2 = %0.1lf  (%d, %d)", critical_prop, ec.a, ec.b);
	if(command == 0){
		mvprintw(row++, 1, "Precision:  %0.4lf", precision);
		mvprintw(row++, 1, "Plot EC");
		mvprintw( row , 1, "Exit");
		printw_equation(ec, a_or_b);
	} else if(command == 1){
		printw_equation(ec, 0);
		mvprintw(row + 1 , 1, "Plot EC");
		mvprintw(row + 2, 1, "Exit");

		mvprintw( row , 1, "Precision:  ");
		attron(A_BOLD);
		printw("%0.4lf", precision);
		attroff(A_BOLD);
	} else if(command == 2){
		printw_equation(ec, 0);
		mvprintw( row , 1, "Precision:  %0.4lf", precision);
		mvprintw(row + 2, 1, "Exit");
		attron(A_BOLD);
		mvprintw(row + 1, 1, "Plot EC");
		attroff(A_BOLD);
	} else {
		printw_equation(ec, 0);
		mvprintw(row++, 1, "Precision:  %0.4lf", precision);
		mvprintw(row++, 1, "Plot EC");
		attron(A_BOLD);
		mvprintw( row , 1, "Exit");
		attroff(A_BOLD);
	}

}

int get_input_modify(int *command, const int command_limit, char *a_or_b, struct ec_parameters *ec){
	double precision;
	int c;

	c=getch();

	switch(c){
	  case 127:/*\b*/
	  case KEY_BACKSPACE:
		c = 127;
		break;
	  case KEY_UP:
		if(*command > 0) *command = (*command) - 1;
		break;
	  case KEY_DOWN:
		if(*command < command_limit - 1) *command = (*command) + 1;
		break;
	  case KEY_RIGHT:
		if(*command == 0) *a_or_b = 2;
		else if(*command == 1){
			ec->precision_denominator = 10*(ec->precision_denominator);
		}
		break;
	  case KEY_LEFT:
		if(*command == 0) *a_or_b = 1;
		else if(*command == 1){
			precision = 10.0/(double)(ec->precision_denominator) * (double)(ec->precision_numerator);
			ec->precision_denominator = (ec->precision_denominator) / 10;

			if(ec->precision_denominator == 0){
				ec->precision_numerator = 10*(ec->precision_numerator);
				ec->precision_denominator = 1;
			}
			if(ec->precision_numerator > MAX_PRECISION || precision > MAX_PRECISION){
				ec->precision_numerator = MAX_PRECISION;
				ec->precision_denominator = 1;
			}
		}
		break;
	}

	return c;
}

void wait_for_click(const int key_code){
	int c;
	char run = 1;

	while(run){
		c = getch();
		if(c == key_code) run = 0;
	}
}

void instruction(){
	struct ec_parameters ec;

	ec_init(&ec, -1);
	ec.a = 17;

	mvprintw(0, 4, "EC equation draw - introduction.");

	printw_equation(ec, 1);
	mvprintw(ROW_BEGIN + 1, 3, "Pressing any number or  backspace  changes the value of parameter 'a'.");
	mvprintw(ROW_BEGIN + 2, 3, "Pressing '+'/'-' changes the sign of 'a'.");
	mvprintw(ROW_BEGIN + 3, 3, "Pressing  right-arrow  switches to parameter 'b'.");
	mvprintw(ROW_BEGIN + 4, 3, "Pressing  enter  plots EC function.");

	attron(A_BOLD);
	mvprintw(ROW_BEGIN + 5, 1, "Press enter for next instruction");
	attroff(A_BOLD);
	wait_for_click('\n');


	printw_equation(ec, 2);
	mvprintw(ROW_BEGIN + 1, 3, "Pressing any number or  backspace  changes the value of parameter 'b'.");
	mvprintw(ROW_BEGIN + 2, 3, "Pressing '+'/'-' changes the sign of 'b'.");
	mvprintw(ROW_BEGIN + 3, 3, "Pressing  left-arrow  switches to parameter 'a'.");
	mvprintw(ROW_BEGIN + 4, 3, "Pressing  enter  plots EC function.");

	attron(A_BOLD);
	mvprintw(ROW_BEGIN + 5, 1, "Press enter for next instruction");
	attroff(A_BOLD);
	wait_for_click('\n');


	clear_from_NC(ROW_BEGIN, 0);
	printw_equation(ec, 0);
	mvprintw(ROW_BEGIN + 1, 3, "Pressing any number or  backspace  changes the size of precision.");
	mvprintw(ROW_BEGIN + 2, 3, "Pressing left or right arrow moves decimal point of the size of precision.");

	attron(A_BOLD);
	mvprintw(ROW_BEGIN + 3, 1, "Press enter for next instruction");
	attroff(A_BOLD);
	wait_for_click('\n');


	clear_from_NC(ROW_BEGIN, 0);
	mvprintw( ROW_BEGIN , 3, "Pressing up and down arrows switches command.");
	mvprintw(ROW_BEGIN + 1, 3, "Pressing  enter  executes the command.");
	attron(A_BOLD);
	mvprintw(ROW_BEGIN + 2, 1, "Press enter to start the main program");
	attroff(A_BOLD);
	wait_for_click('\n');
	clear_from_NC(0, 0);
}

void usage(char *app_name){
	printf("%s: application to plot elliptic curves (EC; build date: %s);\n  it requires gnuplot application to be installed.\n", app_name, __DATE__);
	puts("\tOptions:");
	printf("-n (--points) n: set number of points to plot into n < 10000.\n\
	It limits maximum number of points into 2*n but minimum %u. Default n = %u.\n", MINIMUM_LIMIT_FOR_POINTS, POINTS_TO_PLOT);
	puts("-h (--help): prints this help.\n\n\tScreen menu with description");

	puts("\t- pressing up and down arrows switches command;\n\t- pressing  enter  executes the command.\n");
	puts("\"EC: y^2 = x^3 - 48*x + 128\"");
	puts("\t- pressing  right and left arrows switches between parameters 'a' and 'b';\n\
	- pressing any number or  backspace  changes the value of selected parameter;\n\
	- pressing '+'/'-' changes the sign of selected parameter;\n\
	- pressing  enter  plots EC function.");

	puts("  \"x for y = 0: -8.000000,  4.000\"");
	puts("\t- presents x value(s) for point(s) where y = 0.");

	puts("  \"4*a^3 + 27*b^2 = 0.0  (-48, 128)\"");
	puts("\t- computes the value of discriminant parameter for Elliptic Curves.");

	puts(" \"Plot EC\"\n\t- pressing enter plots the equation.");
	puts(" \"Exit\"\n\t- pressing enter exits this application.");
}

int points_to_file(char *file_name, struct ec_parameters ec){
	FILE *fpoints = NULL;
	struct point *points = ec.points;
	unsigned i;

	if(ec.n < 2) return -1;

	fpoints = fopen(file_name, "w");
	if(fpoints == NULL) return -2;

	for(i = 0; i < ec.n; i++){
		fprintf(fpoints, "%0.6lf\t%0.6lf\n", (points+i)->x, (points+i)->y);
	}

	fclose(fpoints);

	return 1;
}

void closeNC(){
	endwin();
}

