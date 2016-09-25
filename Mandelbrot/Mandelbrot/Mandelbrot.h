#ifndef Mandelbrot_H
#define Mandelbrot_H
#include <complex>
#include <vector>
#include <array>
#include <thread>
#include <atomic>
#include "ppmImage.h"
#include "ThreadPool.h"

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
			double y = (h * 2 / float(height + 1)) + float(2 / float(height) / 2) - 1;
			img[h][w] = ppmImage_H::colorize(int(Mandelbrot(x, y) / 1000.0 * 255));
		}
	}
	return img;
}

image threadedMethod1(const int width, const int height)
{
	ppmImage_H::image img = std::vector<std::vector<ppmImage_H::Color>>(height, std::vector<ppmImage_H::Color>(width, Color()));
	std::vector<std::thread> threads{};// = std::vector<std::vector<std::thread>>(height, std::vector<std::thread>(width));

	auto calcRow = [&img, width, height](int h)
	{
		for (int w = 0; w < width; w++)
		{
			//Map from pixel coordinates to valid mandelbrot area (x:-2.5 to 1, y: -1 to 1)
			double x = (w * 3.5 / float(width + 1)) + float(3.5 / float(width) / 2.0) - 2.5;
			double y = (h * 2 / float(height + 1)) + float(2 / float(height) / 2) - 1;

			img[h][w] = ppmImage_H::colorize(int(Mandelbrot(x, y) / 1000.0 * 255));
		}
	};

	//Create threads
	for (int h = 0; h < height; h++)
	{
		threads.emplace_back(std::thread(calcRow, h));
	}

	//Wait for threads to finish
	for (int h = 0; h < height; h++)
	{
			threads[h].join();
	}

	return img;
}

image threadedMethod2(const int width, const int height)
{
	struct param
	{
		param() { x = 0; y = 0; }
		param(double a, double b) { x = a; y = b; }
		double x;
		double y;
	};

	std::atomic_int counter(0);
	ppmImage_H::image img = std::vector<std::vector<ppmImage_H::Color>>(height, std::vector<ppmImage_H::Color>(width, Color()));
	std::vector<std::vector<param>> pool = std::vector<std::vector<param>>(height, std::vector<param>(width));

	//Initialize the pool with the values I want to calculate
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			//Map from pixel coordinates to valid mandelbrot area (x:-2.5 to 1, y: -1 to 1)
			double x = (w * 3.5 / float(width + 1)) + float(3.5 / float(width) / 2.0) - 2.5;
			double y = (h * 2 / float(height + 1)) + float(2 / float(height) / 2) - 1;
			pool[h][w] = param(x, y);
		}
	}

	auto processor = [&img, &counter, &pool, height, width]()
	{
		int temp = counter++;
		while (temp < height * width)
		{
			int h = temp / width;
			int w = temp % width;
			img[h][w] = ppmImage_H::colorize(int(Mandelbrot(pool[h][w].x, pool[h][w].y) / 1000.0 * 255));
			temp = counter++;
		}
		return;
	};

	std::vector<std::thread> threads = std::vector<std::thread>(4);

	//create threads
	for (int t = 0; t < 4; t++)
	{
		threads[t] = std::thread(processor);
	}

	//close threads
	for (int t = 0; t < 4; t++)
	{
		threads[t].join();
	}

	return img;
}

image threadpoolPixel(const int width, const int height, const int num_threads)
{
	ppmImage_H::image img = std::vector<std::vector<ppmImage_H::Color>>(height, std::vector<ppmImage_H::Color>(width, Color()));

	{
		//create threadpool
		ThreadPool_H::ThreadPool pool(num_threads, width*height);

		//create jobs
		for (int h = 0; h < height; h++)
		{
			std::vector<std::function<void(void)>> jobs{};
			for (int w = 0; w < width; w++)
			{
				//Map from pixel coordinates to valid mandelbrot area (x:-2.5 to 1, y: -1 to 1)
				double x = (w * 3.5 / float(width + 1)) + float(3.5 / float(width) / 2.0) - 2.5;
				double y = (h * 2 / float(height + 1)) + float(2 / float(height) / 2) - 1;
				std::function<void(void)> f = [x, y, w, h, &img]()
				{
					img[h][w] = ppmImage_H::colorize(int(Mandelbrot(x, y) / 1000.0 * 255));
					return;
				};
				jobs.push_back(f);
				//pool.post(f);
			}
			pool.postVector(jobs);
		}
		//pool is deconstructed here. It will wait for the jobs to finish before deconstructing.
	}	
	return img;
}

image threadpoolRow(const int width, const int height, const int num_threads)
{
	ppmImage_H::image img = std::vector<std::vector<ppmImage_H::Color>>(height, std::vector<ppmImage_H::Color>(width, Color()));

	{
		//create threadpool
		ThreadPool_H::ThreadPool pool(num_threads, height);

		//create jobs
		for (int h = 0; h < height; h++)
		{
			std::function<void(void)> f = [&img, h, width, height]()
				{
					for (int w = 0; w < width; w++)
					{
						//Map from pixel coordinates to valid mandelbrot area (x:-2.5 to 1, y: -1 to 1)
						double x = (w * 3.5 / float(width + 1)) + float(3.5 / float(width) / 2.0) - 2.5;
						double y = (h * 2 / float(height + 1)) + float(2 / float(height) / 2) - 1;

						img[h][w] = ppmImage_H::colorize(int(Mandelbrot(x, y) / 1000.0 * 255));
					}
				};
			pool.post(f);
		}
		//pool is deconstructed here. It will wait for the jobs to finish before deconstructing.
	}
	return img;
}

image threadpoolMultiRow(const int width, const int height, const int num_threads, const long chunk_size)
{
	ppmImage_H::image img = std::vector<std::vector<ppmImage_H::Color>>(height, std::vector<ppmImage_H::Color>(width, Color()));
	unsigned num_jobs = std::ceil(width*height / float(chunk_size));
	{
		//create threadpool
		ThreadPool_H::ThreadPool pool(num_threads, num_jobs );

		//create jobs
		for (unsigned i = 0; i < num_jobs; i++)
		{
			std::function<void(void)> f = [&img, i, chunk_size, width, height]()
			{
				for (int p = 0; p < chunk_size && (p+ chunk_size*i) < width*height; p++)
				{
					int h = (i*chunk_size + p) / width;
					int w = (i*chunk_size + p) % width;
					
					//Map from pixel coordinates to valid mandelbrot area (x:-2.5 to 1, y: -1 to 1)
					double x = (w * 3.5 / float(width + 1)) + float(3.5 / float(width) / 2.0) - 2.5;
					double y = (h * 2 / float(height + 1)) + float(2 / float(height) / 2) - 1;

					img[h][w] = ppmImage_H::colorize(int(Mandelbrot(x, y) / 1000.0 * 255));
				}
			};
			pool.post(f);
		}

		//pool is deconstructed here. It will wait for the jobs to finish before deconstructing.
	}
	return img;
}

#endif
