#ifndef UI
#define UI

void ui_start();
void simple_print(const int row, const int col, char *message);

void print_menu(const int command, struct ec_parameters ec, const int x_validity, const double critical_prop, const char a_or_b);
int get_input_modify(int *command, const int command_limit, char *a_or_b, struct ec_parameters *ec);
void instruction();
void usage(char *app_name);
int points_to_file(char *file_name, struct ec_parameters ec);
void closeNC();

#endif

