#include "CImg.h"
#include "math.h"
#include <chrono>
#include <iostream>
using namespace cimg_library;
using namespace std;
using namespace std::chrono;

const double PI = 3.1415927;

/* Gaussian Blur function
	input source image, width, height, radius
	output blur image
*/
CImg<unsigned char> gaussBlur(CImg<unsigned char>& src, int w, int h, float r) {
	CImg<unsigned char> result= src;

	int rs = (int)ceil(r * 2.57);     // significant radius
	for (int i = 0; i < h; i++) { //for every column
		for (int j = 0; j < w; j++) { //for every row
			double		//RGB channels sum, weights sum
				redSum = 0,
				greenSum = 0,
				blueSum = 0,
				wSum = 0;

			for (int iy = i - rs; iy < i + rs + 1; iy++) {//x blur mask position
				for (int ix = j - rs; ix < j + rs + 1; ix++) {//y blur mask position
					int x = fmin(w - 1, fmax(0, ix));
					int y = fmin(h - 1, fmax(0, iy));
					double dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i); // position square
					double wght = exp(-dsq / (2 * r*r)) / (PI * 2 * r*r); //weight
					redSum += ((double)src(x, y, 0) * wght);
					greenSum += ((double)src(x, y, 1) * wght);
					blueSum += ((double)src(x, y, 2) * wght);
					wSum += wght;
				}
			}
			result(j, i, 0) = round(redSum / wSum);
			result(j, i, 1) = round(greenSum / wSum);
			result(j, i, 2) = round(blueSum / wSum);
		}
	}
	return result;
}



/* Gaussian Blur function that works on horizontal
	input source image, width, height, radius
	output blur image
*/
void gaussBlurHoriz(CImg<unsigned char>& src, CImg<unsigned char>& dest, int w, int h, float r) {
	//CImg<unsigned char> result= src;

	int rs = (int)ceil(r * 2.57);     // significant radius

  // for every pixel do a 1d blur in x direction
	for (int i = 0; i < h; i++) { //for every column
		for (int j = 0; j < w; j++) { //for every row
			double		//RGB channels sum, weights sum
				redSum = 0,
				greenSum = 0,
				blueSum = 0,
				wSum = 0;

			int iy = i; //y position
				for (int ix = j - rs; ix < j + rs + 1; ix++) {//y blur mask position
					int x = fmin(w - 1, fmax(0, ix));
					int y = fmin(h - 1, fmax(0, iy));
					double dsq = (ix - j)*(ix - j); // + (iy - i)*(iy - i); // position square
					double wght = exp(-dsq / (2 * r*r)) / sqrt(PI * 2 * r*r); //weight
					redSum += ((double)src(x, y, 0) * wght);
					greenSum += ((double)src(x, y, 1) * wght);
					blueSum += ((double)src(x, y, 2) * wght);
					wSum += wght;
				}

			dest(j, i, 0) = round(redSum / wSum);
			dest(j, i, 1) = round(greenSum / wSum);
			dest(j, i, 2) = round(blueSum / wSum);
		}
	}

// for every pixel do a 1d blur in y direction
  for (int i = 0; i < h; i++) { //for every column
		for (int j = 0; j < w; j++) { //for every row
			double		//RGB channels sum, weights sum
				redSum = 0,
				greenSum = 0,
				blueSum = 0,
				wSum = 0;

      int ix = j;
			for (int iy = i - rs; iy < i + rs + 1; iy++) {//x blur mask position
					int x = fmin(w - 1, fmax(0, ix));
					int y = fmin(h - 1, fmax(0, iy));
					double dsq = (iy - i)*(iy - i); // position square
					double wght = exp(-dsq / (2 * r*r)) / sqrt(PI * 2 * r*r); //weight
					redSum += ((double)src(x, y, 0) * wght);
					greenSum += ((double)src(x, y, 1) * wght);
					blueSum += ((double)src(x, y, 2) * wght);
					wSum += wght;

			}
			dest(j, i, 0) += round(redSum / wSum);
			dest(j, i, 1) += round(greenSum / wSum);
			dest(j, i, 2) += round(blueSum / wSum);
		}
	}
}




int main() {
	// load images
	CImg<unsigned char> image("terrypratchett.bmp"), blur("terrypratchett.bmp");

	// capture system time as blur begins
	high_resolution_clock::time_point start = high_resolution_clock::now();

	// create a blurred image based off of our first image
	gaussBlurHoriz(image, blur , image.width(), image.height(), 5);


	// capture the sytem time after our blur algorithm has finished
	high_resolution_clock::time_point end = high_resolution_clock::now();

	// display the elapsed time for our blur method
	auto duration = duration_cast<milliseconds>(end - start).count();
	cout << duration;

	/*
	// display our blurred image on screen
	CImgDisplay main_disp(blur, "GaussianBlur");
	// loop while the image is still being displayed
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
	// save the blurred image and exit
	*/
	blur.save("blur_separate_pass.bmp");
	return 0;
}
