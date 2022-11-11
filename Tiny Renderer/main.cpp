#include <iostream>
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

/* First attempt :
	
*/
void line00(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	for (float i = 0; i < 1.; i += .01)
	{
		int x = x0 + (x1 - x0) * i;
		int y = y0 + (y1 - y0) * i;
		image.set(x, y, color);
	}
}

void line01(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;
		image.set(x, y, color);
	}
}

void line02(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false; // steep means that y range is bigger than x range.
	if(std::abs(x1-x0) < std::abs(y1-y0))
	{
		steep = true;
		std::swap(x0,y0);
		std::swap(x1, y1);
	} // solve pixelation errors

	if (x1 < x0)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	} // solve undrawn lines

	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;
		if(steep) image.set(y, x, color); else image.set(x, y, color);
	}
}
void line03(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false; // steep means that y range is bigger than x range.
	if (std::abs(x1 - x0) < std::abs(y1 - y0))
	{
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	} // solve pixelation errors

	if (x1 < x0)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	} // solve undrawn lines

	int xRange = x1 - x0;
	int yRange = y1 - y0;
	float slope = (float) yRange / (float) xRange;
	float error = 0;
	int y = y0;
	std::cout << "Slope : " << slope << std::endl;

	for (int x = x0; x <= x1; x++)
	{
		if (steep) image.set(y, x, color); else image.set(x, y, color);
		error += slope;
		std::cout << "error : " << error << std::endl;
		if (error > .5) {
			y += (y1 > y0 ? 1 : -1);
			std::cout << "increment y"<< std::endl;

			error -= 1.;
		}
	}
	std::cout << std::endl;
}

void line04(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false; // steep means that y range is bigger than x range.
	if (std::abs(x1 - x0) < std::abs(y1 - y0))
	{
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	} // solve pixelation errors

	if (x1 < x0)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	} // solve undrawn lines

	int xRange = x1 - x0;
	int yRange = y1 - y0;
	int slope = std::abs(yRange) * 2;
	int error = 0;
	int y = y0;
	int yIncrement = (y1 > y0 ? 1 : -1);
	std::cout << "Slope : " << slope << std::endl;

	for (int x = x0; x <= x1; x++)
	{
		if (steep) image.set(y, x, color); else image.set(x, y, color);
		error += slope;
		std::cout << "error : " << error << std::endl;
		if (error > xRange) {
			y += yIncrement;
			std::cout << "increment y" << std::endl;

			error -= xRange*2;
		}
	}
	std::cout << std::endl;
}

int main()
{
	TGAImage image(100, 100, TGAImage::RGB);
	/* FIRST ATTEMPT:
		problems: we used a magic number range (between 0 to 1 stepping by .01) to be able to draw the line
	*/
	//line00(10,20, 40,50,image,white);

	/* SECOND ATTEMPT:
	*	solved: no magic numbers are used.
		problems:
		1 - we now depend on x solely to get the number of pixels between the 2 given coordinates, so if the range of y is bigger than x the line will appear to be pixelated.
		2 - only lines starting from a lower to a higher x value are processed, and thus they are not drawn to the screen.
	*/
	/*line01(13, 20, 80, 40, image, white);
	line01(20, 13, 40, 80, image, red);
	line01(80, 40, 13, 20, image, red);*/

	/* THIRD ATTEMPT:
		solved: all previous problems
		problems:
		needs optimization
	*/
	//line02(20, 13, 40, 80, image, red); // see if pixelated
	//line02(80, 40, 13, 20, image, red); // see if unprocessd

	/* Fourth ATTEMPT:
		solved: 
		problems: 
	*/
	//line03(20, 13, 40, 80, image, red);
	//line03(0, 0, 40, 10, image, red);
	//line03(0, 0, 10, 40, image, white);

	/* Fifth ATTEMPT:*/
	line04(0, 0, 10, 40, image, white);

	image.flip_vertically(); // set the origin at the left bottom corner of the image
	image.write_tga_file("fifth_attempt.tga");

	std::cout << "Done" << std::endl;
	return 0;
}