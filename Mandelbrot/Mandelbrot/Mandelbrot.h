#ifndef Mandelbrot_H
#define Mandelbrot_H
#include <complex>

template<typename T>
int Mandelbrot(std::complex<T> c)
{
	return Mandelbrot(c.real(), c.imag());
}

/**
Takes a complex number and returns how many cycles it took to reject the numbre from the mandelbrot set. The maximum return is 1000.
This code is based on pseudo code from https://en.wikipedia.org/wiki/Mandelbrot_set.
**/
int Mandelbrot(double r, double i)
{
	if (r <= -2.5 || r > 1)
		return 0;
	if (i <= -1 || i >= 1)
		return 0;
	double x = 0.0;
	double y = 0.0;
	int itr = 0;
	while ( itr < 1000 && x*x + y*y < 2 * 2)
	{
		double xtemp = x*x - y*y + r;
		y = 2 * x*y + i;
		x = xtemp;
		itr++;
	}
	return itr;
}

#endif