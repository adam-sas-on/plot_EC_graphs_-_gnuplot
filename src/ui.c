#include <ncurses.h>
#include <stdio.h>
#include "../include/ec_graphs.h"

#define ROW_BEGIN 2

void ui_start(){
	initscr();
	raw();// * zeby nie trzeba bylo naciskac [enter];
	keypad(stdscr, TRUE);// * to get F1, F2 etc..;
	noecho();
}

//Struct names and typedef's in camelcase: GtkWidget, TrackingOrder; other lower case.
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

	clear_from_NC(ROW_BEGIN, 0);

	mvprintw(row++, 2, "x for y = 0: %0.6lf", xs_4_y0[0]);
	if( (x_validity&2) ) printw(",  %0.3lf", xs_4_y0[1]);
	if( (x_validity&4) ) printw( ", %0.3lf", xs_4_y0[2]);

	mvprintw(row++, 2, "4*a^3 + 27*b^2 = %0.1lf  (%d, %d)", critical_prop, ec.a, ec.b);
	if(command == 0){
		mvprintw(row++, 1, "Plot EC");
		mvprintw( row , 1, "Exit");
		printw_equation(ec, a_or_b);
	} else if(command == 1){
		printw_equation(ec, 0);
		mvprintw(row+1, 1, "Exit");
		attron(A_BOLD);
		mvprintw( row , 1, "Plot EC");
		attroff(A_BOLD);
	} else {
		printw_equation(ec, 0);
		mvprintw(row++, 1, "Plot EC");
		attron(A_BOLD);
		mvprintw( row , 1, "Exit");
		attroff(A_BOLD);
	}

}

int get_input_modify(int *command, const int command_limit, char *a_or_b){
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
		break;
	  case KEY_LEFT:
		if(*command == 0) *a_or_b = 1;
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
	mvprintw( ROW_BEGIN , 3, "Pressing up and down arrows switches command.");
	mvprintw(ROW_BEGIN + 1, 3, "Pressing  enter  executes the command.");
	attron(A_BOLD);
	mvprintw(ROW_BEGIN + 2, 1, "Press enter to start the main program");
	attroff(A_BOLD);
	wait_for_click('\n');
	clear_from_NC(0, 0);
}

void closeNC(){
	endwin();
}

