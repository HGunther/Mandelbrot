#ifndef Mandelbrot_H
#define Mandelbrot_H
#include <complex>
#include "ppmImage.h"

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


image seriesMandelbrot(int width, int height)
{
	ppmImage_H::image img;
	for (int h = 0; h < height; h++)
	{
		std::vector<ppmImage_H::Color> v{};
		for (int w = 0; w < width; w++)
		{
			ppmImage_H::Color temp;

			//Map from pixel coordinates to valid mandelbrot area (x:-2.5 to 1, y: -1 to 1)
			double x = (w * 3.5 / float(width + 1)) + float(3.5 / float(width) / 2.0) - 2.5;
			double y = (h * 2 / float(height + 1)) + float(2 / float(width) / 2) - 1;

			int result = int(Mandelbrot_H::Mandelbrot(x, y) / 1000.0 * 255);

			//Coloring scheme based on result value
			if (result * result > 255) //blue is sensitive to high values
				temp.b = 255;
			else
				temp.b = result * result;
			temp.r = int(sqrt(result) * sqrt(255)); //red is sensitive to low values
			temp.g = result; // green makes the picture white instead of purple

			v.push_back(temp);
		}
		img.push_back(v);
	}
	return img;
}

auto timeSeriesMandelbrot(int n, int width, int height)
{
	std::vector<double> times{};
	auto x = [width, height]() {return seriesMandelbrot(width, height); };
	for (int i = 0; i < n; i++)
	{
		double nseconds = double(TimeF_H::timeFunction(x).count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
		times.push_back(nseconds);
	}
	return times;
}

image threadedMandelbrot(int width, int height)
{
	ppmImage_H::image img;
	for (int h = 0; h < height; h++)
	{
		std::vector<ppmImage_H::Color> v{};
		for (int w = 0; w < width; w++)
		{
			ppmImage_H::Color temp;

			//Map from pixel coordinates to valid mandelbrot area (x:-2.5 to 1, y: -1 to 1)
			double x = (w * 3.5 / float(width + 1)) + float(3.5 / float(width) / 2.0) - 2.5;
			double y = (h * 2 / float(height + 1)) + float(2 / float(width) / 2) - 1;

			int result = int(Mandelbrot_H::Mandelbrot(x, y) / 1000.0 * 255);

			//Coloring scheme based on result value
			if (result * result > 255) //blue is sensitive to high values
				temp.b = 255;
			else
				temp.b = result * result;
			temp.r = int(sqrt(result) * sqrt(255)); //red is sensitive to low values
			temp.g = result; // green makes the picture white instead of purple

			v.push_back(temp);
		}
		img.push_back(v);
	}
	return img;
}

auto timeThreadedMandelbrot(int n, int width, int height)
{
	std::vector<double> times{};
	auto x = [width, height]() {return seriesMandelbrot(width, height); };
	for (int i = 0; i < n; i++)
	{
		double nseconds = double(TimeF_H::timeFunction(x).count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
		times.push_back(nseconds);
	}
	return times;
}





#endif