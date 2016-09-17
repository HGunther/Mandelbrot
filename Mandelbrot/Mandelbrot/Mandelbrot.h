#ifndef Mandelbrot_H
#define Mandelbrot_H
#include <complex>
#include <vector>
#include <array>
#include <thread>
#include "ppmImage.h"

template<typename T>
int Mandelbrot(const std::complex<T> c)
{
	return Mandelbrot(c.real(), c.imag());
}

/**
Takes a complex number and returns how many cycles it took to reject the numbre from the mandelbrot set. The maximum return is 1000.
This code is based on pseudo code from https://en.wikipedia.org/wiki/Mandelbrot_set.
**/
int Mandelbrot(const double r, const double i)
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


image seriesMandelbrot(const int width, const int height)
{
	ppmImage_H::image img = std::vector<std::vector<ppmImage_H::Color>>(height, std::vector<ppmImage_H::Color>(width, Color()));
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			//Map from pixel coordinates to valid mandelbrot area (x:-2.5 to 1, y: -1 to 1)
			double x = (w * 3.5 / float(width + 1)) + float(3.5 / float(width) / 2.0) - 2.5;
			double y = (h * 2 / float(height + 1)) + float(2 / float(width) / 2) - 1;

			int result = int(Mandelbrot_H::Mandelbrot(x, y) / 1000.0 * 255);

			//Coloring scheme based on result value
			if (result * result > 255) //blue is sensitive to high values
				img[h][w].b = 255;
			else
				img[h][w].b = result * result;
			img[h][w].r = int(sqrt(result) * sqrt(255)); //red is sensitive to low values
			img[h][w].g = result; // green makes the picture white instead of purple
		}
	}
	return img;
}

image threadedMethod1( const int width, const int height)
{
	ppmImage_H::image img = std::vector<std::vector<ppmImage_H::Color>>(height, std::vector<ppmImage_H::Color>(width, Color()));
	std::vector<std::vector<std::thread *>> threads = std::vector<std::vector<std::thread * >>(height, std::vector<std::thread * >(width));

	auto calcMandelbrot = [& img](double x, double y, int h, int w)
	{
		ppmImage_H::Color temp;
		int result =  Mandelbrot(x, y); 
		if (result * result > 255)
			temp.b = 255;
		else
			temp.b = result * result;
		temp.r = int(sqrt(result) * sqrt(255));
		temp.g = result;
		img[h][w] = temp;
	};

	//Create threads
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			//Map from pixel coordinates to valid mandelbrot area (x:-2.5 to 1, y: -1 to 1)
			double x = (w * 3.5 / float(width + 1)) + float(3.5 / float(width) / 2.0) - 2.5;
			double y = (h * 2 / float(height + 1)) + float(2 / float(width) / 2) - 1;
			threads[h][w] = new std::thread(calcMandelbrot, x, y, h, w);
		}
	}

	//Wait for threads to finish
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			threads[h][w]->join();
		}
	}

	return img;
}




#endif