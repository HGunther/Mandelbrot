#ifndef Mandelbrot_H
#define Mandelbrot_H
#include <complex>

template<typename T>
int Mandelbrot(std::complex<T> c)
{
	return Mandelbrot(c.real(), c.imag());
}


int Mandelbrot(double r, double i)
{
	return int(r + i);
}

#endif