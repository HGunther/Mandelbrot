#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <thread>
#include "TimeF.h"
#include "Mandelbrot.h"


int main()
{
	const int n = 10;
	const int width = 640;
	const int height = 640;

	//Time how long it takes to generate the mandelbrot set
	std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using in series algorithm." << std::endl;
	std::vector<double> times = Mandelbrot_H::timeSeriesMandelbrot(n, width, height);

	//Print out relevant stats about the timing
	double total = std::accumulate(times.begin(), times.end(), 0.0);
	double average = total / double(times.size());
	auto calcStdev = [&average](double total, double newval) {return total + pow(newval - average,2); };
	double var = std::accumulate(times.begin(), times.end(), 0.0, calcStdev) / double(times.size());
	double stdev = sqrt(var);
	std::cout << "Total time: " << std::setw(18) << total << " seconds" << std::endl;
	std::cout << "Average time: " << std::setw(16) << average << " seconds" << std::endl;
	std::cout << "Standard Deviation: " << std::setw(10) << stdev << " seconds" << std::endl;

	
	//Time how it takes to generate using threads
	std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using threaded algorithm." << std::endl;
	std::vector<double> times = Mandelbrot_H::timeThreadedMandelbrot(n, width, height);

	//Print out stats for thread timing
	double total = std::accumulate(times.begin(), times.end(), 0.0);
	double average = total / double(times.size());
	auto calcStdev = [&average](double total, double newval) {return total + pow(newval - average, 2); };
	double var = std::accumulate(times.begin(), times.end(), 0.0, calcStdev) / double(times.size());
	double stdev = sqrt(var);
	std::cout << "Total time: " << std::setw(18) << total << " seconds" << std::endl;
	std::cout << "Average time: " << std::setw(16) << average << " seconds" << std::endl;
	std::cout << "Standard Deviation: " << std::setw(10) << stdev << " seconds" << std::endl;


	//Save a copy of the image
	ppmImage_H::image img = seriesMandelbrot(width, height);
	std::ofstream ofile;
	ofile.open("mandelbrot.ppm");
	ppmImage_H::writeImage(img, ofile);
	ofile.close();
	std::cout << "File saved as mandelbrot.ppm" << std::endl;


	//system("pause");
	return 0;
}