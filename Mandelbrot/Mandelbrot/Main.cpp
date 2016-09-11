#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include "TimeF.h"
#include "Mandelbrot.h"
#include "ppmImage.h"



image generateMandelbrot(int width, int height)
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

auto timeMandelbrot(int n, int width, int height)
{
	std::vector<double> times {};
	auto x = [width, height]() {return generateMandelbrot(width, height); };
	for (int i = 0; i < n; i++)
	{
		double nseconds = double(TimeF_H::timeFunction(x).count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
		times.push_back(nseconds);
	}
	return times;
}



int main()
{
	const int n = 10;
	const int width = 640;
	const int height = 640;

	//Time how long it takes to generate the mandelbrot set
	std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << std::endl;
	std::vector<double> times = timeMandelbrot(n, width, height);

	//Print out relevant stats about the timing
	double total = std::accumulate(times.begin(), times.end(), 0.0);
	double average = total / double(times.size());
	auto calcStdev = [&average](double total, double newval) {return total + pow(newval - average,2); };
	double var = std::accumulate(times.begin(), times.end(), 0.0, calcStdev) / double(times.size());
	double stdev = sqrt(var);
	std::cout << "Total time: " << std::setw(18) << total << " seconds" << std::endl;
	std::cout << "Average time: " << std::setw(16) << average << " seconds" << std::endl;
	std::cout << "Standard Deviation: " << std::setw(10) << stdev << " seconds" << std::endl;

	//Save a copy of the image
	ppmImage_H::image img = generateMandelbrot(width, height);
	std::ofstream ofile;
	ofile.open("mandelbrot.ppm");
	ppmImage_H::writeImage(img, ofile);
	ofile.close();
	std::cout << "File saved as mandelbrot.ppm" << std::endl;


	//system("pause");
	return 0;
}