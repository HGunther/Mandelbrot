#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include "TimeF.h"
#include "Mandelbrot.h"

auto timeSeriesMandelbrot(const int n, const int width, const int height)
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

auto timeThreadedMethod1(const int n, const int width, const int height)
{
	std::vector<double> times{};
	auto x = [width, height]() {return threadedMethod1(width, height); };
	for (int i = 0; i < n; i++)
	{
		double nseconds = double(TimeF_H::timeFunction(x).count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
		times.push_back(nseconds);
		std::cout << i+1 << "th run finshed." << std::endl;
	}
	return times;
}

int main()
{
	const int n = 10;
	const int width = 50;
	const int height = 50;

	//Time how long it takes to generate the mandelbrot set
	std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using in series algorithm." << std::endl;
	std::vector<double> times = timeSeriesMandelbrot(n, width, height);

	//Print out relevant stats about the timing
	double total = std::accumulate(times.begin(), times.end(), 0.0);
	double average = total / double(times.size());
	auto calcStdev = [&average](double total, double newval) {return total + pow(newval - average,2); };
	double var = std::accumulate(times.begin(), times.end(), 0.0, calcStdev) / double(times.size());
	double stdev = sqrt(var);
	std::cout << "Total time: " << std::setw(18) << total << " seconds" << std::endl;
	std::cout << "Average time: " << std::setw(16) << average << " seconds" << std::endl;
	std::cout << "Standard Deviation: " << std::setw(10) << stdev << " seconds" << std::endl;
	std::cout << std::endl;

	//Time how it takes to generate using threads
	std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using threaded algorithm." << std::endl;
	times = Mandelbrot_H::timeThreadedMandelbrot(n, width, height);

	//Print out stats for thread timing
	total = std::accumulate(times.begin(), times.end(), 0.0);
	average = total / double(times.size());
	//Defined above: auto calcStdev = [&average](double total, double newval) {return total + pow(newval - average, 2); };
	var = std::accumulate(times.begin(), times.end(), 0.0, calcStdev) / double(times.size());
	stdev = sqrt(var);
	std::cout << "Total time: " << std::setw(18) << total << " seconds" << std::endl;
	std::cout << "Average time: " << std::setw(16) << average << " seconds" << std::endl;
	std::cout << "Standard Deviation: " << std::setw(10) << stdev << " seconds" << std::endl;
	std::cout << std::endl;


	////Save a copy of the image
	//ppmImage_H::image img = seriesMandelbrot(width, height);
	//std::ofstream ofile;
	//ofile.open("mandelbrot.ppm");
	//ppmImage_H::writeImage(img, ofile);
	//ofile.close();
	//std::cout << "File saved as mandelbrot.ppm" << std::endl;


	system("pause");
	return 0;
}