#include "include/draw_nc.h"


int main(int ac, char**av, char**env){
	int c = draw_start(ac, av);
	if(!c) run();

	return 0;
}

