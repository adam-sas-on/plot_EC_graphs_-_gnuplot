#include "include/draw_nc.h"


int main(int ac, char**av, char**env){
	unsigned points_to_plot;
	int c = draw_start(ac, av, &points_to_plot);
	if(!c) run(points_to_plot);

	return 0;
}

