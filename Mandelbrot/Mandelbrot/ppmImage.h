#ifndef ppmImage_H
#define ppmImage_H
#include <cmath>
#include <vector>
#include <iostream>

struct Color {
	Color() { r = g = b = 0; };
	Color(int _r, int _g, int _b) { r = _r; g = _g; b = _b; };
	int r;
	int g;
	int b;
};

using image = std::vector< std::vector<Color> >;

/**
A function to print out an image in plain PPM format.
Recieves an image (std::vector<std::vector<Color>>) and an output stream (usually an ofstream).
Returns void.
**/
void writeImage(image const& img, std::ostream & os)
{
	int width = img[0].size();
	int height = img.size();
	int max_color_value = 255;

	os << "P3" << std::endl;
	os << width << " " << height<< std::endl;
	os << max_color_value << std::endl;
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			os << img[h][w].r << " " << img[h][w].g << " " << img[h][w].b;
			if (w != width - 1)
				os << " ";
		}
		os << std::endl;
	}
	return;
}

Color colorize(int x)
{
	Color temp(0, 0, 0);

	if (x * x > 255) //blue is sensitive to high values
		temp.b = 255;
	else
		temp.b = x * x;
	temp.r = int(std::sqrt(x) * std::sqrt(255)); //red is sensitive to low values
	temp.g = x; // green makes the picture white instead of purple
	return temp;
}

#endif
