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
void gaussBlur(CImg<unsigned char> &ref, CImg<unsigned char> &src, int w, int h, float r, int startX, int startY) {
//	CImg<unsigned char> result= src;

	int rs = (int)ceil(r * 2.57);     // significant radius
	for (int i = startY; i < h; i++) { //for every column
		for (int j = startX; j < w; j++) { //for every row
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


int main() {
	// image files
	CImg<unsigned char> image("terrypratchett.bmp"), blur("terrypratchett.bmp");
	//threadTests(image , blur);

	// clock variables
	high_resolution_clock::time_point start , end;

/*
	// 1 thread test
	start = high_resolution_clock::now();
	thread single(gaussBlur , std::ref(image) , std::ref(blur) ,blur.width() , blur.height() , 5 , 0 , 2 );
	single.join();
	end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start).count();
	cout <<"the duration of the two thread test is: ";
	cout << duration;
	single.~thread();
*/

/*
	// 2 thread test
	start = high_resolution_clock::now();
	thread gauss1(gaussBlur,std::ref(image) , std::ref(blur) ,blur.width() / 2 , blur.height() , 5 , 0 , 0 );
	thread gauss2(gaussBlur, std::ref(image), std::ref(blur) , blur.width() , blur.height() , 5 , blur.width() / 2 , 0);
	gauss1.join();
	gauss2.join();
	end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start).count();
	cout <<"the duration of the two thread test is: ";
	cout << duration;
	blur.save("parallel_blur_two_subdivides.bmp");
	gauss1.~thread();
	gauss2.~thread();
*/


	// 4 thread test
	start = high_resolution_clock::now();
	thread gauss4_1(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() / 2 , blur.height() / 2, 5 , 0 , 0);
	thread gauss4_2(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() / 2 , 5 , blur.width() / 2 , 0);
	thread gauss3(gaussBlur , std::ref(image) , std::ref(blur) , blur.width()  / 2 , blur.height(), 5 , 0 , blur.height() / 2);
	thread gauss4(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , blur.width() / 2 , blur.height() / 2);

	gauss4_1.join();
	gauss4_2.join();
	gauss3.join();
	gauss4.join();

	end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start).count();
	cout << "\nthe duration of the four thread test is: ";
	cout << duration;
	blur.save("parallel_blur_four_threads.bmp");
	// destruct threads used in 4 test
	gauss4_1.~thread();
	gauss4_2.~thread();
	gauss3.~thread();
	gauss4.~thread();

/*
	// 8 thread test
	start = high_resolution_clock::now();
	thread gauss8_1(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 0 , 8);
	thread gauss8_2(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 1 , 8);
	thread gauss8_3(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 2 , 8);
	thread gauss8_4(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 3 , 8);
	thread gauss8_5(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 4 , 8);
	thread gauss8_6(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 5 , 8);
	thread gauss8_7(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 6 , 8);
	thread gauss8_8(gaussBlur , std::ref(image) , std::ref(blur) , blur.width() , blur.height() , 5 , 7 , 8);

	gauss8_1.join();
	gauss8_2.join();
	gauss8_3.join();
	gauss8_4.join();
	gauss8_5.join();
	gauss8_6.join();
	gauss8_7.join();
	gauss8_8.join();

	end = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(end - start).count();
	cout <<"\nthe duration of the 8 thread test is: ";
	cout << duration;
	blur.save("parallel_blur_eight_threads.bmp");

	// destruct threads
	gauss8_1.~thread();
	gauss8_2.~thread();
	gauss8_3.~thread();
	gauss8_4.~thread();
	gauss8_5.~thread();
	gauss8_6.~thread();
	gauss8_7.~thread();
	gauss8_8.~thread();

  */
	return 0;

}
