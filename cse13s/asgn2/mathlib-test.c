# include <stdio.h>
# include <unistd.h> // For getopt().
# include <math.h>   // For M_PI
# include <stdlib.h> // For exit()
# include "mathlib.h"

# define OPTIONS "ascSCTl" // determining flags 

// helped me understand getopt https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html 
 
// making display table and obtaining values for sin
void do_sin () { 
	printf("  %-12s %-16s %-13s %-13s \n", "x","sin", "Library", "Difference");
	printf("  %-12s %-16s %-13s %-13s \n", "-", "------", "-------", "----------");
	for (double i=0; i<=2*M_PI; i+=0.05*M_PI) {
		printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, my_sin(i), sin(i), (sin(i)-my_sin(i)));
	}
	return;
}

// making display table and obtaining values for cos
void do_cos() {
	printf("  %-12s %-16s %-13s %-13s \n", "x","cos", "Library", "Difference");
	printf("  %-12s %-16s %-13s %-13s \n", "-", "------", "-------", "----------");
        for (double i=0; i<=2*M_PI; i+=0.05*M_PI) {
		printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, my_cos(i), cos(i), (cos(i)-my_cos(i)));
	}
	return;
}

// making display table and obtaining values for arcsin
void do_arcsin() {
        printf("  %-12s %-16s %-13s %-13s \n", "x","arcsin", "Library", "Difference");
        printf("  %-12s %-16s %-13s %-13s \n", "-", "------", "-------", "----------");
        for (double i=-1; i<1; i+=0.05) {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, my_arcsin(i), asin(i), (asin(i)-my_arcsin(i)));
        }
	return;
}

// making display table and obtaining values for arccos
void do_arccos() {
        printf("  %-12s %-16s %-13s %-13s \n", "x","arccos", "Library", "Difference");
        printf("  %-12s %-16s %-13s %-13s \n", "-", "------", "-------", "----------");
        for (double i=-1; i<1; i+=0.05) {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, my_arccos(i), acos(i), (acos(i)-my_arccos(i)));
        }
	return;
}

// making display table and obtaining values for arctan
void do_arctan() {
        printf("  %-12s %-16s %-13s %-13s \n", "x","arctan", "Library", "Difference");
        printf("  %-12s %-16s %-13s %-13s \n", "-", "------", "-------", "----------");
        for (double i=1; i<10; i+=0.05) {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, my_arctan(i), atan(i), (atan(i)-my_arctan(i)));
        }
	return;
}

// making display table and obtaining values for log
void do_log() {
        printf("  %-12s %-16s %-13s %-13s \n", "x","log", "Library", "Difference");
        printf("  %-12s %-16s %-13s %-13s \n", "-", "------", "-------", "----------");
        for (double i=1; i<10; i+=0.05) {
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, my_log(i), log(i), (log(i)-my_log(i)));
        }
	return;
}

int main(int argc , char **argv) {
	int opt = 0;
	while ((opt = getopt(argc , argv , OPTIONS)) != -1) {
		switch (opt) {
		case 's': // do sin if -s
			do_sin();
			break; 
		case 'c': // do cos if -c
			do_cos();
			break;
		case 'S': // do arcsin if -S
			do_arcsin();
			break;
		case 'C': // do arccos if -C
			do_arccos();
			break;
		case 'T': // do arctan if -T
			do_arctan();
			break;
		case 'l': // do log if -l
			do_log();
			break; 
		case 'a': // run every function if -a
			do_sin();
			do_cos();
			do_arcsin();
			do_arccos();
			do_arctan();
			do_log();
			exit (1);
		}
	}
	return 0;
}

