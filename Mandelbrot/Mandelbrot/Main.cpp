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

void printStats(std::vector<double> times)
{
	double total = std::accumulate(times.begin(), times.end(), 0.0);
	double average = total / double(times.size());
	auto calcStdev = [&average](double total, double newval) {return total + pow(newval - average, 2); };
	double var = std::accumulate(times.begin(), times.end(), 0.0, calcStdev) / double(times.size());
	double stdev = sqrt(var);
	std::cout << "Total time: " << std::setw(18) << total << " seconds" << std::endl;
	std::cout << "Average time: " << std::setw(16) << average << " seconds" << std::endl;
	std::cout << "Standard Deviation: " << std::setw(10) << stdev << " seconds" << std::endl;
	std::cout << std::endl;
	return;
}

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
		std::cout << i+1 << " run finshed." << std::endl;
	}
	return times;
}

auto timeThreadedMethod2(const int n, const int width, const int height)
{
	std::vector<double> times{};
	auto x = [width, height]() {return threadedMethod2(width, height); };
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
	std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using in series algorithm." << std::endl;
	std::vector<double> times = timeSeriesMandelbrot(10, width, height);
	printStats(times);  //Print out stats for in series calculations


	//Time how it takes to generate with threads using method 1
	std::cout << "Generating " << n / 3 << " mandelbrots of size " << width/10 << "x" << height/10 << " using threaded algorithm 1." << std::endl;
	std::cout << "This method is crazy slow. That's why it is doing fewer, smaller sets." << std::endl;
	times = Mandelbrot_H::timeThreadedMethod1(n / 3, width/10, height/10);
	printStats(times);

	//Time how it takes to generate with threads using method 2
	std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using threaded algorithm 2." << std::endl;
	times = Mandelbrot_H::timeThreadedMethod2(10, width, height);
	printStats(times);


	//Save a copy of the image
	ppmImage_H::image img = threadedMethod2(width, height);
	std::ofstream ofile;
	ofile.open("mandelbrot.ppm");
	ppmImage_H::writeImage(img, ofile);
	ofile.close();
	std::cout << "File saved as mandelbrot.ppm" << std::endl;


	system("pause");
	return 0;
}