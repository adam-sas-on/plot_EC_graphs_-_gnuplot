#ifndef UI
#define UI

void ui_start();
void simple_print(const int row, const int col, char *message);

void print_menu(const int command, struct ec_parameters ec, const double x_for_y0, const double critical_prop, const char a_or_b);
int get_input_modify(int *command, const int command_limit, char *a_or_b);
void instruction();
void closeNC();

#endif

