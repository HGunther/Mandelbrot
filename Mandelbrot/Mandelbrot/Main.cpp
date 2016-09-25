#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <thread>
#include <atomic>
#include <cmath>
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
	auto x = [width, height]() {return Mandelbrot_H::seriesMandelbrot(width, height); };
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
	auto x = [width, height]() {return Mandelbrot_H::threadedMethod1(width, height); };
	for (int i = 0; i < n; i++)
	{
		double nseconds = double(TimeF_H::timeFunction(x).count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
		times.push_back(nseconds);
	}
	return times;
}

auto timeThreadedMethod2(const int n, const int width, const int height)
{
	std::vector<double> times{};
	auto x = [width, height]() {return Mandelbrot_H::threadedMethod2(width, height); };
	for (int i = 0; i < n; i++)
	{
		double nseconds = double(TimeF_H::timeFunction(x).count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
		times.push_back(nseconds);
	}
	return times;
}

auto timeThreadpoolRow(const int n, const int width, const int height, const int numThreads)
{
	std::vector<double> times{};
	auto x = [&width, &height, &numThreads]() {return Mandelbrot_H::threadpoolRow(width, height, numThreads); };
	for (int i = 0; i < n; i++)
	{
		double nseconds = double(TimeF_H::timeFunction(x).count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
		times.push_back(nseconds);
	}
	return times;
}

auto timeThreadpoolMultiRow(const int n, const int width, const int height, const int numThreads, const int chunk_size)
{
	std::vector<double> times{};
	auto x = [&width, &height, &numThreads, &chunk_size]() {return Mandelbrot_H::threadpoolMultiRow(width, height, numThreads, chunk_size); };
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
	const int width = 350;
	const int height = 200;

	std::vector<double> times;

	//Time how long it takes to generate the mandelbrot set
	std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using in series algorithm." << std::endl;
	times = timeSeriesMandelbrot(n, width, height);
	printStats(times);  //Print out stats for in series calculations


	////Time how it takes to generate with threads using method 1
	//std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using threaded algorithm 1." << std::endl;
	//times = timeThreadedMethod1(n, width, height);
	//printStats(times);

	//Time how it takes to generate with threads using method 2
	std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using threaded algorithm 2." << std::endl;
	times = timeThreadedMethod2(n, width, height);
	printStats(times);


	std::function<void(int,int,int)> num_cores_test_funct = [](int n, int height, int width) {
		unsigned num_threads = n; //std::thread::hardware_concurrency();
		//Time how it takes to generate with using the threadpool method
		std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using a threadpool with " << num_threads << " threads." << std::endl;
		std::vector<double> times = timeThreadpoolRow(n, width, height, num_threads);
		printStats(times);
	};
	for (int i = 0; i < 8; i++)
	{
		num_cores_test_funct(i, height, width);
	}


	std::function<void(int, int, int, int)> chunk_size_test_funct = [](int n, int height, int width, int chunk_size) {
		unsigned num_threads = n; //std::thread::hardware_concurrency();
								  //Time how it takes to generate with using the threadpool method
		std::cout << "Generating " << n << " mandelbrots of size " << width << "x" << height << " using a threadpool with " << num_threads << " threads, each thread using jobs of size " << chunk_size << " pixels." << std::endl;
		std::vector<double> times = timeThreadpoolMultiRow(n, width, height, num_threads, chunk_size);
		printStats(times);
	};
	for (int i = 0; std::pow(2, i) < height*width; i++)
	{
		chunk_size_test_funct(4, height, width, std::pow(2, i));
	}




	//Save a copy of the image
	ppmImage_H::image img = Mandelbrot_H::threadpoolRow(width, height, std::thread::hardware_concurrency());
	std::ofstream ofile;
	ofile.open("mandelbrot.ppm");
	ppmImage_H::writeImage(img, ofile);
	ofile.close();
	std::cout << "File saved as mandelbrot.ppm" << std::endl;


	system("pause");
	return 0;
}
