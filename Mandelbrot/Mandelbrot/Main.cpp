#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <random>
#include "TimeF.h"
#include "Mandelbrot.h"
#include "Main.h"

struct Color {
	int r;
	int g;
	int b;
};

using image = std::vector<std::vector<Color>>;

/**
A function to print out an image in plain PPM format.
Recieves an image (std::vector<std::vector<Color>>) and an output stream (usually an ofstream).
Returns void.
**/
void writeImage(image const& img, std::ostream & os)
{
	int width = 10;
	int height = 10;
	int max_color_value = 255;

	os << "P3" << std::endl;
	os << width << " " << height << std::endl;
	os << max_color_value << std::endl;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			os << img[i][j].r << " " << img[i][j].g << " " << img[i][j].b;
			if (j != height - 1)
				os << " ";
		}
		os << std::endl;
	}
	return;
}

int main()
{
	auto x = []() {return 1 + 2;};

	double nseconds = double(TimeF_H::timeFunction(x).count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
	std::cout << nseconds << std::endl;

	std::cout << Mandelbrot_H::Mandelbrot(1.0, 1.0) << std::endl;
	std::complex<float> c (1.7, 1.9);
	std::cout << Mandelbrot_H::Mandelbrot(c) << std::endl;

	image img;

	std::array<std::array<Color, 10>,10> y;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 255);
	for (int i = 0; i < 10; i++)
	{
		std::vector<Color> v{};
		for (int j = 0; j < 10; j++)
		{
			Color temp;
			temp.r = 0; // distribution(generator);
			temp.g = 0; // distribution(generator);
			temp.b = distribution(generator);
			v.push_back(temp);
		}
		img.push_back(v);
	}

	std::ofstream ofile;
	ofile.open("test.ppm");
	writeImage(img, ofile);
	ofile.close();

	system("pause");
	return 0;
}