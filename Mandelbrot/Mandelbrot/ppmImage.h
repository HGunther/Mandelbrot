#ifndef ppmImage_H
#define ppmImage_H

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

#endif