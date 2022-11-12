#include <iostream>
#include "tgaimage.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model *model = NULL;
int width = 800;
int height = 800;

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
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

	for (int x = x0; x <= x1; x++)
	{
		if (steep) image.set(y, x, color); else image.set(x, y, color);
		error += slope;
		if (error > xRange) {
			y += yIncrement;
			error -= xRange*2;
		}
	}
}

int map(float a, float b, float c, float d, float x) {
	return (int)c + (x - a) * (d - c) / (b - a);
}

int main()
{
	model = new Model("obj/skull.obj");
	TGAImage image(width, height, TGAImage::RGB);
	
	for(int i = 0;i<model->nfaces();i++)
	{
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j+1)%3]);
			int x0 = (v0.x + 1.) * (width / 2.);
			int y0 = (v0.y + 1.) * (height / 2.);
			int x1 = (v1.x + 1.) * (width / 2.);
			int y1 = (v1.y + 1.) * (height / 2.);
			line(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically(); // set the origin at the left bottom corner of the image
	image.write_tga_file("outputs/skull.tga");

	std::cout << "Done" << std::endl;
	return 0;
}