#include "CImg.h"
#include "math.h"
#include <iostream>
#include <thread>

using namespace std;
using namespace cimg_library;
using namespace std::chrono;
const double PI = 3.1415927;

/* Gaussian Blur function
	input source image, width, height, radius
	output blur image
*/
void gaussBlur(CImg<unsigned char> &ref, CImg<unsigned char> &src, int w, int h, float r,int start, int skip) {
//	CImg<unsigned char> result= src;
	
	int rs = (int)ceil(r * 2.57);     // significant radius
	for (int i = start; i < h; i+=skip) { //for every column
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
					redSum += ((double)ref(x, y, 0) * wght);
					greenSum += ((double)ref(x, y, 1) * wght);
					blueSum += ((double)ref(x, y, 2) * wght);
					wSum += wght;
				}
			}
			src(j, i, 0) = round(redSum / wSum);
			src(j, i, 1) = round(greenSum / wSum);
			src(j, i, 2) = round(blueSum / wSum);
		}
	}
}
/*
void threadTests(CImg<unsigned char>* imgage , CImg<unsigned char>* blur){
	// clock variables
	high_resolution_clock::time_point start;
	high_resolution_clock::time_point end;
	
	// test with two threads
	start = high_resolution_clock::now();
	thread gauss1(gaussBlur,std::ref(image) , std::ref(blur) ,blur.width() , blur.height() , 5 , 0 , 2 );
	thread gauss2(gaussBlur, std::ref(image), std::ref(blur) , blur.width() , blur.height() , 5 , 1, 2);
	gauss1.join();
	gauss2.join();
	end = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(end - start).count();
	cout << "the duration of two tests is:" + duration;
	blur.save("parallel_blur_two_threads.bmp");	

	// test with three threads
	// test with four threads
	// test with five threads
	// test with six threads
	// test with seven threads
	// test with eight threads
}
*/


int main() {
	// image files
	CImg<unsigned char> image("terrypratchett.bmp"), blur("terrypratchett.bmp");
	//threadTests(image , blur);

	// clock variables
	high_resolution_clock::time_point start , end;

	// 2 thread test
	start = high_resolution_clock::now();
	thread gauss1(gaussBlur,std::ref(image) , std::ref(blur) ,blur.width() , blur.height() , 5 , 0 , 2 );
	thread gauss2(gaussBlur, std::ref(image), std::ref(blur) , blur.width() , blur.height() , 5 , 1, 2);
	gauss1.join();
	gauss2.join();
	end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start).count();
	cout <<"the duration of the two thread test is: ";
	cout << duration;
	blur.save("parallel_blur_two_threads.bmp");	
	gauss1.~thread();
	gauss2.~thread();

	// 4 thread test
	start = high_resolution_clock::now();
	thread gauss4_1(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 0 , 4);
	thread gauss4_2(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 1 , 4);
	thread gauss3(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 2 , 4);
	thread gauss4(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 3 , 4);
	
	gauss4_1.join();
	gauss4_2.join();
	gauss3.join();
	gauss4.join();

	end = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(end - start).count();
	cout << "\nthe duration of the four thread test is: ";
	cout << duration;
	blur.save("parallel_blur_four_threads.bmp");
	// destruct threads used in 4 test
	gauss4_1.~thread();
	gauss4_2.~thread();
	gauss3.~thread();
	gauss4.~thread();	
	return 0;

}
