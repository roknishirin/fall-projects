# include <stdio.h>
# include <math.h>      // for M_PI
# include <assert.h>    // for square root function

# include "mathlib.h"

# define EPSILON  1E-10 // defining epsilon
 
double my_abs(double x) {              // defining absolute value function 
        int y = 0;
        if (x < y) { return (-1) * (x);} else { return x; }
}

double my_sqrt(double x) {            // defining sqare root function
        assert(x >= 0);		      // obtained from piazza post #150 
        double f = 1.0,
               y = 1.0;
        while (x >  4) {
                x /= 4.0;
                f *= 2.0;
        }
        for (double guess = 0.0; my_abs(y - guess) > EPSILON; y = (y + x / y) / 2.0) {
                guess = y;
        }
        return f * y;
} 

double my_sin(double x) {	      // defining sin function
	double total=x, num=x, denom=1, previous;
	int de_var=1;		      // used image from https://en.wikipedia.org/wiki/Taylor_series to look at expanded taylor series
	previous = (num/denom);       // equation derived from taylor series
	while (my_abs(previous) > EPSILON){
		num = (-1) * (num) * x * x;
		denom = (denom) * (de_var + 1) * (de_var + 2);
		previous = (num/denom); 
		total += previous;
		de_var += 2;
	}
	return total;
}

double my_cos(double x) {             // defining cos function
	double total=1, num=1, denom=1, previous;
	int de_var=1;		      // used image from https://en.wikipedia.org/wiki/Taylor_series to look at expanded taylor series 
	previous = (num/denom);       // equation derived from taylor series
	while (my_abs(previous) > EPSILON) {
		num = (-1) * x * x * (num);
		denom = (denom) * (2 * de_var) * (2 * de_var - 1);
		previous = (num/denom);
		total += previous;
		de_var += 1;
	}
	return total;
}

double my_arcsin(double x) {         // defining arcsin function 
	double previous=0, current = x;
	int i=1;                     // obtained the equation from piazza post #155
	if (x < 0) {x *= (-1); i  = (-1);}
	while (my_abs((my_sin(previous) - x)/my_cos(previous)) > EPSILON) {
		current = previous - ((my_sin(previous)-x)/my_cos(previous));
		previous = current;
	}
	return (previous * i);

}

double my_arccos(double x) {       // defining arccos function
	return ((M_PI / 2) - my_arcsin(x));
}	

double my_arctan(double x) {       // defining arctan function
	return (my_arccos(1/(my_sqrt((x * x) + 1))));
}

static double Exp(double x) {     // obtained from asgn 2 
	double t = 1.0;
	double y = 1.0;
	for (double k = 1.0; t > EPSILON; k += 1.0) {
		t *= x / k;
		y += t;
	}
	return y;
}

double my_log(double x) {         // equation explained by tutor lev
	int excess=0;
	double total=1, e=Exp(1);
	while (x > e) {           // normalizing since log wouldn't work over exp(1) threshold
		x/=e;
		excess += 1;
	}	
	double diff=(x - Exp(total))/Exp(total);  // following Newton-Raphson method
	while (my_abs(x - Exp(total)) > EPSILON) {	
		total+= diff ;
		diff=(((x - Exp(total))/Exp(total)));
	}
	return (total + excess);
}
